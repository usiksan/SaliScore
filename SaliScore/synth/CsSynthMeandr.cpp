#include "CsSynthMeandr.h"

CsSynthMeandr::CsSynthMeandr()
  {

  }


CsSoundSample CsSynthMeandr::sample()
  {
  static int count;
  count++;
  CsSoundSample sample;
  if( count < 44 ) {
    sample.mLeft = sample.mRight = 1000;
    }
  else {
    sample.mLeft = sample.mRight = -1000;
    if( count > 84 ) count = 0;
    }
  return sample;
  }


bool CsSynthMeandr::isStopped() const
  {
  return false;
  }
