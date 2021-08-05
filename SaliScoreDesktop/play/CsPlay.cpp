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

bool CsPlay::isHit(int position, int duration) const
  {
  int tick = lineTickIndex();
  return position <= tick && tick < (position + duration);
  }



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
        mTickLineStop = mTickLineStart + line.taktCount() * 256;
        return;
        }
      }
    //Next line not found. Song is finished
    mTickIndex = mTickLineStop;
    }
  }



void CsPlay::reset()
  {
  bool isFirst = true;
  mTickCount = mTickIndex = 0;
  for( int index = 0; index < mComposition.lineCount(); index++ ) {
    const auto &line = mComposition.line( index );
    if( !line.isRemark() ) {
      mTickCount += line.taktCount() * 256;

      if( isFirst ) {
        mLineIndex = index;
        mTickLineStart = 0;
        mTickLineStop = line.taktCount() * 256;
        isFirst = false;
        }
      }
    }
  }
