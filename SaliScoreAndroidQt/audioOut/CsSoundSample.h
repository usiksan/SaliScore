/*
  Project "SaliScore Score music edit, view and tutorial programm"

  Author
    Sibilev Alexander S.
  Web
    www.saliLab.com
    www.saliLab.ru

  Description
    Single sample of PCM sound
*/
#ifndef CSSOUNDSAMPLE_H
#define CSSOUNDSAMPLE_H

#include "config.h"

struct CsSoundSample {
    int mLeft;  //!< Left channel
    int mRight; //!< Right channel

    CsSoundSample() : mLeft(0), mRight(0) {}

    void operator += ( const CsSoundSample &sample ) { mLeft += sample.mLeft; mRight += sample.mRight; }
  };

#endif // CSSOUNDSAMPLE_H
