#ifndef CSEDITCURSOR_H
#define CSEDITCURSOR_H

#include "score/CsCursor.h"

class CsComposition;

class CsEditCursor : public CsCursor
  {
  public:
    CsEditCursor();

    void editBegin( const CsComposition &comp, CsCursor &place );

    void editCancel();

    void editApply( CsComposition &comp );
  };

#endif // CSEDITCURSOR_H
