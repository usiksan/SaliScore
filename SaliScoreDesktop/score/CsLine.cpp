#include "CsLine.h"
#include "../SvJson/SvJsonIO.h"

CsLine::CsLine()
  {

  }

CsLine::CsLine( const QString &lang, const QString &rem) :
  mRemark( lang, rem )
  {

  }


void CsLine::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonObject<CsRemark>( "Remark", mRemark );
  js.jsonObject<CsChordKit>( "ChordKit", mChordKit );
  js.jsonObject<CsNoteKit>( "NoteKit", mNoteKit );
  js.jsonList<CsLyric>( "LyricList", mLyricList );
  js.jsonMapString( "Translation", mTranslation );
  }

void CsLine::jsonRead(CsJsonReader &js)
  {
  js.jsonObject<CsRemark>( "Remark", mRemark );
  js.jsonObject<CsChordKit>( "ChordKit", mChordKit );
  js.jsonObject<CsNoteKit>( "NoteKit", mNoteKit );
  js.jsonList<CsLyric>( "LyricList", mLyricList );
  js.jsonMapString( "Translation", mTranslation );
  }
