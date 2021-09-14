#include "CsMidiSequencer.h"

#include <QDebug>
#ifdef Q_OS_LINUX
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#endif

CsMidiSequencer::CsMidiSequencer(QThread *th, QObject *parent) :
  QObject(parent),
  mPeriodic(nullptr), //!< Timer for polling midi input and generate soft tick sequence
  mTickStep(12800),    //!< Tick step in nanoSec, default 120bpm
  mTickCount(0),      //!< Current tick in nanoSec. When count over 10ms sended one or more ticks
  mMidiHandle(-1),    //!< Handle to midi keyboard
  mDataIndex(-1),
  mSysExIndex(-1),
  mRun(false)
  {
  moveToThread( th );
  connect( th, &QThread::started, this, &CsMidiSequencer::onStart );
  }




CsMidiSequencer::~CsMidiSequencer()
  {
#ifdef Q_OS_LINUX
  if( mMidiHandle >= 0 )
    close( mMidiHandle );
#endif
  }

void CsMidiSequencer::setRun(bool on)
  {
  mRun = on;
  mTickCount = 0;
  }

void CsMidiSequencer::setTempo(int tempo)
  {

  }




void CsMidiSequencer::midiSend(int count, quint8 *array)
  {
  if( mMidiHandle >= 0 ) {
#ifdef Q_OS_LINUX
    write( mMidiHandle, array, count );
#endif
    }
  }




void CsMidiSequencer::periodic()
  {
  if( mMidiHandle >= 0 ) {
#ifdef Q_OS_LINUX
    //There is opened midi device. Parse incomming messages
    char buf[33];
    ssize_t r;
    do {
      //Read buffer
      r = read( mMidiHandle, buf, 30 );

      //Parse bytes
      for( int i = 0; i < r; ++i ) {
        if( buf[i] & 0x80 ) {
          //Check system command
          int system = buf[i] & 0x7f;
          if( system == 0x78 ) {
            //Timing clock
            tickGenerate(26666);
            continue;
            }
          if( system == 0x7e )
            //Active sensing
            continue;
          if( system == 0x7a ) {
            emit midiStart();
            continue;
            }
          if( system == 0x7c ) {
            emit midiStop();
            continue;
            }
          if( system == 0x70 ) {
            //Begin of sysEx message
            qDebug() << "Begin sysEx";
            mSysExIndex = 0;
            continue;
            }
          if( system == 0x77 ) {
            //End of sysEx message
            qDebug() << "End sysEx";
            parseSysEx();
            mSysExIndex = -1;
            continue;
            }
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
          if( mSysExIndex >= 0 ) {
            //This byte for sysEx
            mSysExBuf[mSysExIndex++] = buf[i] & 0x7f;
            if( mSysExIndex >= 1024 ) mSysExIndex = 1023;
            continue;
            }
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
#endif
    }
  else {
    //No midi device. Make soft tick signal
    tickGenerate( mTickStep );
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
#ifdef Q_OS_LINUX
  mMidiHandle = open( "/dev/snd/midiC1D0", O_RDONLY | O_NONBLOCK );
  if( mMidiHandle >= 0 ) {
    emit midiLink( true );
    //qDebug() << "midi open" << mMidiHandle;
    }
  else {
    //Try connect after one second
    QTimer::singleShot( 1000, this, &CsMidiSequencer::onStart );
    }
#endif
  }



void CsMidiSequencer::midiSignal(quint8 control, quint8 data0, quint8 data1)
  {
  qDebug() << "Midi " << control << data0 << data1;
  }



void CsMidiSequencer::tickGenerate(int count)
  {
  mTickCount += count;
  int tc = mTickCount / 10000;
  mTickCount %= 10000;
  if( tc > 0 && mRun )
    emit tick( tc );
  }




void CsMidiSequencer::parseSysEx()
  {
  qDebug() << "====SysEx:" << mSysExIndex;
  for( int i = 0; i < mSysExIndex; i++ )
    qDebug() << mSysExBuf[i];
  qDebug() << "----";
  }
