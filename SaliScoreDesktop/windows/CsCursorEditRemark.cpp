#include "CsCursorEditRemark.h"

#include "score/CsComposition.h"

CsCursorEditRemark::CsCursorEditRemark(int lineIndex, const QString &part, CsComposition &comp) :
  CsCursorEditString( comp )
  {
  mClass     = cccRemark;
  mLineIndex = lineIndex;
  mPartName  = part;

  setText( comp.remarkGet( mLineIndex, mPartName ), false );
  }


void CsCursorEditRemark::apply()
  {
  mComposition.remarkSet( mLineIndex, mPartName, mString );
  }
