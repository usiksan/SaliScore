#include "CsChord.h"

CsChord::CsChord()
  {

  }


void CsChord::jsonWrite(SvJsonWriter &js) const
  {
  CsPosition::jsonWrite( js );
  }

void CsChord::jsonRead(SvJsonReader &js)
  {
  CsPosition::jsonRead( js );
  }
