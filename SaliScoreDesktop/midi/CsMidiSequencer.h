#ifndef CSMIDISEQUENCER_H
#define CSMIDISEQUENCER_H

#include <QObject>
#include <QTimer>
#include <QThread>

class CsMidiSequencer : public QObject
  {
    Q_OBJECT

    QTimer *mPeriodic;   //!< Timer for polling midi input and generate soft tick sequence
    int     mTickStep;   //!< Tick step in nanoSec
    int     mTickCount;  //!< Current tick in nanoSec. When count over 10ms sended one or more ticks

    int     mMidiHandle; //!< Handle to midi keyboard
    int     mDataIndex;

    //Midi message
    quint8  mControl;
    quint8  mData0;

    quint8  mSysExBuf[1024];
    int     mSysExIndex;

    bool    mRun;
  public:
    explicit CsMidiSequencer( QThread *th, QObject *parent = nullptr);
    ~CsMidiSequencer();

    bool isLink() const { return mMidiHandle >= 0; }

    bool isRun() const { return mRun; }
    void setRun( bool on );

    void setTempo( int tempo );

    void midiSend( int count, quint8 *array );


  signals:
    void tick( int count );

    void midiStart();

    void midiStop();

    void midiLink( bool on );

  private:
    void periodic();

    void onStart();

    void midiSignal( quint8 control, quint8 data0, quint8 data1 );

    void tickGenerate( int count );

    void parseSysEx();
  };

#endif // CSMIDISEQUENCER_H
