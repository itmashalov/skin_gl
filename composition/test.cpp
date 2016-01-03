#include "test.h"
#include <QTimer>
#include <QTimerEvent>
#include <QDateTime>
#include <QElapsedTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>
#include <QFont>
#include <QLabel>
#include <stdio.h>
#include <math.h>
#include <QGLWidget>

#define TO_WIDTH_FACTOR 2.0f
#define TO_HEIGHT_FACTOR 0.5f

namespace htl_qt_widgets
{
namespace
{
struct MoveHistory_lt
{
  int position;
  qint64 time;

  MoveHistory_lt(int position_, qint64 time_):
    position(position_),
    time(time_)
  {

  }
};
}

//##################################################################################################
struct test::Private
{
  test* q;

  QPixmap cache;
  QImage bigText;
  QImage mask;
  bool invalidateCache;

  QString text;
  QString units;
  QColor color;
  QColor fadeColor;
  QColor textColor;
  float fraction;
  float resize;
  int lastPosition;
  float pixelScale;
  float speed;
  int bounceIndex;
  int resIndex;
  float bounce[10];
  float res[10];
  float diff;

  int fullValue;
  int newFullValue;

  int lowerLimit;
  int upperLimit;

  bool mousePressed;
  bool emitSignal;

  QList<MoveHistory_lt> moveHistory;


  //################################################################################################
  Private(test* q_):
    q(q_),
    color(62, 169, 244),
    fadeColor(62, 169, 244,0),
    textColor(Qt::white),
    fraction(0.0f),
    resize(1.0f),
    lastPosition(0),
    pixelScale(1.0f),
    speed(0.0f),
    bounceIndex(10),
    resIndex(3),
    diff(0.0f),
    fullValue(0),
    newFullValue(0),
    lowerLimit(0),
    upperLimit(0),
    mousePressed(false),
    emitSignal(false)
  {
    bounce[0] =  0.99f;
    bounce[1] =  0.76f;
    bounce[2] =  0.46f;
    bounce[3] =  0.00f;
    bounce[4] = -0.23f;
    bounce[5] = -0.37f;
    bounce[6] = -0.47f;
    bounce[7] = -0.37f;
    bounce[8] = -0.23f;
    bounce[9] =  0.00f;

    for(int i=0; i<10; i++)
      bounce[i]*=0.9f;

    res[0] =  1.030f;
    res[1] =  1.060f;
    res[2] =  1.030f;
    res[3] =  1.00f;
  }


  //################################################################################################

void test::paintEvent(QPaintEvent* event)
{

  Q_UNUSED(event)
  QPainter p(this);

  if(!p.isActive() || width()<10 || height()<10)
    return;

  if(d->invalidateCache)
  {
    d->invalidateCache = false;

    //We adjust the scaling here to make this look good on tablets like the iPad, this is because
    //every pixel in our coordinate system is 2 in device coordinates, making the cach jagged
    float scaleX = p.deviceTransform().m11();
    float scaleY = p.deviceTransform().m22();

    QSize adjustedSize(float(contentsRect().width())*scaleX, float(contentsRect().height())*scaleY);

    float w = adjustedSize.width();
    float h = w * TO_HEIGHT_FACTOR;
    if(h>adjustedSize.height())
    {
      h = adjustedSize.height();

    }

    float widthFactor = w/h;
    if(d->cache.size() != adjustedSize)
    {
      d->cache = QPixmap(adjustedSize);
      d->bigText = QImage(adjustedSize, QImage::Format_ARGB32_Premultiplied);
      d->mask = QImage(adjustedSize, QImage::Format_ARGB32_Premultiplied);

      //-- Mask for the big text -------------------------------------------------------------------
      {
        d->mask.fill(QColor(0, 0, 0, 0));
        QPainter painter(&d->mask);

        painter.scale(h, h);
        painter.translate(widthFactor*0.25, 0.5f);
        painter.scale(0.9f, 1.3f);

        painter.setPen(Qt::NoPen);
        QRadialGradient gradiant(0.0f, 0.0f, 0.27f);
        gradiant.setColorAt(0.72f, QColor( 0, 0, 0));
        gradiant.setColorAt(0.95f, QColor( 0, 0, 0, 0));
        painter.setBrush(gradiant);

        painter.drawEllipse(QPointF(0.0f, 0.0f), 0.40f*d->resize, 0.40f*d->resize);
      }
    }

    d->cache.fill(QColor(0, 0, 0, 0));
    d->bigText.fill(QColor(d->color.red(), d->color.green(), d->color.blue(), 0));

    //-- Big text ----------------------------------------------------------------------------------
    {
      QFont f = font();
      QPainterPath yPath;
      yPath.addText(0, 0, f, "0123456789");
      QRectF yr = yPath.boundingRect();

      QPainterPath textPath;
      textPath.addText(0, 0, f, QString::number(d->fullValue));

      QRectF r = textPath.boundingRect();
      textPath.translate(-(r.x()+(r.width()/2.0f)), 0.0f);

      //Full value + 1
      qreal left = r.right()+(yr.height()*0.6);
      if (d->fullValue<d->upperLimit)
      {
        r = textPath.boundingRect();
        textPath.addText(left, 0, f, QString::number(d->fullValue+1));
      }

      r = textPath.boundingRect();
      qreal center = ((r.right() - left) / 2.0) + left;
      center+=0.1;
      textPath.translate(-(d->fraction*center), -(yr.y()+(yr.height()/2.0f)));

      QPainter painter(&d->bigText);
      painter.setRenderHint(QPainter::Antialiasing, true);

      painter.scale(h, h);
      painter.translate(widthFactor*0.25, 0.487f);
      float textScale;
      if(d->fullValue<100){
        textScale = 0.24f / yr.height();
      }

      if(d->fullValue>99){
        textScale = 0.17f / yr.height();
      }

      painter.scale(textScale, textScale);
      d->pixelScale = center * textScale * h;

      painter.setBrush(d->textColor);
      painter.setPen(Qt::NoPen);

      painter.drawPath(textPath);
    }

    //-- Apply mask to big text --------------------------------------------------------------------
    {
      int yMax = d->mask.height();
      int xMax = d->mask.width();

      for(int y=0; y<yMax; y++)
      {
        uchar* aPtr = d->bigText.scanLine(y);
        const uchar* aMax = aPtr + (xMax*4);

        uchar* bPtr = d->mask.scanLine(y);

        for(; aPtr<aMax;)
        {
          //(*aPtr)=(*bPtr);
          aPtr++;bPtr++;
          //(*aPtr)=(*bPtr);
          aPtr++;bPtr++;
          //(*aPtr)=(*bPtr);
          aPtr++;bPtr++;

          float a=((float(*aPtr)/255.0f) * (float(*bPtr)/255.0f))*255.0f;

          (*aPtr)=a;
          // (*aPtr)=(*bPtr);//
          aPtr++;bPtr++;
        }
      }
    }


    QPainter painter(&d->cache);
    painter.setRenderHint(QPainter::Antialiasing, true);
    if(h>adjustedSize.height())
    {
      h = adjustedSize.height();

    }

    //-- Generate the paths ------------------------------------------------------------------------
    QList <QPainterPath*> paths;

    QPainterPath textPath;
    QPainterPath prefixPath;
    QPainterPath units;
    QPainterPath valuePath;

    paths.append(&textPath);
    paths.append(&prefixPath);
    paths.append(&valuePath);

    QFont f = font();
    textPath.addText(0, 0, f, d->text);
    units.addText(0, 0, f, d->units);

    qreal minY = 0;
    qreal textHeight = 0;
    Q_FOREACH(QPainterPath* path, paths)
    {
      minY = qMin(path->boundingRect().y(), minY);
      textHeight = qMax(path->boundingRect().height(), textHeight);
    }

    Q_FOREACH(QPainterPath* path, paths)
      path->translate(-path->boundingRect().x(), -minY);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);

    //Big background circle
    {
      float scale = h;

      painter.save();
      painter.scale(scale, scale);
      painter.translate(widthFactor*0.25, 0.5f);

      painter.setBrush(d->color);
      painter.drawEllipse(QPointF(0.0f, 0.0f), 0.47f*d->resize, 0.47f*d->resize);
      painter.restore();

    }

    //-- Small text ----------------------------------------------------------------------------------
    {
      painter.save();
      QFont f = font();
      QPainterPath yPath;
      yPath.addText(0, 0, f, "0123456789");
      QRectF yr = yPath.boundingRect();

      //Full value
      QPainterPath textPath;
      textPath.addText(0, 0, f, QString::number(d->fullValue));

      QRectF r = textPath.boundingRect();
      textPath.translate(-(r.x()+(r.width()/2.0f)), 0.0f);

      //Full value + 1
      r = textPath.boundingRect();
      qreal left = r.right()+(yr.height()*1.6);
      if (d->fullValue<d->upperLimit)
      {
        textPath.addText(left, 0, f, QString::number(d->fullValue+1));
      }

      r = textPath.boundingRect();
      qreal center = ((r.right() - left) / 2.0) + left;
      center+=0.1;

      //Full value - 1
      if (d->fullValue>d->lowerLimit)
      {
        QPainterPath tmp;
        tmp.addText(0, 0, f, QString::number(d->fullValue-1));

        QRectF tr=tmp.boundingRect();
        qreal left = r.left()-(yr.height()*1.6)-(tr.x()+tr.width());
        tmp.translate(left, 0);
        textPath.addPath(tmp);
      }

      //Full value + 2
      if ((d->fullValue+1)<d->upperLimit)
      {
        r = textPath.boundingRect();
        textPath.addText(r.right()+(yr.height()*1.6), 0, f, QString::number(d->fullValue+2));
      }

      textPath.translate(-(d->fraction)*center, -(yr.y()+(yr.height()/2.0f)));

      //QPainter painter(&d->bigText);
      painter.setRenderHint(QPainter::Antialiasing, true);

      painter.scale(h, h);
      painter.translate(widthFactor*0.25, 0.5f);

      float textScale /*= 0.14f / yr.height()*/;
      if(d->fullValue<100){
        textScale = 0.14f / yr.height();
      }
      if(d->fullValue>99){
        textScale = 0.12f / yr.height();
      }
      painter.scale(textScale, textScale);

      //d->pixelScale = center * textScale * h;

      painter.setBrush(d->textColor);
      painter.setPen(Qt::NoPen);

      painter.drawPath(textPath);
      painter.restore();
    }

    //small circle on the top of the small text(numbers)
    {
      float scale = h;
      painter.save();
      painter.scale(scale, scale);
      QPainterPath epath;
      painter.translate(widthFactor*0.25, 0.5f);

      painter.setPen(Qt::NoPen);

      QRadialGradient gradiant(0.0f, 0.0f, 0.30f);
      gradiant.setColorAt(1.00f, d->fadeColor );//0, 178, 238//102,255,102
      gradiant.setColorAt(0.65f, d->color );

      painter.setBrush(gradiant);


      epath.addEllipse(QPointF(0.0f, 0.0f), 0.28f*d->resize, 0.86f*d->resize);

      painter.drawPath(epath);
      // painter.setClipPath(epath);

      painter.restore();
    }

    //drawing the big ellipse on the top used for fading of the circle
    {
      float scale = h;

      painter.save();
      painter.scale(scale, scale);
      painter.translate(widthFactor*0.25, 0.5f);

      QRadialGradient gradiant(0.0f, 0.0f, 0.57f);
      gradiant.setColorAt(0.72f, QColor( 0, 178, 238,0));
      gradiant.setColorAt(1.0f, QColor(0, 178, 238));

      painter.setBrush(gradiant);
      painter.drawEllipse(QPointF(0.0f, 0.0f), 0.47f*d->resize, 0.47f*d->resize);
      painter.restore();
    }

    //draw the text on the top
    {
      float scale = (h*0.08f) / textHeight;

      painter.save();
      painter.translate(w/2.0f, h/5.0f);
      painter.scale(scale, scale);

      {
        float leftOffset = -textPath.boundingRect().width()*0.25f;

        painter.save();
        painter.setBrush(QColor("#F8F8FF"));
        painter.translate(leftOffset, 6.4f);
        painter.drawPath(textPath);
        painter.restore();
      }

      painter.restore();
    }

    //Draw The UNITS
    {
      QPainter painter(&d->bigText);

      float scale = (h*0.088f) / textHeight;

      painter.save();
      painter.translate(w/2.0f, h/1.33f);
      painter.scale(scale, scale);

      {
        float leftOffset = -units.boundingRect().width()*0.5f;

        painter.setPen(Qt::NoPen);
        painter.setBrush(d->textColor);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.translate(leftOffset, 0.0f);
        painter.drawPath(units);
      }

      painter.restore();
    }

    painter.drawImage(0, 0, d->bigText);
  }

   p.setRenderHint(QPainter::Antialiasing, true);
  p.drawPixmap(rect(), d->cache);
}


};
