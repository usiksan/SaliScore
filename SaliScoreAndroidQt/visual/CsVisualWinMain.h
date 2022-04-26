#ifndef CSVISUALWINMAIN_H
#define CSVISUALWINMAIN_H

#include "score/CsComposition.h"

#include <QMainWindow>
#include <QObject>
#include <QSet>


using CsKeyEvent = int;

inline CsKeyEvent csKeyEvent( int channel, int note ) { return (channel << 16) | note; }

struct CsPlayerEvent
  {
    int mChannel;    //!< Channel to which need to be sended this event
    int mNote;       //!< Note of event
    int mTickRemain; //!< Count of ticks remain to end of event

    CsPlayerEvent() {}
    CsPlayerEvent( int channel, int note, int duration ) : mChannel(channel), mNote(note), mTickRemain(duration) {}

    bool tick( int tick )
      {
      mTickRemain -= tick;
      return mTickRemain <= 0;
      }

    CsKeyEvent keyEvent() const { return csKeyEvent( mChannel, mNote ); }
  };


using CsPlayerEventList = QList<CsPlayerEvent>;

using CsKeyEventSet = QSet<CsKeyEvent>;


class CsVisualWinMain : public QMainWindow
  {
    Q_OBJECT
    int           mFragmentIndex; //!< Current train fragment index
    int           mLineIndex;     //!< Current train line index
    int           mLinePosition;  //!< Current position in line
    int           mLineTickCount;
    bool          mIsRun;         //!< If true then player in run state
    bool          mIsPaused;      //!< If true then player paused

    CsPlayerEventList mEventList;
    CsKeyEventSet     mKeyEventFromKeyboard; //!< Events of keyboard
    CsKeyEventSet     mKeyEventFromPlayer;   //!< Event waiting from player

  protected:
    CsComposition mComposition;   //!< Current composition

    //Player part
  public:
    explicit CsVisualWinMain(QWidget *parent = nullptr);

  signals:

  private:
    void extractChords(const CsLine &line, const CsDefinition &def, int lastPosition);
  };

#endif // CSVISUALWINMAIN_H
