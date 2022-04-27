#ifndef CSMIDIKEYBOARDDESKTOP_H
#define CSMIDIKEYBOARDDESKTOP_H

#include <QObject>
#include <QTimer>

class CsMidiKeyboardDesktop : public QObject
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
  public:
    explicit CsMidiKeyboardDesktop(QObject *parent = nullptr);

    bool isLink() const { return mMidiHandle >= 0; }

    void setTempo( int tempo );

  signals:
    void tick( int count );

    void midiNote( int note, int velo );

    void midiRun( bool run );

    void midiLink( bool on );

  public slots:
    void playRun( bool run );
    void playNote( int note, int velo );
    void playTempo( int tempo );
    void playVoice( int voice );

  private slots:
    void periodic();

    void onStart();

    void midiSignal( quint8 control, quint8 data0, quint8 data1 );
  };

#endif // CSMIDIKEYBOARDDESKTOP_H
