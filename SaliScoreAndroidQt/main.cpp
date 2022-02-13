#include "config.h"
#include "MainWindow.h"
#include "audioOut/CsSoundBufferIODevice.h"
#include "synth/CsSynthMeandr.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
  {
  QApplication a(argc, argv);
  MainWindow w;

  //=============================================================================
  //        Audio output setup
  if( CsSoundBufferIODevice::soundBuffer() == nullptr )
    return 0;
  CsSoundBufferIODevice::soundBuffer()->addSound( new CsSynthMeandr() );
  qDebug() << "Audio supported";

  w.show();
  return a.exec();
  }
