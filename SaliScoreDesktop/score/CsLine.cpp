#include "CsLine.h"
#include "../SvJson/SvJsonIO.h"

CsLine::CsLine()
  {

  }

void CsLine::jsonWrite(SvJsonWriter &js) const
  {
  js.jsonObject<CsRemark>( "Remark", mRemark );
  js.jsonObject<CsChordKit>( "ChordKit", mChordKit );
  js.jsonObject<CsNoteKit>( "NoteKit", mNoteKit );
  js.jsonList<CsLyric>( "LyricList", mLyricList );
  js.jsonMapString( "Translation", mTranslation );
  }

void CsLine::jsonRead(SvJsonReader &js)
  {
  js.jsonObject<CsRemark>( "Remark", mRemark );
  js.jsonObject<CsChordKit>( "ChordKit", mChordKit );
  js.jsonObject<CsNoteKit>( "NoteKit", mNoteKit );
  js.jsonList<CsLyric>( "LyricList", mLyricList );
  js.jsonMapString( "Translation", mTranslation );
  }
