/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "composition.h"
#include "TargetCircleWidget.h"
#include "Skin.h"
#include <QPixmap>
#include <QtWidgets>
#include <QtCore/qstate.h>

#include <QApplication>
#ifdef QT_OPENGL_SUPPORT
#include <QtOpenGL>
#endif













class Pixmap : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    Pixmap(const QPixmap &pix)
        : QObject(), QGraphicsPixmapItem(pix)
    {
        setCacheMode(DeviceCoordinateCache);
    }
};
class View : public QGraphicsView
{
public:
    View(QGraphicsScene *scene) : QGraphicsView(scene) { }

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE
    {
        QGraphicsView::resizeEvent(event);
        fitInView(sceneRect(), Qt::KeepAspectRatio);
    }
};
int main(int argc, char *argv[])
{
  //Q_INIT_RESOURCE(animatedtiles);

  QApplication app(argc, argv);

//  QPixmap kineticPix(":/res/images/foreground.png");
//  QPixmap bgPix(":/images/Time-For-Lunch-2.jpg");
////  kinetic
////kineticPix=kineticPix.scaled(1000,600,Qt::KeepAspectRatio, Qt::SmoothTransformation);
//  QGraphicsScene scene(0,0, 700, 700);

//  QList<Pixmap *> items;
//  for (int i = 0; i < 1; ++i) {
//      Pixmap *item = new Pixmap(kineticPix);
//      item->setOffset(-10,500);
//      item->setZValue(i);
//      items << item;
//      scene.addItem(item);
//  }


//  QGraphicsItem *buttonParent = new QGraphicsRectItem;


//  scene.addItem(buttonParent);
//  buttonParent->setTransform(QTransform::fromScale(0.75, 0.75), true);
//  buttonParent->setPos(200, 200);
//  buttonParent->setZValue(65);

//  // States
//  QState *rootState = new QState;
//  QState *ellipseState = new QState(rootState);
//  QState *figure8State = new QState(rootState);
//  QState *randomState = new QState(rootState);
//  QState *tiledState = new QState(rootState);
//  QState *centeredState = new QState(rootState);

//  // Values
//  for (int i = 0; i < items.count(); ++i) {
//      Pixmap *item = items.at(i);
//      // Ellipse
//      ellipseState->assignProperty(item, "pos",
//                                       QPointF(0, sin((i / 63.0) * 6.28) * 250));

//      // Figure 8
//      figure8State->assignProperty(item, "pos",
//                                       QPointF(sin((i / 63.0) * 6.28) * 250,
//                                               sin(((i * 2)/63.0) * 6.28) * 250));

//      // Random
//      randomState->assignProperty(item, "pos",
//                                      QPointF(-250 + qrand() % 500,
//                                              -250 + qrand() % 500));

//      // Tiled
//      tiledState->assignProperty(item, "pos",
//                                     QPointF(0,((i / 8) - 4) * 120 + 180/2));

//      // Centered
//      centeredState->assignProperty(item, "pos", QPointF());
//  }

//  // Ui
//  View *view = new View(&scene);
//  view->setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Animated Tiles"));
//  view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
//  view->setBackgroundBrush(bgPix);
//  view->setCacheMode(QGraphicsView::CacheBackground);
//  view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
//  //view->show();

//  QStateMachine states;
//  states.addState(rootState);
//  states.setInitialState(rootState);
//  rootState->setInitialState(centeredState);

//  QParallelAnimationGroup *group = new QParallelAnimationGroup;
//  for (int i = 0; i < items.count(); ++i) {
//      QPropertyAnimation *anim = new QPropertyAnimation(items[i], "pos");
//      anim->setDuration(1750 + i * 25);//SPEED!!!
//      anim->setEasingCurve(QEasingCurve::InOutBack);
//      group->addAnimation(anim);
//  }
////   QAbstractTransition *trans = rootState->addTransition(ellipseButton, SIGNAL(pressed()), ellipseState);
// // trans->addAnimation(group);

////    trans = rootState->addTransition(figure8Button, SIGNAL(pressed()), figure8State);
////    trans->addAnimation(group);

////    trans = rootState->addTransition(randomButton, SIGNAL(pressed()), randomState);
////    trans->addAnimation(group);

////    trans = rootState->addTransition(tiledButton, SIGNAL(pressed()), tiledState);
////    trans->addAnimation(group);

////    trans = rootState->addTransition(centeredButton, SIGNAL(pressed()), centeredState);
////    trans->addAnimation(group);

//  QTimer timer;
//  timer.start(125);
//  timer.setSingleShot(true);
//   //QAbstractTransition *trans = rootState->addTransition(&timer, SIGNAL(pressed()), ellipseState);
// QAbstractTransition *trans = rootState->addTransition(&timer, SIGNAL(timeout()), tiledState);
//  trans->addAnimation(group);

//  states.start();
  QGLWidget *window = new QGLWidget;
  //QTimer::singleShot(1000, window, SLOT(showFullScreen()));
//window->resize(QApplication::desktop()->availableGeometry().size());
  window->resize(400,600);

 // cad_mobile_application::MainWindow w;
//cad_mobile_application::Skin skin;
  QGridLayout *layout = new QGridLayout;
//w.setStyleSheet("* { background-color: rgb(255,255,255,0); }");



//  layout->addWidget(view, 0, 0);
  //layout->addWidget(&w, 0, 0);
 cad_mobile_application::Skin skin;
 //

  window->setStyleSheet("* { background-color: rgb(234, 252, 254); }");


layout->setContentsMargins(0,0,0,0);


 layout->addWidget(&skin, 0, 0);
//layout->addWidget(button, 0, 0,Qt::AlignCenter);

 window->setLayout(layout);
     window->show();



  return app.exec();
}
#include "main.moc"
