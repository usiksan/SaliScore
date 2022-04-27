#ifndef CSMIDIKEYBOARDLINUX_H
#define CSMIDIKEYBOARDLINUX_H

#include "CsMidiKeyboard.h"

#include <QObject>
#include <QTimer>
#include <QThread>

#ifdef Q_OS_LINUX


class CsMidiKeyboardLinux : public CsMidiKeyboard
  {
    Q_OBJECT

    QThread *mThread;
    QTimer *mPeriodic;   //!< Timer for polling midi input and generate soft tick sequence
    int     mTickStep;   //!< Tick step in nanoSec
    int     mTickCount;  //!< Current tick in nanoSec. When count over 10ms sended one or more ticks

    int     mMidiHandle; //!< Handle to midi keyboard
    int     mQuietCount; //!< With this we monitor that mMidiHandle actualy open and work
    int     mDataIndex;

    //Midi message
    quint8  mControl;
    quint8  mData0;

    quint8  mSysExBuf[1024];
    int     mSysExIndex;
  public:
    explicit CsMidiKeyboardLinux(QObject *parent = nullptr);
    virtual ~CsMidiKeyboardLinux();

    virtual void init() override;

    virtual bool isLink() const override { return mMidiHandle >= 0; }

    virtual void setTempo( int tempo ) override;

  signals:

  public slots:
    virtual void playRun( bool run ) override;
    virtual void playNote( int note, int velo ) override;
    virtual void playTempo( int tempo ) override;
    virtual void playVoice( int voice ) override;

  private:
    void periodic();

    void onStart();

    void tickGenerate( int count );

    void parseSysEx();
  };

#endif

#endif // CSMIDIKEYBOARDLINUX_H
