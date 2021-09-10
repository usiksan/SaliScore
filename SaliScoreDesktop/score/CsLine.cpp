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
  js.jsonMapString( "Translation", mTranslation );
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
  js.jsonInt( "taktCount", mTaktCount );

  //Read packet lyric
  QString lyricString;
  js.jsonString( "LyricLine", lyricString );
  stringToLyric( lyricString );
  }





QString CsLine::lyricToString() const
  {
  QString line;
  for( auto const &lyricSymbol : mLyricLine )
    line.append( lyricSymbol.string() );
  return line;
  }



void CsLine::stringToLyric(const QString line)
  {
  mLyricLine.clear();
  for( int i = 0; i < line.count(); i++ )
    if( line.at(i) == CsLyricAlignPrefix && (i + 1) < line.count() ) {
      i++;
      //Append symbol as alignment
      mLyricLine.append( CsLyricSymbol( line.at(i).toLatin1() ) );
      }
    else
      //Append simple symbol
      mLyricLine.append( CsLyricSymbol( line.at(i) ) );
  }
