#include "CsNoteKit.h"
#include "../SvJson/SvJsonIO.h"

CsNoteKit::CsNoteKit()
  {

  }

void CsNoteKit::noteRename(const QString &prevPart, const QString &newPart)
  {
  mNoteMap.insert( newPart, mNoteMap.value(prevPart) );
  mNoteMap.remove( prevPart );
  }


void CsNoteKit::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonMap<CsNoteLine>( "NoteMap", mNoteMap );
  }

void CsNoteKit::jsonRead(CsJsonReader &js)
  {
  js.jsonMap<CsNoteLine>( "NoteMap", mNoteMap );
  }
