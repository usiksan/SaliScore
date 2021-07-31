#include "CsChordKit.h"
#include "../SvJson/SvJsonIO.h"

CsChordKit::CsChordKit()
  {

  }


void CsChordKit::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonMap<CsChordLine>( "ChordMap", mChordMap );
  }


void CsChordKit::jsonRead(CsJsonReader &js)
  {
  js.jsonMap<CsChordLine>( "ChordMap", mChordMap );
  }
