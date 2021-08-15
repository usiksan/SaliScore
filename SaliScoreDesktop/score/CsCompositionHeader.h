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

  };

#endif // CSCOMPOSITIONHEADER_H
