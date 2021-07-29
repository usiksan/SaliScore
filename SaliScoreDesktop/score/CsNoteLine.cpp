#include "CsNoteLine.h"
#include "../SvJson/SvJsonIO.h"

CsNoteLine::CsNoteLine()
  {

  }

void CsNoteLine::jsonWrite(SvJsonWriter &js) const
  {
  js.jsonList<CsNote>( "NoteList", mNoteList );
  }

void CsNoteLine::jsonRead(SvJsonReader &js)
  {
  js.jsonList<CsNote>( "NoteList", mNoteList );
  }
