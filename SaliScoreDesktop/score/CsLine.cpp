#include "CsLine.h"
#include "../SvJson/SvJsonIO.h"

CsLine::CsLine() :
  mTaktCount(8)
  {

  }

CsLine::CsLine( const QString &lang, const QString &rem) :
  mRemark( lang, rem ),
  mTaktCount(0)
  {

  }

void CsLine::translationRename(const QString &prevLang, const QString &newLang)
  {
  mTranslation.insert( newLang, mTranslation.value(prevLang) );
  mTranslation.remove( prevLang );
  }



void CsLine::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonObject<CsRemark>( "Remark", mRemark );
  js.jsonObject<CsChordKit>( "ChordKit", mChordKit );
  js.jsonObject<CsNoteKit>( "NoteKit", mNoteKit );
  js.jsonList<CsLyric>( "LyricList", mLyricList );
  js.jsonMapString( "Translation", mTranslation );
  js.jsonInt( "taktCount", mTaktCount );
  }

void CsLine::jsonRead(CsJsonReader &js)
  {
  js.jsonObject<CsRemark>( "Remark", mRemark );
  js.jsonObject<CsChordKit>( "ChordKit", mChordKit );
  js.jsonObject<CsNoteKit>( "NoteKit", mNoteKit );
  js.jsonList<CsLyric>( "LyricList", mLyricList );
  js.jsonMapString( "Translation", mTranslation );
  js.jsonInt( "taktCount", mTaktCount );
  }
