#include "CsCursorEditTranslation.h"
#include "score/CsComposition.h"

CsCursorEditTranslation::CsCursorEditTranslation(int lineIndex, const QString &part, CsComposition &comp) :
  CsCursorEditString( comp )
  {
  mClass     = cccTranslation;
  mLineIndex = lineIndex;
  mPartName  = part;

  setText( comp.translationGet( mLineIndex, mPartName ), false );
  }


void CsCursorEditTranslation::apply()
  {
  mComposition.translationSet( mLineIndex, mPartName, mString );
  }
