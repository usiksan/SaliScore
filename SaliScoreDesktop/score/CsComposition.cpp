#include "CsComposition.h"
#include "CsCompositionSettings.h"

#include "../SvJson/SvJsonIO.h"

#include <QSettings>
#include <QJsonDocument>
#include <QFile>

CsComposition::CsComposition()
  {
  mLineStartOffset = 0;
  mTickPerPart = 4;
  mPartPerTakt = 4;
  mStepChord = 64;
  mStepNote  = 16;
  mStepLyric = 16;
  }




void CsComposition::makeCopy()
  {
  QSettings s;
  mHeader.authorSet( s.value( QStringLiteral(KEY_AUTHOR) ).toString() );
  dirtySet();
  }



void CsComposition::remarkAppend(const QString &lang, const QString &descr)
  {
  dirtySet();
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




void CsComposition::remarkRename(int index, const QString &lang)
  {
  dirtySet();
  //Get previous lang
  QString prevLang = mRemarkList.at(index).mName;
  if( prevLang == lang ) return;
  //Scan all lines and rename remark
  for( int i = 0; i < mLineList.count(); i++ )
    if( mLineList.at(i).isRemark() )
      //Rename lang
      mLineList[i].remarkRename( prevLang, lang );
  //Replace name in definition list
  mRemarkList[index].mName = lang;
  }



void CsComposition::remarkRemove(int index)
  {
  dirtySet();
  //Get lang
  QString lang = mRemarkList.at(index).mName;
  //Scan all lines and rename remark
  for( int i = 0; i < mLineList.count(); i++ )
    if( mLineList.at(i).isRemark() )
      //Remove lang
      mLineList[i].remarkRemove( lang );
  //Remove from definition
  mRemarkList.removeAt(index);
  }




void CsComposition::chordAppend(const QString &part, const QString &descr)
  {
  dirtySet();
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


void CsComposition::chordRename(int index, const QString &part)
  {
  dirtySet();
  //Get previous part
  QString prevPart = mChordList.at(index).mName;
  if( prevPart == part ) return;
  //Scan all lines and rename chord
  for( int i = 0; i < mLineList.count(); i++ )
    if( !mLineList.at(i).isRemark() )
      //Rename part
      mLineList[i].chordRename( prevPart, part );
  //Replace name in definition list
  mChordList[index].mName = part;
  }



void CsComposition::chordRemove(int index)
  {
  dirtySet();
  //Get part
  QString part = mChordList.at(index).mName;
  //Scan all lines and remove chord
  for( int i = 0; i < mLineList.count(); i++ )
    if( !mLineList.at(i).isRemark() )
      //Remove part
      mLineList[i].chordRemove( part );
  //Remove from definition
  mChordList.removeAt(index);
  }




void CsComposition::noteAppend(const QString &part, const QString &descr, int clef)
  {
  dirtySet();
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




void CsComposition::noteRename(int index, const QString &part)
  {
  dirtySet();
  //Get previous part
  QString prevPart = mNoteList.at(index).mName;
  if( prevPart == part ) return;
  //Scan all lines and rename notes
  for( int i = 0; i < mLineList.count(); i++ )
    if( !mLineList.at(i).isRemark() )
      //Rename part
      mLineList[i].noteRename( prevPart, part );
  //Replace name in definition list
  mNoteList[index].mName = part;
  }



void CsComposition::noteRemove(int index)
  {
  dirtySet();
  //Get part
  QString part = mNoteList.at(index).mName;
  //Scan all lines and remove note
  for( int i = 0; i < mLineList.count(); i++ )
    if( !mLineList.at(i).isRemark() )
      //Remove part
      mLineList[i].noteRemove( part );
  //Remove from definition
  mNoteList.removeAt(index);
  }




void CsComposition::translationAppend(const QString &lang, const QString &descr)
  {
  dirtySet();
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



void CsComposition::translationRename(int index, const QString &lang)
  {
  dirtySet();
  //Get previous lang
  QString prevLang = mTranslationList.at(index).mName;
  if( prevLang == lang ) return;
  //Scan all lines and rename notes
  for( int i = 0; i < mLineList.count(); i++ )
    if( !mLineList.at(i).isRemark() )
      //Rename part
      mLineList[i].translationRename( prevLang, lang );
  //Replace name in definition list
  mTranslationList[index].mName = lang;
  }



void CsComposition::translationRemove(int index)
  {
  dirtySet();
  //Get lang
  QString lang = mTranslationList.at(index).mName;
  //Scan all lines and rename translation
  for( int i = 0; i < mLineList.count(); i++ )
    if( !mLineList.at(i).isRemark() )
      //Remove lang
      mLineList[i].translationRemove( lang );
  //Remove from definition
  mTranslationList.removeAt(index);
  }




int CsComposition::lineInsert(int index, bool rem)
  {
  dirtySet();
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




void CsComposition::lineRemove(int index)
  {
  mLineList.removeAt( index );
  }







void CsComposition::jsonWrite(CsJsonWriter &js) const
  {
  mHeader.jsonWrite( js );
  js.jsonString( "Composer", mComposer );
  js.jsonString( "CompLyricist", mLyricist );
  js.jsonList<CsLine>( "LineList", mLineList );
  js.jsonInt( "LineStartOffset", mLineStartOffset );
  js.jsonInt( "TickPerPart", mTickPerPart );
  js.jsonInt( "PartPerTakt", mPartPerTakt );

  js.jsonList<CsDefinition>( "RemarkDefList", mRemarkList );
  js.jsonList<CsDefinition>( "ChordDefList", mChordList );
  js.jsonList<CsDefinition>( "NoteDefList", mNoteList );
  js.jsonList<CsDefinition>( "TranslateList", mTranslationList );

  js.jsonMapInt( "ClefMap", mClefMap );
  }



void CsComposition::jsonRead(CsJsonReader &js)
  {
  mHeader.jsonRead( js );
  js.jsonString( "Composer", mComposer );
  js.jsonString( "CompLyricist", mLyricist );
  js.jsonList<CsLine>( "LineList", mLineList );
  js.jsonInt( "LineStartOffset", mLineStartOffset );
  js.jsonInt( "TickPerPart", mTickPerPart );
  js.jsonInt( "PartPerTakt", mPartPerTakt );

  js.jsonList<CsDefinition>( "RemarkDefList", mRemarkList );
  js.jsonList<CsDefinition>( "ChordDefList", mChordList );
  js.jsonList<CsDefinition>( "NoteDefList", mNoteList );
  js.jsonList<CsDefinition>( "TranslateList", mTranslationList );

  js.jsonMapInt( "ClefMap", mClefMap );

  mStateDirty = mDirty = false;
  }



void CsComposition::settingsRead(const CsCompositionSettings &settings)
  {
  //Setup settings
  mHeader = settings;

  //Setup visibility lists
  defListUpdate( mRemarkList, settings.remarkList() );
  defListUpdate( mChordList,  settings.chordList() );
  defListUpdate( mNoteList,   settings.noteList() );
  defListUpdate( mTranslationList, settings.translationList() );
  }



void CsComposition::clear()
  {
  mHeader.clear();
  mComposer.clear();
  mLyricist.clear();

  mLineStartOffset = 0;

  mStepChord = 64;
  mStepNote  = 16;
  mStepLyric = 16;

  mRemarkList.clear();
  mChordList.clear();
  mNoteList.clear();
  mTranslationList.clear();
  mClefMap.clear();

  mLineList.clear();

  mDirty = mStateDirty = false;
  }



QByteArray CsComposition::toByteArray() const
  {
  //Create writer and use it to write composition contents to json object
  CsJsonWriter js{};
  jsonWrite( js );

  //Append file type and version
  js.jsonString( CS_BASE_TYPE_KEY, QStringLiteral(CS_BASE_TYPE) );
  js.jsonInt( CS_BASE_VERSION_KEY, CS_BASE_VERSION );

  //Write contents to file
  return QJsonDocument(js.object()).toJson();
  }






void CsComposition::fromByteArray(const QByteArray &ar)
  {
  QJsonObject obj = QJsonDocument::fromJson( ar ).object();

  //Check object type and scan version
  if( obj.value( QStringLiteral(CS_BASE_TYPE_KEY) ).toString() == QStringLiteral(CS_BASE_TYPE) ) {
    int version = obj.value( QStringLiteral(CS_BASE_VERSION_KEY) ).toInt();
    CsJsonReader js( obj, &version );
    jsonRead( js );
    }
  }




void CsComposition::fileSave() const
  {
  QFile file( mHeader.path() );
  if( file.open(QIODevice::WriteOnly) ) {
    //Write contents to file
    file.write( toByteArray() );
    }
  }





QStringList CsComposition::visibleList(const CsDefList &src)
  {
  QStringList list;
  for( const auto &def : src )
    if( def.mVisible )
      list.append( def.mName );
  return list;
  }



QString CsComposition::prevVisible(const CsDefList &list, const QString &key)
  {
  int i = key.isEmpty() ? list.count() - 1 : defListIndex( list, key ) - 1;
  if( i < 0 )
    return QString{};
  while( i >= 0 )
    if( list.at(i).mVisible )
      return list.at(i).mName;
    else i--;
  return QString{};
  }




QString CsComposition::nextVisible(const CsDefList &list, const QString &key)
  {
  int i = key.isEmpty() ? 0 : defListIndex( list, key ) + 1;
  if( i < 0 )
    return QString{};
  while( i < list.count() )
    if( list.at(i).mVisible )
      return list.at(i).mName;
    else i++;
  return QString{};
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




void CsComposition::defListUpdate(CsDefList &list, const QStringList &visibleList)
  {
  //Make all parts invisible
  for( int i = 0; i < list.count(); i++ )
    list[i].mVisible = false;

  //For each item from visibleList we find item in list and change it to visible
  for( const auto &key : visibleList ) {
    //Find index for key
    int index = defListIndex( list, key );
    if( index >= 0 )
      list[index].mVisible = true;
    }
  }

