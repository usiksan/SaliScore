#include "MainWindow.h"

#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#include <QtAndroid>
#endif


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  {
  QString str("1");
#ifdef Q_OS_ANDROID
  if(QAndroidJniObject::isClassAvailable("CsMidiAndroid")) {
    QAndroidJniObject someJavaObject = QAndroidJniObject("CsMidiAndroid","(Landroid/content/Context;)V", QtAndroid::androidContext().object() );
    str = someJavaObject.callObjectMethod<jstring>("sayHello").toString();
    }
  else {
    str = QStringLiteral("SOME JAVA CLASS UNAVAIABLE!");
    }
#endif

  setCentralWidget( new QLabel( str )  );
  }

MainWindow::~MainWindow()
  {
  }

