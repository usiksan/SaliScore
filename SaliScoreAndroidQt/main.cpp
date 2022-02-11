#include "config.h"
#include "MainWindow.h"

#include <QApplication>

#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

#include <QDebug>


int main(int argc, char *argv[])
  {
  QApplication a(argc, argv);
  MainWindow w;

  //=============================================================================
  //        Audio output setup
  //Create audio device for output synthesed audio stream
  QAudioFormat format;
  // Set up the format, eg.
  format.setSampleRate(CS_SAMPLES_PER_SECOND);
  format.setChannelCount(2);
  format.setSampleSize(16);
  format.setCodec("audio/pcm");
  format.setByteOrder(QAudioFormat::LittleEndian);
  format.setSampleType(QAudioFormat::SignedInt);

  QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
  if( !info.isFormatSupported(format) ) {
    qWarning() << "Raw audio format not supported by backend, cannot play audio.";
    return 0;
    }
  else {
    qDebug() << "Audio supported";
    }

  w.show();
  return a.exec();
  }
