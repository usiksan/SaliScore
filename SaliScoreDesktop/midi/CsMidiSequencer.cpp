#include "CsMidiSequencer.h"

#include <QDebug>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

CsMidiSequencer::CsMidiSequencer(QThread *th, QObject *parent) :
  QObject(parent),
  mPeriodic(nullptr), //!< Timer for polling midi input and generate soft tick sequence
  mTickStep(12800),    //!< Tick step in nanoSec, default 120bpm
  mTickCount(0),      //!< Current tick in nanoSec. When count over 10ms sended one or more ticks
  mMidiHandle(-1),    //!< Handle to midi keyboard
  mDataIndex(-1),
  mRun(false)
  {
  moveToThread( th );
  connect( th, &QThread::started, this, &CsMidiSequencer::onStart );
  }




CsMidiSequencer::~CsMidiSequencer()
  {
  if( mMidiHandle >= 0 )
    close( mMidiHandle );
  }

void CsMidiSequencer::setRun(bool on)
  {
  mRun = on;
  mTickCount = 0;
  }

void CsMidiSequencer::setTempo(int tempo)
  {

  }




void CsMidiSequencer::periodic()
  {
  if( mMidiHandle >= 0 ) {
    //There is opened midi device. Parse incomming messages
    char buf[30];
    ssize_t r;
    do {
      //Read buffer
      r = read( mMidiHandle, buf, 30 );

      //Parse bytes
      for( int i = 0; i < r; ++i ) {
        if( buf[i] & 0x80 ) {
          //Control byte i
          if( mDataIndex >= 0 ) {
            midiSignal( mControl, mData0, 0 );
            //qDebug() << "before control midi" << mControl << mData0;
            }
          mControl = buf[i] & 0x7f;
          mDataIndex = 0;
          mData0 = 0;
          }
        else {
          //Data byte
          if( mDataIndex == 0 ) {
            mData0 = buf[i] & 0x7f;
            mDataIndex = 1;
            }
          else if( mDataIndex == 1 ) {
            midiSignal( mControl, mData0, buf[i] & 0x7f );
            //qDebug() << "normal midi" << mControl << mData0 << static_cast<int>(buf[i] & 0x7f);
            mDataIndex = -1;
            }
          }
        }
      }
    while( r == 30 );
    if( mDataIndex >= 0 ) {
      midiSignal( mControl, mData0, 0 );
      //qDebug() << "post midi" << mControl << mData0;
      }
    }
  else {
    //No midi device. Make soft tick signal
    mTickCount += mTickStep;
    int tc = mTickCount / 10000;
    mTickCount %= 10000;
    if( tc > 0 )
      tickGenerate( tc );
    }
  }



void CsMidiSequencer::onStart()
  {
  if( mPeriodic == nullptr ) {
    mPeriodic = new QTimer();
    mPeriodic->setInterval(10);
    connect( mPeriodic, &QTimer::timeout, this, &CsMidiSequencer::periodic );
    mPeriodic->start();
    }
  mMidiHandle = open( "/dev/snd/midiC1D0", O_RDONLY | O_NONBLOCK );
  if( mMidiHandle >= 0 ) {
    emit midiLink( true );
    //qDebug() << "midi open" << mMidiHandle;
    }
  else {
    //Try connect after one second
    QTimer::singleShot( 1000, this, &CsMidiSequencer::onStart );
    }
  }



void CsMidiSequencer::midiSignal(quint8 control, quint8 data0, quint8 data1)
  {
  qDebug() << "Midi " << control << data0 << data1;
  }



void CsMidiSequencer::tickGenerate(int count)
  {
  if( mRun )
    emit tick( count );
  }
