#ifndef CSPLAY_H
#define CSPLAY_H

#include "score/CsComposition.h"

class CsPlay
  {
    CsComposition &mComposition;
    int            mTickIndex;
    int            mLineIndex;
  public:
    CsPlay();

    int  lineIndex() const { return mLineIndex; }

    int  lineTickIndex() const;

    void next( int tick );
  };

#endif // CSPLAY_H
