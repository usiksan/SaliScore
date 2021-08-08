#include "CsCellCursor.h"
#include "score/CsComposition.h"

CsCellCursor::CsCellCursor(CsComposition &comp) :
  mComposition(comp),
  mCellClass(cccProperty),
  mLineIndex(0)
  {
  mPosition = ccpTitle;
  }




void CsCellCursor::move(CsCellCursorOperation oper, bool doSelect, int n)
  {
  switch( oper ) {
    case ccoNoMove :
    default:
      //Nothing doing
      break;

    case ccoStart :

    }
  }
