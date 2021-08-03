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
    bool           mShow;
  public:
    CsPlay( CsComposition &comp );

    int  lineIndex() const { return mLineIndex; }

    int  lineTickIndex() const { return mTickIndex - mTickLineStart; }

    bool isHit( int position, int duration ) const;

    void next( int tick );

    bool isShow() const { return mShow; }
    void show( bool sh ) { mShow = sh; }
  };

#endif // CSPLAY_H
