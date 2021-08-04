#include "CsPlay.h"

CsPlay::CsPlay(CsComposition &comp) :
  mComposition(comp),
  mTickIndex(0),
  mLineIndex(3),
  mTickLineStart(0),
  mTickLineStop(0)
  {

  }

bool CsPlay::isHit(int position, int duration) const
  {
  int tick = lineTickIndex();
  return position <= tick && tick < (position + duration);
  }



void CsPlay::reset()
  {
  bool isFirst = true;
  mTickCount = 0;
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
