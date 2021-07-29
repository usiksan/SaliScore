#include "CsLyric.h"

#include "../SvJson/SvJsonIO.h"

CsLyric::CsLyric()
  {

  }



void CsLyric::jsonWrite(SvJsonWriter &js) const
  {
  CsPosition::jsonWrite( js );
  js.jsonString( "text", mLyric );
  }



void CsLyric::jsonRead(SvJsonReader &js)
  {
  CsPosition::jsonRead( js );
  js.jsonString( "text", mLyric );
  }
