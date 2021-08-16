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
