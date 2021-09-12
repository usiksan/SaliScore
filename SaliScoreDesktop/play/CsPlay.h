#ifndef CSPLAY_H
#define CSPLAY_H

#include "score/CsComposition.h"

class CsPlay
  {
    CsComposition &mComposition;
    int            mTickIndex;
    int            mLineIndex;
    int            mTickLineStart;
    int            mTickLineStop;
    int            mTickCount;
    bool           mShow;
  public:
    CsPlay( CsComposition &comp );

    int  lineIndex() const { return mLineIndex; }

    int  lineTickIndex() const { return mTickIndex - mTickLineStart; }

    int  lineTickElapsed() const { if( auto tickCount = (mTickLineStop - mTickLineStart) ) return lineTickIndex() * 100 / tickCount; return 100; }

    bool isHit( int position, int duration ) const;

    void next( int tick );

    void jump( int lineIndex, int position );

    void reset();

    bool isShow() const { return mShow; }

    void show( bool sh ) { mShow = sh; }
  };

#endif // CSPLAY_H
