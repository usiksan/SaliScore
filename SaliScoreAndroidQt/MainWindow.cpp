#include "MainWindow.h"

#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QtAndroidExtras>
#include <QtAndroid>


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  {
  QString str("1");
  if(QAndroidJniObject::isClassAvailable("CsMidiAndroid")) {
    QAndroidJniObject someJavaObject = QAndroidJniObject("CsMidiAndroid","(Landroid/content/Context;)V", QtAndroid::androidContext().object() );
    str = someJavaObject.callObjectMethod<jstring>("sayHello").toString();
    }
  else {
    str = QStringLiteral("SOME JAVA CLASS UNAVAIABLE!");
    }

  setCentralWidget( new QLabel( str )  );
  }

MainWindow::~MainWindow()
  {
  }

