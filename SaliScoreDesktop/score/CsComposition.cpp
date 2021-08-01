#include "CsComposition.h"

#include "../SvJson/SvJsonIO.h"

CsComposition::CsComposition()
  {

  }



void CsComposition::remarkAppend(const QString &lang, const QString &descr)
  {
  mDirty = true;
  //Test if lang already exist
  int index = remarkIndex(lang);
  if( index < 0 ) {
    //No remark with this lang, append
    mRemarkList.append( CsDefinition( lang, descr ) );
    //Run over all lines remark and append lang
    for( int i = 0; i < mLineList.count(); i++ )
      if( mLineList.at(i).isRemark() )
        //Append new lang
        mLineList[i].remarkSet( lang, QString{} );
    }
  else {
    //Remark already exist, so change description only
    mRemarkList[index].mDescription = descr;
    }
  }




void CsComposition::chordAppend(const QString &part, const QString &descr)
  {
  mDirty = true;
  //Test if part already exist
  int index = chordIndex(part);
  if( index < 0 ) {
    //No chord with this part, append
    mChordList.append( CsDefinition( part, descr ) );
    //Empty chord list
    CsChordList chordList;
    //Run over all lines and append chord to non remark line
    for( int i = 0; i < mLineList.count(); i++ )
      if( !mLineList.at(i).isRemark() )
        //Append new chord
        mLineList[i].chordListSet( part, chordList );
    }
  else {
    //Chord already exist, so change description only
    mChordList[index].mDescription = descr;
    }
  }




void CsComposition::noteAppend(const QString &part, const QString &descr, int clef)
  {
  mDirty = true;
  //Test if part already exist
  int index = noteIndex(part);
  if( index < 0 ) {
    //No note with this part, append
    mNoteList.append( CsDefinition( part, descr ) );
    mClefMap.insert( part, clef );
    //Empty note list
    CsNoteList noteList;
    //Run over all lines and append note to non remark line
    for( int i = 0; i < mLineList.count(); i++ )
      if( !mLineList.at(i).isRemark() )
        //Append new chord
        mLineList[i].noteListSet( part, noteList );
    }
  else {
    //Chord already exist, so change description only
    mNoteList[index].mDescription = descr;
    mClefMap.insert( part, clef );
    }
  }




void CsComposition::translationAppend(const QString &lang, const QString &descr)
  {
  mDirty = true;
  //Test if lang already exist
  int index = translationIndex(lang);
  if( index < 0 ) {
    //No translation with this lang, append
    mTranslationList.append( CsDefinition( lang, descr ) );
    //Run over all non remark lines and append lang
    for( int i = 0; i < mLineList.count(); i++ )
      if( !mLineList.at(i).isRemark() )
        //Append new lang
        mLineList[i].translationSet( lang, QString{} );
    }
  else {
    //Remark already exist, so change description only
    mTranslationList[index].mDescription = descr;
    }
  }




int CsComposition::lineInsert(int index, bool rem)
  {
  mDirty = true;
  //Create new line
  CsLine line;
  if( rem ) {
    //We need to create remark line
    //Append all remark languages
    if( mRemarkList.count() == 0 )
      return -1;
    for( auto const &def : qAsConst(mRemarkList) )
      line.remarkSet( def.mName, QString{} );
    }
  else {
    //We build song line
    //Append all chord
    CsChordList chordList;
    for( auto const &def : qAsConst(mChordList) )
      line.chordListSet( def.mName, chordList );

    //Append all notes
    CsNoteList noteList;
    for( auto const &def : qAsConst(mNoteList) )
      line.noteListSet( def.mName, noteList );

    //Append all translations
    for( auto const &def : qAsConst(mTranslationList) )
      line.translationSet( def.mName, QString{} );
    }

  //Line created, insert
  if( index < 0 ) {
    mLineList.append( line );
    return mLineList.count() - 1;
    }
  mLineList.insert( index, line );
  return index;
  }







void CsComposition::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonString( "CompTitle", mTitle );
  js.jsonString( "CompSinger", mSinger );
  js.jsonString( "Composer", mComposer );
  js.jsonString( "CompLyricist", mLyricist );
  js.jsonList<CsLine>( "LineList", mLineList );

  js.jsonList<CsDefinition>( "RemarkDefList", mRemarkList );
  js.jsonList<CsDefinition>( "ChordDefList", mChordList );
  js.jsonList<CsDefinition>( "NoteDefList", mNoteList );
  js.jsonList<CsDefinition>( "TranslateList", mTranslationList );

  js.jsonMapInt( "ClefMap", mClefMap );
  }



void CsComposition::jsonRead(CsJsonReader &js)
  {
  js.jsonString( "CompTitle", mTitle );
  js.jsonString( "CompSinger", mSinger );
  js.jsonString( "Composer", mComposer );
  js.jsonString( "CompLyricist", mLyricist );
  js.jsonList<CsLine>( "LineList", mLineList );

  js.jsonList<CsDefinition>( "RemarkDefList", mRemarkList );
  js.jsonList<CsDefinition>( "ChordDefList", mChordList );
  js.jsonList<CsDefinition>( "NoteDefList", mNoteList );
  js.jsonList<CsDefinition>( "TranslateList", mTranslationList );

  js.jsonMapInt( "ClefMap", mClefMap );

  mDirty = false;
  }



QStringList CsComposition::visibleList(const CsDefList &src)
  {
  QStringList list;
  for( const auto &def : src )
    if( def.mVisible )
      list.append( def.mName );
  return list;
  }




int CsComposition::defListIndex(const CsDefList &list, const QString &key)
  {
  //Prepare index for finding
  int i = 0;

  //Scan list and test for equal name
  for( const auto &def : list )
    if( def.mName == key )
      return i;
    else
      i++;

  //Key not found in list
  return -1;
  }

