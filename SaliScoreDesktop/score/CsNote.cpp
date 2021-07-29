#include "CsNote.h"
#include "../SvJson/SvJsonIO.h"

CsNote::CsNote()
  {

  }


void CsNote::jsonWrite(SvJsonWriter &js) const
  {
  CsPosition::jsonWrite( js );
  js.jsonInt( "Duration", mDuration );
  js.jsonInt( "Pitch", mPitch );
  }



void CsNote::jsonRead(SvJsonReader &js)
  {
  CsPosition::jsonRead( js );
  js.jsonInt( "Duration", mDuration );
  js.jsonInt( "Pitch", mPitch );
  }
