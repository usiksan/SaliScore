#ifndef CSPLAYER_H
#define CSPLAYER_H

#include "CsPlayerEvent.h"

#include <QObject>
#include <QSet>

class CsComposition;
class CsLine;
class CsDefinition;

using CsKeyEventSet = QSet<CsKeyEvent>;

class CsPlayer : public QObject
  {
    Q_OBJECT

    CsComposition &mComposition;
    int           mFragmentIndex; //!< Current train fragment index
    int           mLineIndex;     //!< Current train line index
    int           mLinePosition;  //!< Current position in line
    int           mLineTickCount;
    bool          mIsRun;         //!< If true then player in run state
    bool          mIsPaused;      //!< If true then player paused

    CsPlayerEventList mEventList;
    CsKeyEventSet     mKeyEventFromKeyboard; //!< Events of keyboard
    CsKeyEventSet     mKeyEventFromPlayer;   //!< Event waiting from player
  public:
    explicit CsPlayer(QObject *parent = nullptr);

  signals:

  public slots:
    void playStart();
    void playPause();
    void playStop();

  private slots:
    void tickNext( int tickAddon );

  private:
    void extractChords( const CsLine &line, const CsDefinition &def, int lastPosition );
    //void extr
  };

#endif // CSPLAYER_H
