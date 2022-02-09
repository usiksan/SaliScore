#include "MainWindow.h"

#include <QLabel>
#include <QVBoxLayout>

#ifdef Q_OS_LINUX
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#endif

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  {
//  QVBoxLayout *lay = new QVBoxLayout();
//  lay->addWidget( new QLabel(QStringLiteral("Primer. Hello.") ) );
//  setLayout( lay );
#ifdef Q_OS_LINUX
  int mMidiHandle = open( "/dev/snd/midiC1D0", O_RDONLY | O_NONBLOCK );
  QString str;
  if( mMidiHandle >= 0 ) {
    str = QStringLiteral("Open successfull");
    }
  else {
    str = QStringLiteral("Not open");
    }
#endif

  setCentralWidget( new QLabel( str )  );
  }

MainWindow::~MainWindow()
  {
  }

