#include "CsPlay.h"

CsPlay::CsPlay(CsComposition &comp) :
  mComposition(comp),
  mTickIndex(0),
  mLineIndex(3),
  mTickLineStart(0),
  mTickLineStop(0),
  mTickCount(0),
  mShow(false)
  {

  }





//!
//! \brief isHit    Returns true if current play position is hit into given interval
//! \param position Start position of interval
//! \param duration Duration of interval
//! \return         true if current play position is hit into given interval
//!
bool CsPlay::isHit(int position, int duration) const
  {
  int tick = lineTickIndex();
  return position <= tick && tick < (position + duration);
  }







//!
//! \brief next Move play position to the next position by adding tick to current position
//! \param tick Count of tick added to current position
//!
void CsPlay::next(int tick)
  {
  mTickIndex += tick;
  if( mTickIndex >= mTickLineStop ) {
    //Find next line
    for( int index = mLineIndex + 1; index < mComposition.lineCount(); index++ ) {
      const auto &line = mComposition.line( index );
      if( !line.isRemark() ) {
        //Next song line found
        qDebug() << "CsPlay:new line" << index;
        mLineIndex = index;
        mTickLineStart = mTickLineStop;
        mTickLineStop = mTickLineStart + line.taktCount() * mComposition.tickPerTakt();
        return;
        }
      }
    //Next line not found. Song is finished
    mTickIndex = mTickLineStop;
    }
  }





//!
//! \brief jump      Jump to random position in score
//! \param lineIndex Line index of new position
//! \param position  Position inside of line
//!
void CsPlay::jump(int lineIndex, int position)
  {
  if( lineIndex < mComposition.lineCount() && !mComposition.line(lineIndex).isRemark() ) {
    //We begin always from compositions line start offset
    mTickLineStart = mComposition.lineStartOffset();

    //Scan all lines previous for lineIndex and calculate line start position
    for( int index = 0; index < lineIndex; index++ ) {
      const auto &line = mComposition.line( index );
      if( !line.isRemark() ) {
        //Song line found
        mTickLineStart += line.taktCount() * mComposition.tickPerTakt();
        }
      }
    mLineIndex = lineIndex;
    mTickLineStop = mTickLineStart + mComposition.line(lineIndex).taktCount() * mComposition.tickPerTakt();
    mTickIndex = mTickLineStart + position;
    }
  }





//!
//! \brief reset Resets player to initial state (begin of score and no show)
//!
void CsPlay::reset()
  {

  bool isFirst = true;
  mTickCount = mTickIndex = mComposition.lineStartOffset();
  //Calculate tick count of hole composition
  // and sametime find line with begin of composition
  for( int index = 0; index < mComposition.lineCount(); index++ ) {
    const auto &line = mComposition.line( index );
    if( !line.isRemark() ) {
      mTickCount += line.taktCount() * mComposition.tickPerTakt();

      if( isFirst ) {
        mLineIndex = index;
        mTickLineStart = 0;
        mTickLineStop = line.taktCount() * mComposition.tickPerTakt();
        isFirst = false;
        }
      }
    }
  }
