#include "CsPosition.h"
#include "../SvJson/SvJsonIO.h"



CsPosition::CsPosition(int pos) :
  mPosition(pos)
  {

  }



void CsPosition::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonInt( "Position", mPosition );
  }




void CsPosition::jsonRead(CsJsonReader &js)
  {
  js.jsonInt( "Position", mPosition );
  }
