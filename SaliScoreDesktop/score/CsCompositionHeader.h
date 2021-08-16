#ifndef CSCOMPOSITIONHEADER_H
#define CSCOMPOSITIONHEADER_H

#include "CsDescrSong.h"
#include "CsSynthSettings.h"

class CsCompositionHeader : public CsDescrSong
  {
  public:
    CsSynthSettings mSettings;
  public:
    CsCompositionHeader();

    //=================================================================
    //         Settings JSON io

    virtual void jsonWrite( CsJsonWriter &js ) const override;

    virtual void jsonRead( CsJsonReader &js ) override;
  };

#endif // CSCOMPOSITIONHEADER_H
