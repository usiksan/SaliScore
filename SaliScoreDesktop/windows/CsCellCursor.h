#ifndef CSCELLCURSOR_H
#define CSCELLCURSOR_H

#include "score/CsCursor.h"

#include <QString>



enum CsCellCursorOperation
  {
  ccoNoMove, //!< No move operation
  ccoStart,  //!< Move to start of composition
  ccoStartLine, //!< Move to start of line whithout change part name
  ccoLeft,      //!< Move to left position
  ccoRight,     //!< Move to right position
  ccoUp,        //!< Move to previous part, class or line
  ccoDown,      //!< Move to next part, class or line

  ccoLast
  };


class CsComposition;

class CsCellCursor : public CsCursor
  {
    CsComposition &mComposition; //!< Composition on which works this cursor
  public:
    CsCellCursor( CsComposition &comp );

    void moveTop();

    void move( CsCellCursorOperation oper, bool doSelect = false, int n = 1 );

  private:
    void setPosition( int pos, int step );

    void movePrevPart();

    void moveNextPart();

    void moveUp();

    void moveDown();

    void normPosition( int step );
  };

#endif // CSCELLCURSOR_H
