#ifndef test_h
#define test_h


#include <QFrame>
#include <QIcon>
#include <QGLWidget>
namespace htl_qt_widgets
{

//##################################################################################################
class  test : public QGLWidget
{
  Q_OBJECT
  //Q_PROPERTY(QString colorSeries          READ colorSeries           WRITE setColorSeries          RESET unsetColorSeries)

protected:
  //################################################################################################
  virtual void paintEvent(QPaintEvent* event);


signals:
  //################################################################################################
  void targetChanged();

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
