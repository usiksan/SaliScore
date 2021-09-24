#include "CsLine.h"
#include "CsNoteChord.h"
#include "../SvJson/SvJsonIO.h"

CsLine::CsLine() :
  mTickPerTakt(duraHole),
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
  js.jsonMapString( "Translation", mTranslation );
  js.jsonInt( "tickPerTakt", mTickPerTakt );
  js.jsonInt( "taktCount", mTaktCount );

  //Write packet lyric
  QString lyricString = lyricToString();
  js.jsonString( "LyricLine", lyricString );
  }





void CsLine::jsonRead(CsJsonReader &js)
  {
  js.jsonObject<CsRemark>( "Remark", mRemark );
  js.jsonObject<CsChordKit>( "ChordKit", mChordKit );
  js.jsonObject<CsNoteKit>( "NoteKit", mNoteKit );
  js.jsonMapString( "Translation", mTranslation );
  js.jsonInt( "tickPerTakt", mTickPerTakt );
  js.jsonInt( "taktCount", mTaktCount );

  //Read packet lyric
  QString lyricString;
  js.jsonString( "LyricLine", lyricString );
  stringToLyric( lyricString );

  if( mTickPerTakt == 0 ) mTickPerTakt = duraHole;
  }





QString CsLine::lyricToString() const
  {
  return lyricLineToString( mLyricLine );
  }



void CsLine::stringToLyric(const QString line)
  {
  mLyricLine = lyricLineFromString( line );
  }
