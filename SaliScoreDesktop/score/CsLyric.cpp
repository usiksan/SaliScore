#include "CsLyric.h"

#include "../SvJson/SvJsonIO.h"

CsLyric::CsLyric()
  {

  }



void CsLyric::jsonWrite(CsJsonWriter &js) const
  {
  CsPosition::jsonWrite( js );
  js.jsonString( "text", mLyric );
  }



void CsLyric::jsonRead(CsJsonReader &js)
  {
  CsPosition::jsonRead( js );
  js.jsonString( "text", mLyric );
  }
