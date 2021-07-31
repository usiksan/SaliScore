#include "CsNoteLine.h"
#include "../SvJson/SvJsonIO.h"

CsNoteLine::CsNoteLine()
  {

  }

void CsNoteLine::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonList<CsNote>( "NoteList", mNoteList );
  }

void CsNoteLine::jsonRead(CsJsonReader &js)
  {
  js.jsonList<CsNote>( "NoteList", mNoteList );
  }
