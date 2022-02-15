#include "CsNoteLine.h"
#include "../SvJson/SvJsonIO.h"


CsNoteLine::CsNoteLine()
  {

  }

void CsNoteLine::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonList<CsNote>( "NoteList", mNoteList );
  js.jsonInt( "Numerator", mNumerator );
  js.jsonInt( "Denomenator", mDenominator );
  }

void CsNoteLine::jsonRead(CsJsonReader &js)
  {
  js.jsonList<CsNote>( "NoteList", mNoteList );
  js.jsonInt( "Numerator", mNumerator );
  js.jsonInt( "Denomenator", mDenominator );
  }
