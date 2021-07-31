#include "CsState.h"

CsState::CsState()
  {

  }

void CsState::remarkAppend(const QString name, const QString descr)
  {
  mRemarkList.append( CsDefinition( name, descr ) );
  }

void CsState::chordAppend(const QString name, const QString descr)
  {
  mChordList.append( CsDefinition( name, descr ) );
  }

void CsState::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonList<CsDefinition>( "RemarkDefList", mRemarkList );
  js.jsonList<CsDefinition>( "ChordDefList", mChordList );
  js.jsonList<CsDefinition>( "NoteDefList", mNoteList );
  js.jsonList<CsDefinition>( "TranslateList", mTranslateList );
  }

void CsState::jsonRead(CsJsonReader &js)
  {
  js.jsonList<CsDefinition>( "RemarkDefList", mRemarkList );
  js.jsonList<CsDefinition>( "ChordDefList", mChordList );
  js.jsonList<CsDefinition>( "NoteDefList", mNoteList );
  js.jsonList<CsDefinition>( "TranslateList", mTranslateList );
  }



QStringList CsState::visibleList(const CsDefList &src)
  {
  }
