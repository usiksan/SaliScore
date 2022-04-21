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

    void updatePosition();

    void move( CsCellCursorOperation oper, bool doSelect = false, int n = 1 );

    void jump( int aclass, int aposition, int aline, const QString &apart );

    void jump( int aclass, int aposition, int aline ) { jump( aclass, aposition, aline, QString{} ); }

    void jump( int aclass, const QString &apart ) { jump( aclass, 0, -1, apart ); }

  private:
    void setPosition( int pos, int step );

    void movePrevPart();

    void moveNextPart();

    void moveUp();

    void moveDown();

    void normPosition( int step );

    static QMap<QString,QString> mMoveAttrLeft;
    static QMap<QString,QString> mMoveAttrRight;
    static QMap<QString,QString> mMoveAttrTop;
    static QMap<QString,QString> mMoveAttrBot;
  };

#endif // CSCELLCURSOR_H
