#include "CsLine.h"
#include "CsTrackString.h"

CsLine::CsLine() :
  mTaktCount(4)
  {

  }


CsLine::CsLine( const QString &part, const QString &rem ) :
  mTaktCount(4)
  {
  mTrackMap.insert( part, rem );
  }




CsChordList CsLine::chordListGet(const QString &part) const
  {
  //Check if there in cashe
  if( mChordMap.contains(part) )
    return mChordMap.value(part);

  //No in cashe, we need convert from string
  CsChordList list;
  parseList( mTrackMap.value(part), list );
  //Insert in cashe
  mChordMap.insert( part, list );
  return list;
  }



void CsLine::chordListSet( const QString &part, const CsChordList &chordList )
  {
  //Replace in cashe
  mChordMap.insert( part, chordList );
  //Replace in map
  mTrackMap.insert( part, toString( chordList ) );
  }



CsNoteList CsLine::noteListGet(const QString &part) const
  {
  //Check if there in cashe
  if( mNoteMap.contains(part) )
    return mNoteMap.value(part);

  //No in cashe, we need convert from string
  CsNoteList list;
  parseList( mTrackMap.value(part), list );
  //Insert in cashe
  mNoteMap.insert( part, list );
  return list;
  }


void CsLine::noteListSet( const QString &part, const CsNoteList &noteList )
  {
  //Replace in cashe
  mNoteMap.insert( part, noteList );
  //Replace in map
  mTrackMap.insert( part, toString( noteList ) );
  }




CsLyricLine CsLine::lyricLineGet() const
  {
  if( mLyricLine.count() || mTrackMap.value(KEY_LYRIC).isEmpty() )
    return mLyricLine;

  return mLyricLine = lyricLineFromString( mTrackMap.value(KEY_LYRIC) );
  }




void CsLine::lyricLineSet(const CsLyricLine &line)
  {
  mLyricLine = line;
  mTrackMap.insert( KEY_LYRIC, lyricLineToString(line) );
  }


void CsLine::partRename( const QString &prevPart, const QString &newPart )
  {
  if( mTrackMap.contains( prevPart ) ) {
    auto track = mTrackMap.value( prevPart );
    mTrackMap.remove( prevPart );
    mTrackMap.insert( newPart, track );
    }
  }



void CsLine::partRemove(const QString &part)
  {
  if( mTrackMap.contains(part) )
    mTrackMap.remove(part);
  }



void CsLine::clear()
  {
  mTrackMap.clear();
  mTaktCount = 4;
  clearCashe();
  }

void CsLine::clearCashe()
  {
  mChordMap.clear();
  mNoteMap.clear();
  mLyricLine.clear();
  }



void CsLine::json(SvJsonWriter &js) const
  {
  js.jsonMapString( "parts", mTrackMap );
  js.jsonInt( "taktCount", mTaktCount );
  }



void CsLine::json(const SvJsonReader &js)
  {
  js.jsonMapString( "parts", mTrackMap );
  js.jsonInt( "taktCount", mTaktCount );
  }
