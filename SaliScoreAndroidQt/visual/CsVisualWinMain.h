/*
  Project "SaliScore Score music edit, view and tutorial program"

  Author
    Sibilev Alexander S.
  Web
    www.saliLab.com
    www.saliLab.ru

  Description
    CsVisualWinMain - base window for main window of application

    In base window playing functionality

    One tick is 1/256 part of hole note

    One takt is one hole note

    One takt is 4 beats

    Playback tempo: main timer is 20ms, with this period changes mTickCount by mTickStep.
    Each step 20/16ms.
    bpm - beats per minute
    time of one beat is 60 / bpm
    time of takt is 4 * 60 / bpm
    time of tick is 4 * 60 / bpm / 256
    time of tick in us 4 * 60000000 / bpm / 256

*/
#ifndef CSVISUALWINMAIN_H
#define CSVISUALWINMAIN_H

#include "CsConfig.h"
#include "score/CsComposition.h"

#include <QMainWindow>
#include <QObject>
#include <QSet>
#include <QTimer>


using CsKeyEvent = int;

inline CsKeyEvent csKeyEvent( int channel, int note ) { return (channel << 16) | note; }
inline int        csKeyEventChannel( CsKeyEvent ev ) { return (ev >> 16) & 0xff; }
inline int        csKeyEventPitch( CsKeyEvent ev ) { return ev & 0xffff; }

class CsPlayerEvent
  {
    int mChannel;    //!< Channel to which need to be sended this event
    int mPitch;      //!< Note of event
    int mTickRemain; //!< Count of ticks remain to end of event
  public:

    CsPlayerEvent() {}
    CsPlayerEvent( int channel, int pitch, int duration ) : mChannel(channel), mPitch(pitch), mTickRemain(duration) {}

    int channel() const { return mChannel; }

    int pitch() const { return mPitch; }

    bool tick( int tick )
      {
      mTickRemain -= tick;
      return mTickRemain <= 0;
      }

    CsKeyEvent keyEvent() const { return csKeyEvent( mChannel, mPitch ); }
  };


using CsPlayerEventList = QList<CsPlayerEvent>;

using CsKeyEventSet = QSet<CsKeyEvent>;


class CsVisualWinMain : public QMainWindow
  {
    Q_OBJECT

    QTimer            mTimer;                //!< Timer for playback composition
    qint32            mTickCount;     //! Current time tick value
    qint32            mTickStep;      //! Current time tick step for playback

    int               mTrainIndex;    //!< Current train fragment index
    int               mLineIndex;     //!< Current train line index
    int               mLinePosition;  //!< Current position in line
    int               mLineTickCount;
    bool              mIsRun;         //!< If true then player in run state
    bool              mIsPaused;      //!< If true then player paused
    bool              mIsTraining;    //!< true if teaching process actived

    CsPlayerEventList mEventList;
    CsPlayerEventList mWaitingList;
    CsKeyEventSet     mKeyEventFromKeyboard; //!< Events of keyboard
    CsKeyEventSet     mKeyEventFromPlayer;   //!< Event waiting from player

  protected:
    CsComposition mComposition;   //!< Current composition

    //Player part
  public:
    explicit CsVisualWinMain(QWidget *parent = nullptr);

    int playLineIndex() const { return mLineIndex; }

    int playLinePosition() const { return mLinePosition; }

    bool playIsRun() const { return mIsRun; }

    void setTempo( int tempo );

    virtual void playUpdate() = 0;

  signals:
    void playNote( int channel, int pitch, int velo );
    void playHighlight( int channel, int pitch, int velo );
    void playRun( bool run );
    void playPause( bool pause );
    void playVoice( int channel, int voice );

  public slots:
    void cmPlayRun();
    void cmPlayTrain();
    void cmPlayPause();
    void cmPlayStop();

    void midiNote( int pitch, int velo );
    void midiRun( bool run );
    void midiVoice( int voice );

  private slots:

    void periodic();

  private:
    void tick( int tickOffset );
    void findNextLine();
    void prepareRun();
    void extractLine( int lastPosition );
    void extractChords( const CsLine &line, const CsDefinition &def, int lastPosition );
    void extractNote( const CsLine &line, const CsDefinition &def, int lastPosition );
  };

#endif // CSVISUALWINMAIN_H
