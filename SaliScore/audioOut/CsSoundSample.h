/*
  Project "SaliScore Score music edit, view and tutorial program"

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

#include "CsConfig.h"

struct CsSoundSample {
    int mLeft;  //!< Left channel
    int mRight; //!< Right channel

    CsSoundSample() : mLeft(0), mRight(0) {}

    void addLeft( int sample ) { mLeft += sample; }
    void addRight( int sample ) { mRight += sample; }
    void addMono( int sample ) { mLeft += sample; mRight += sample; }
    void add( int type, int sample ) {
      if( type != 4 ) mRight += sample;
      if( type != 2 ) mLeft += sample;
      }

    void operator += ( const CsSoundSample &sample ) { mLeft += sample.mLeft; mRight += sample.mRight; }
  };

#endif // CSSOUNDSAMPLE_H
