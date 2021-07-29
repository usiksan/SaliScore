#include "CsChordKit.h"
#include "../SvJson/SvJsonIO.h"

CsChordKit::CsChordKit()
  {

  }

void CsChordKit::jsonWrite(SvJsonWriter &js) const
  {
  js.jsonMap<CsChordLine>( "ChordLines", mChordLines );
  }


void CsChordKit::jsonRead(SvJsonReader &js)
  {
  js.jsonMap<CsChordLine>( "ChordLines", mChordLines );
  }
