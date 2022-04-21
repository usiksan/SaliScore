#include "CsCursorEditProperty.h"
#include "score/CsComposition.h"

CsCursorEditProperty::CsCursorEditProperty(const QString &partName, CsComposition &comp) :
  CsCursorEditString( comp )
  {
  mClass = cccAttribute;
  mPartName = partName;
  setText( comp.attributeGet(partName), true );
  }





void CsCursorEditProperty::apply()
  {
  mComposition.attributeSet( mPartName, mString );
  }
