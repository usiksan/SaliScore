#include "CsPosition.h"
#include "../SvJson/SvJsonIO.h"



CsPosition::CsPosition(int pos, int duration) :
  mPosition(pos),
  mDuration(duration)
  {

  }



void CsPosition::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonInt( "Duration", mDuration );
  js.jsonInt( "Position", mPosition );
  }




void CsPosition::jsonRead(CsJsonReader &js)
  {
  js.jsonInt( "Duration", mDuration );
  js.jsonInt( "Position", mPosition );
  }
