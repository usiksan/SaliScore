#include "CsPosition.h"
#include "../SvJson/SvJsonIO.h"



CsPosition::CsPosition()
  {

  }



void CsPosition::jsonWrite(SvJsonWriter &js) const
  {
  js.jsonInt( "Position", mPosition );
  }




void CsPosition::jsonRead(SvJsonReader &js)
  {
  js.jsonInt( "Position", mPosition );
  }
