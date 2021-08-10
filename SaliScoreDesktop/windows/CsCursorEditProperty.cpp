#include "CsCursorEditProperty.h"
#include "score/CsComposition.h"

CsCursorEditProperty::CsCursorEditProperty(int propertyId, CsComposition &comp) :
  CsCursorEditString( comp )
  {
  mClass = propertyId;

  switch( propertyId ) {
    case cccTitle :
      setText( comp.title(), false );
      break;
    }
  }


void CsCursorEditProperty::apply()
  {
  switch( mClass ) {
    case cccTitle :
      mComposition.titleSet( mString );
      break;
    }
  }
