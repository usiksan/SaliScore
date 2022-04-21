#include "CsLine.h"
#include "CsNoteChord.h"
#include "../SvJson/SvJsonIO.h"

CsLine::CsLine() :
  mTaktCount(4)
  {
  }



CsLine::CsLine( const QString &lang, const QString &rem) :
  mTaktCount(0)
  {
  mRemark.set( lang, rem );
  }




void CsLine::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonValue( "RemarkKit", mRemark );
  js.jsonValue( "ChordKit", mChordKit );
  js.jsonValue( "NoteKit", mNoteKit );
  js.jsonInt( "TaktCount", mTaktCount );

  //Write packet lyric
  js.jsonString( "LyricLine", lyricLineToString( mLyricLine ) );
  }





void CsLine::jsonRead(CsJsonReader &js)
  {
  js.jsonValue( "RemarkKit", mRemark );
  js.jsonValue( "ChordKit", mChordKit );
  js.jsonValue( "NoteKit", mNoteKit );
  js.jsonInt( "TaktCount", mTaktCount );

  //Read packet lyric
  QString lyricString;
  js.jsonString( "LyricLine", lyricString );
  mLyricLine = lyricLineFromString( lyricString );
  }



