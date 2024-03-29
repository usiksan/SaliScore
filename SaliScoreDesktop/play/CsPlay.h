/*
Project "SaliScore Score music edit, view and tutorial programm"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Score player
*/
#ifndef CSPLAY_H
#define CSPLAY_H

#include "score/CsComposition.h"

class CsPlay
  {
    CsComposition  &mComposition;    //!< Composition on which player works
    int             mTickIndex;      //!< Current position of tick count
    int             mLineIndex;      //!< Index of current played line
    int             mTickLineStart;  //!< Tick position of current line start
    int             mTickLineStop;   //!< Tick position of current line end
    int             mTickCount;      //!< Tick count of hole composition
    bool            mShow;           //!< When true current play position is displayed
    bool            mRun;            //!< When true then played
    QString         mTrainPart;      //!< Note train part. If empty then no note train
    int             mTrainFragment;  //!< Train fragment index - 1
  public:
    CsPlay( CsComposition &comp );

    //!
    //! \brief lineIndex Return index of current line
    //! \return          Index of current line
    //!
    int  lineIndex() const { return mLineIndex; }

    //!
    //! \brief lineTickIndex Returns tick count from begin of current line
    //! \return              Tick count from begin of current line
    //!
    int  lineTickIndex() const { return mTickIndex - mTickLineStart; }

    //!
    //! \brief lineTickElapsed Return tick elapsend from begin of line in persent
    //! \return                Tick elapsed from begin of line in persent
    //!
    int  lineTickElapsed() const { if( auto tickCount = (mTickLineStop - mTickLineStart) ) return lineTickIndex() * 100 / tickCount; return 100; }

    //!
    //! \brief isHit    Returns true if current play position is hit into given interval
    //! \param position Start position of interval
    //! \param duration Duration of interval
    //! \return         true if current play position is hit into given interval
    //!
    bool isHit( int position, int duration ) const;

    //!
    //! \brief next Move play position to the next position by adding tick to current position
    //! \param tick Count of tick added to current position
    //!
    void next( int tick );

    //!
    //! \brief jump      Jump to random position in score
    //! \param lineIndex Line index of new position
    //! \param position  Position inside of line
    //!
    void jump( int lineIndex, int position );

    //!
    //! \brief reset Resets player to initial state (begin of score and no show)
    //!
    void reset();

    //!
    //! \brief isShow Returns current show state
    //! \return       Current show state
    //!
    //! mShow flag with mRun defines all possible states of player
    //! false           false - player stoped
    //! true            false - player paused
    //! true            true  - player runing
    bool isShow() const { return mShow; }

    //!
    //! \brief show Setup new state of show flag
    //! \param sh   New state of show flag
    //!
    void show( bool sh ) { mShow = sh; }

    //!
    //! \brief isRun Returns current run state
    //! \return      Current run state
    //!
    bool isRun() const { return mRun; }

    //!
    //! \brief train    Start a train process for part with partName
    //! \param partName Name of training part
    //!
    void train( const QString &partName );

    //!
    //! \brief start Run player from current state
    //!
    void start();

    //!
    //! \brief pause Puases player bun not hide it
    //!
    void pause();

    //!
    //! \brief stop Stops player and hide it
    //!
    void stop();

    //!
    //! \brief noteOn Called when user press any piano key
    //! \param note   Piano key midi code
    //!
    void noteOn( int note );

    void setTrainFragment( int fragmentIndex );
  };

#endif // CSPLAY_H
