#include "CsPosition.h"



CsPosition::CsPosition(int pos, int duration) :
  mLinePosition(pos),
  mDuration(duration)
  {

  }



void CsPosition::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonInt( "Duration", mDuration );
  js.jsonInt( "Position", mLinePosition );
  }




void CsPosition::jsonRead(CsJsonReader &js)
  {
  js.jsonInt( "Duration", mDuration );
  js.jsonInt( "Position", mLinePosition );
  }
