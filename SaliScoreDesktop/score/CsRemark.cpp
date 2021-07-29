#include "CsRemark.h"
#include "../SvJson/SvJsonIO.h"

CsRemark::CsRemark()
  {

  }

void CsRemark::jsonWrite(SvJsonWriter &js) const
  {
  js.jsonMapString( "StringMap", mStringMap );
  }

void CsRemark::jsonRead(SvJsonReader &js)
  {
  js.jsonMapString( "StringMap", mStringMap );
  }
