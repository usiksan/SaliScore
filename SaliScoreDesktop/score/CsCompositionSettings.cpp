#include "CsCompositionSettings.h"

CsCompositionSettings::CsCompositionSettings()
  {

  }



CsCompositionSettings::CsCompositionSettings(const CsComposition &comp) :
  CsCompositionHeader( comp.header() ),
  mRemarkList( comp.remarkVisible() ),
  mChordList( comp.chordVisible() ),
  mNoteList( comp.noteVisible() ),
  mTranslationList( comp.translationVisible() )
  {

  }




void CsCompositionSettings::jsonWrite(CsJsonWriter &js) const
  {
  CsCompositionHeader::jsonWrite( js );
  js.jsonListString( "remarkVisible", mRemarkList );
  js.jsonListString( "chordVisible", mChordList );
  js.jsonListString( "noteVisible", mNoteList );
  js.jsonListString( "translationVisible", mTranslationList );
  }




void CsCompositionSettings::jsonRead(CsJsonReader &js)
  {
  CsCompositionHeader::jsonRead( js );
  js.jsonListString( "remarkVisible", mRemarkList );
  js.jsonListString( "chordVisible", mChordList );
  js.jsonListString( "noteVisible", mNoteList );
  js.jsonListString( "translationVisible", mTranslationList );
  }
