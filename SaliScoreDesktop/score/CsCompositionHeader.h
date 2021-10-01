#ifndef CSCOMPOSITIONHEADER_H
#define CSCOMPOSITIONHEADER_H

#include "CsDescrSong.h"
#include "CsSynthSettings.h"
#include "CsTrainInterval.h"

class CsCompositionHeader : public CsDescrSong
  {
  public:
    CsSynthSettings mSettings;

    CsTrainInterval mFragments[2];
  public:
    CsCompositionHeader();

    virtual void clear() override;

    //=================================================================
    //         Settings JSON io

    virtual void jsonWrite( CsJsonWriter &js ) const override;

    virtual void jsonRead( CsJsonReader &js ) override;
  };

#endif // CSCOMPOSITIONHEADER_H
