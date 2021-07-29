#include "CsNoteKit.h"
#include "../SvJson/SvJsonIO.h"

CsNoteKit::CsNoteKit()
  {

  }

void CsNoteKit::jsonWrite(SvJsonWriter &js) const
  {
  js.jsonMap<CsNoteLine>( "NoteMap", mNoteMap );
  }

void CsNoteKit::jsonRead(SvJsonReader &js)
  {
  js.jsonMap<CsNoteLine>( "NoteMap", mNoteMap );
  }
