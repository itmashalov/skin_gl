#include "Skin.h"
#include <QTimer>
#include <QTimerEvent>
#include <QDateTime>
#include <QElapsedTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QScreen>
#include <QApplication>
#include <QPropertyAnimation>
#include <QDesktopWidget>
#include <QFont>
#include <QLabel>
#include <stdio.h>
#include <math.h>
#include <QWidget>
#include <QDebug>
#include <QOpenGLContext>

#define TO_WIDTH_FACTOR 2.0f
#define TO_HEIGHT_FACTOR 0.5f


namespace cad_mobile_application
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
struct Skin::Private
{
  Skin* q;
  QPixmap kineticPix;
  QPixmap backgroundHill;
  QPixmap leftBush;
  QPixmap rightBush;
  QPixmap mountain;
  QPixmap cloud;
  float animationStage;
  float animationStage2;
  float animationStage3;
   float animationStage4;
   QTime m_time;
   int m_frameCount;
 float bound;
  float h;

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
  bool clicked;
  int lowerLimit;
  int upperLimit;
  QPropertyAnimation* animation;
   QPropertyAnimation* animation2;
    QPropertyAnimation* animation3;
     QPropertyAnimation* animation4;

  bool mousePressed;
  bool emitSignal;

  QList<MoveHistory_lt> moveHistory;


  //###############################################################################################
  Private(Skin* q_):
    q(q_),
    kineticPix(":/res/images/foreground.svg"),
    backgroundHill(":/res/images/background.svg"),
    leftBush(":/res/images/lbush.svg"),
    rightBush(":/res/images/rbush.svg"),

    mountain(":/res/images/mount.png"),
    cloud(":/res/images/cloud.svg"),


    animationStage(0.0f),
    animationStage2(0.0f),
    animationStage3(0.0f),
     animationStage4(0.0f),
     m_time(),
      m_frameCount(0),
     bound(0.0f),
    h(q->rect().height()),
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
     clicked(false),
    lowerLimit(0),
    upperLimit(0),
    animation(NULL),
     animation2(NULL),
      animation3(NULL),
       animation4(NULL),
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
  void update()
  {

    q->update();
  }

  //################################################################################################


  //################################################################################################

};

//##################################################################################################
Skin::Skin(QGLWidget *parent):
  QGLWidget(parent),
  d(new Private(this))
{


//background hill
  d->animation = new QPropertyAnimation(this, "animationStage");
  d->animation->setStartValue(-100.0f);
  d->animation->setEndValue(300.0f);
  d->animation->setDuration(900);
  d->animation->setEasingCurve(QEasingCurve::OutBounce);

//foreground hill
  d->animation2 = new QPropertyAnimation(this, "animationStage2");
  d->animation2->setStartValue(-100.0f);
  d->animation2->setEndValue(300.0f);
  d->animation2->setDuration(1000);
  d->animation2->setEasingCurve(QEasingCurve::OutBounce);

  //bush right
    d->animation3 = new QPropertyAnimation(this, "animationStage3");
    d->animation3->setStartValue(1.0f);
    d->animation3->setEndValue(d->h);
    d->animation3->setDuration(2160);
    d->animation3->setEasingCurve(QEasingCurve::InOutBack);

    //clouds right
      d->animation4 = new QPropertyAnimation(this, "animationStage4");
      d->animation4->setStartValue(0);
      d->animation4->setEndValue(10);
      d->animation4->setDuration(1000);
      d->animation4->setEasingCurve(QEasingCurve::Linear);

}

//##################################################################################################
Skin::~Skin()
{
  delete d;
}




float Skin::animationStage()
{
  return d->animationStage;

}



//##################################################################################################
void Skin::setAnimationStage(float animationStage)
{
  d-> animationStage= qBound(-100.0f, animationStage, 300.0f);
  d->update();
}
float Skin::animationStage2()
{
  return d->animationStage2;

}
void Skin::setAnimationStage2(float animationStage2)
{
  d-> animationStage2= qBound(-100.0f, animationStage2, 300.0f);
  d->update();
}
//##################################################################################################
float Skin::animationStage3()
{
  return d->animationStage3;

}
void Skin::setAnimationStage3(float animationStage3)
{
  d-> animationStage3= qBound(0.1f, animationStage3, d->h);
  d->update();
}
//##################################################################################################
//##################################################################################################
float Skin::animationStage4()
{
  return d->animationStage4;

}
void Skin::setAnimationStage4(float animationStage4)
{
  d-> animationStage4= qBound(0.0f, animationStage4, 10.0f);
  d->update();
}
//##################################################################################################

void Skin::initializeGL()
{
    qglClearColor(QColor(Qt::black));
}

void Skin::paintGL()
{


  glColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask (GL_TRUE);

 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
 glClearColor(234/255.0, 252/255.0, 254/255.0, 0.0f);
  QPainter painter(this);
  painter.beginNativePainting();

  //hint(ENABLE_OPENGL_2X_SMOOTH);

 // glEnable(GL_NORMALIZE);
 // glShadeModel(GL_FLAT);
//  glCullFace(GL_BACK);
 // glLoadIdentity;
 // glPushMatrix();
 // glCallList(DL);
 // glPopMatrix();

//
  if (d->m_frameCount == 0) {
           d->m_time.start();
      } else {

        //  qDebug()<<d->m_time.elapsed() / float(d->m_frameCount);
            qDebug()<<d->m_frameCount / (float(d->m_time.elapsed()) / 1000.0f);
      }
     d->m_frameCount++;



  QFont font=painter.font() ;
  font.setPointSize(18);
  painter.setFont(font);
  painter.setPen(Qt::red);
  //painter.drawText(QPoint(50, 50), QString::number(d->m_frameCount / (float(d->m_time.elapsed()) / 1000.0f)));
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::TextAntialiasing);
  painter.setRenderHint(QPainter::SmoothPixmapTransform);
  painter.setRenderHint(QPainter::HighQualityAntialiasing);

  //We adjust the scaling here to make this look good on tablets like the iPad, this is because
  //every pixel in our coordinate system is 2 in device coordinates, making the cach jagged
  float scaleX = painter.deviceTransform().m11();
  float scaleY = painter.deviceTransform().m22();

  QSize adjustedSize(float(contentsRect().width())*scaleX, float(contentsRect().height())*scaleY);
  painter.scale(1.0f / scaleX, 1.0f / scaleY);
//  painter.drawPixmap(-80, 290-d->animationStage2, adjustedSize.width()/1.2, adjustedSize.height()/1.2,d->leftBush);
//  painter.drawPixmap(0, 240-d->animationStage2, adjustedSize.width()*1.5, adjustedSize.height()*1.5,d->backgroundHill);
//painter.drawPixmap(85,  adjustedSize.height()/2-d->animationStage, adjustedSize.width()/1.2, adjustedSize.height()/1.2,d->rightBush);





  float scaleW =  rect().width()/rect().height();
  float scaleH =  rect().height()/rect().width();
  float hillY=1;
  float rhillY=1;
  float mount=1;
  float mountY=1;
  if(adjustedSize.height()>1200)
  {
    hillY =1.5;
    rhillY =1.5;
    mount=1.8;

    mountY=1.2;
  }
  else
  {

    mount=1.17;
    hillY=0.8;
    rhillY=0.9f;
    mountY=0.8;
  }

  float xCloud=adjustedSize.width()+100-d->animationStage4*10-d->bound*10;
 //CLOUDS ANIMATION ONLY WORK ON HD DEVICES which are more powerful.
 if(d->animationStage4==10){
 // if(d->animationStage4==10 && (adjustedSize.height()>1400||adjustedSize.width()>1400) ){
    d->bound=d->bound+10;
   d->animationStage4=0;
   d->animation4->stop();
   d->animation4->start();
  }
if(xCloud<-adjustedSize.width()*6){
d->animationStage4=0;
d->bound=0;
d->animation4->stop();
d->animation4->start();
}

  if( rect().width()> rect().height())
  {

    //drawing the Clouds
    painter.drawPixmap(xCloud/6+adjustedSize.width()/2,  (-adjustedSize.height()/1.2f), adjustedSize.width()/4, adjustedSize.height()/1.5,d->cloud);
    painter.drawPixmap(xCloud/4+adjustedSize.width()/2,  (-adjustedSize.height()/4), adjustedSize.width()/4, adjustedSize.height()/1.5,d->cloud);

    //drawing the mountain
    painter.drawPixmap(0,  ((adjustedSize.height()/2.2/mountY +adjustedSize.height()/10)+600-d->animationStage2*3)+d->h -d->animationStage3, adjustedSize.width(), adjustedSize.height()/2.9,d->mountain);

    //drawing the Clouds
    painter.drawPixmap(xCloud/3+adjustedSize.width()/2,  (-adjustedSize.height()/8), adjustedSize.width()/4, adjustedSize.height()/1.5,d->cloud);

    //drawing Left bush
    painter.drawPixmap(adjustedSize.width()/6.8,((adjustedSize.height()/3.3f/hillY +adjustedSize.height()/5.3)-d->animationStage2)+d->h*2-d->animationStage3*2, adjustedSize.width()/scaleW/3, adjustedSize.height()/1.5,d->leftBush);
  }
  else
  {

    //drawing the Clouds
    painter.drawPixmap(xCloud/6+adjustedSize.width()/2,  (-adjustedSize.height()/4), adjustedSize.width()/2, adjustedSize.height()/1.5,d->cloud);
    painter.drawPixmap(xCloud/4+adjustedSize.width()/2,  (-adjustedSize.height()/8), adjustedSize.width()/2, adjustedSize.height()/1.5,d->cloud);

    //drawing the mountain
    painter.drawPixmap(0, ((adjustedSize.height()/2.2/mountY +adjustedSize.height()/10)+600-d->animationStage2*3)+d->h -d->animationStage3, adjustedSize.width(), adjustedSize.height()/2.9,d->mountain);

    //drawing the Clouds
    painter.drawPixmap(xCloud/3+adjustedSize.width()/2,  (0), adjustedSize.width()/2, adjustedSize.height()/1.5,d->cloud);




    //drawing Left bush
    painter.drawPixmap(-adjustedSize.width()/10+d->h-d->animationStage3, ((adjustedSize.height()/3.2/hillY +adjustedSize.height()/10)-d->animationStage2)+d->h*2 -d->animationStage3*2, adjustedSize.width()/scaleH/1.5/d->h*d->animationStage3, adjustedSize.height()/1.2/d->h*d->animationStage3,d->leftBush);
  }


    painter.drawPixmap(0, (adjustedSize.height()/3/hillY)-d->animationStage2, adjustedSize.width()*1.5, adjustedSize.height()*1.5,d->backgroundHill);


    if( rect().width()> rect().height())
    {

      //rightBush
      painter.drawPixmap(adjustedSize.width()/2.4 +d->h-d->animationStage3, ((adjustedSize.height()/2.37/rhillY +adjustedSize.height()/3.8)-d->animationStage2)+d->h*2-d->animationStage3*2, adjustedSize.width()/2.4/d->h*d->animationStage3, adjustedSize.height()/1.5/d->h*d->animationStage3,d->rightBush);
    }
    else
    {
      //rightBush
      painter.drawPixmap(adjustedSize.width()/4+d->h-d->animationStage3, ((adjustedSize.height()/2.8/hillY +adjustedSize.height()/4.8)-d->animationStage2)+d->h*2-d->animationStage3*2, adjustedSize.width()/1.2/d->h*d->animationStage3, adjustedSize.height()/1.2/d->h*d->animationStage3,d->rightBush);
    }




  painter.drawPixmap(0,  (adjustedSize.height()/3/hillY)-d->animationStage, adjustedSize.width(), adjustedSize.height()*1.5,d->kineticPix);

painter.end();
//swapBuffers();
}



//##################################################################################################
void Skin::resizeEvent(QResizeEvent* event)
{
  Q_UNUSED(event)
 // d->update();
 // QWidget::resizeEvent(event);
}

//##################################################################################################
void Skin::mousePressEvent(QMouseEvent* event)
{
 Q_UNUSED(event);
 d->clicked = true;

 d->update();
}

//##################################################################################################
void Skin::mouseMoveEvent(QMouseEvent* event)
{
 Q_UNUSED(event)
}

//##################################################################################################
void Skin::mouseReleaseEvent(QMouseEvent* event)
{
  Q_UNUSED(event);





  if(d->clicked)
  {
    d->clicked = false;

    if(rect().contains(event->pos()))
    {
      d->animationStage=0.0f;
      d->animation->stop();
      d->animation->start();

      d->animationStage2=0.0f;
      d->animation2->stop();
      d->animation2->start();

      d->animationStage3=0.0f;
      d->animation3->stop();
      d->animation3->start();

      d->animationStage4=0.0f;
      d->animation4->stop();
      d->animation4->start();
      emit clicked();
    }

  }

}

//##################################################################################################





void Skin::showEvent(QShowEvent* event)
{
  Q_UNUSED(event)
  QTimer::singleShot(1000, this, SLOT(animationActivate()));
      d->update();

// QTimer::singleShot(6, this, SLOT(animationActivate()));
//animationActivate();

}
void Skin::animationActivate()
{




  if(!d->clicked)
  {
    d->clicked = false;

  //  if(rect().contains(event->pos()))
    {

      d->animationStage=0.0f;
      d->animation->stop();
      d->animation->start();

      d->animationStage2=0.0f;
      d->animation2->stop();
      d->animation2->start();

      d->animationStage3=0.0f;
      d->animation3->stop();
      d->animation3->start();

      d->animationStage4=0.0f;
      d->animation4->stop();
      d->animation4->start();
      emit clicked();
    }

}




}

}
