#include "CsNote.h"
#include "../SvJson/SvJsonIO.h"

CsNote::CsNote()
  {

  }


void CsNote::jsonWrite(CsJsonWriter &js) const
  {
  CsPosition::jsonWrite( js );
  js.jsonInt( "Duration", mDuration );
  js.jsonInt( "Pitch", mPitch );
  }



void CsNote::jsonRead(CsJsonReader &js)
  {
  CsPosition::jsonRead( js );
  js.jsonInt( "Duration", mDuration );
  js.jsonInt( "Pitch", mPitch );
  }
