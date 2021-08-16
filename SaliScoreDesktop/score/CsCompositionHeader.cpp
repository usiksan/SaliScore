#include "CsCompositionHeader.h"

CsCompositionHeader::CsCompositionHeader()
  {

  }

void CsCompositionHeader::jsonWrite(CsJsonWriter &js) const
  {
  CsDescrSong::jsonWrite( js );
  mSettings.jsonWrite( js );
  }

void CsCompositionHeader::jsonRead(CsJsonReader &js)
  {
  CsDescrSong::jsonRead( js );
  mSettings.jsonRead( js );
  }
