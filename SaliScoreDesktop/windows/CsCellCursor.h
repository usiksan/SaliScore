#ifndef CSCELLCURSOR_H
#define CSCELLCURSOR_H

#include <QString>

enum CsCellClass {
  cccTitle,
  cccSinger,
  cccComposer,
  cccLyricist,
  cccAuthor,

  cccVoice,
  cccVoiceDual,
  cccVoiceRight,
  cccStyle,
  cccTempo,

  cccRemark,
  cccChord,
  cccNote,
  cccLyric,
  cccTranslation
  };

enum CsCellProperty {
  };

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

class CsCellCursor
  {
    CsComposition &mComposition; //!< Composition on which works this cursor
    CsCellClass    mCellClass;
    int            mPosition;
    int            mLineIndex;
    QString        mPartName;
  public:
    CsCellCursor( CsComposition &comp );

    CsCellClass    cellClass() const { return mCellClass; }

    int            position() const { return mPosition; }

    CsCellProperty propertyId() const { return static_cast<CsCellProperty>(mPosition); }

    int            lineIndex() const { return mLineIndex; }

    QString        partName() const { return mPartName; }

    void           move( CsCellCursorOperation oper, bool doSelect = false, int n = 1 );
  };

#endif // CSCELLCURSOR_H
