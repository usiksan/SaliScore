#ifndef CSSYNTHMEANDR_H
#define CSSYNTHMEANDR_H

#include "audioOut/CsSoundSource.h"

class CsSynthMeandr : public CsSoundSource
  {
  public:
    CsSynthMeandr();

    // CsSoundSource interface
  public:
    virtual CsSoundSample sample() override;
    virtual bool isStopped() const override;
  };

#endif // CSSYNTHMEANDR_H
