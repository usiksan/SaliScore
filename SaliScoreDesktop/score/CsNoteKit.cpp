#include "CsNoteKit.h"
#include "../SvJson/SvJsonIO.h"

CsNoteKit::CsNoteKit()
  {

  }


void CsNoteKit::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonMap<CsNoteLine>( "NoteMap", mNoteMap );
  }

void CsNoteKit::jsonRead(CsJsonReader &js)
  {
  js.jsonMap<CsNoteLine>( "NoteMap", mNoteMap );
  }
