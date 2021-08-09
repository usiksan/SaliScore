#ifndef CSCELLCURSOR_H
#define CSCELLCURSOR_H

#include <QString>

enum CsCellClass {
  cccTitle,

  cccVoice,
  cccSinger,

  cccVoiceDual,
  cccComposer,

  cccVoiceRight,
  cccLyricist,

  cccStyle,
  cccAuthor,

  cccTempo,

  cccRemark,
  cccChord,
  cccNote,
  cccLyric,
  cccTranslation
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
    int            mCellClass;
    int            mPosition;
    int            mLineIndex;
    QString        mPartName;
  public:
    CsCellCursor( CsComposition &comp );

    bool           isCurrent( int cellClass ) const { return mCellClass == cellClass; }

    bool           isCurrent( int cellClass, int lineIndex, const QString &partName ) const;

    bool           isCurrent( int cellClass, int position, int lineIndex, const QString &partName ) const;

    bool           isCurrent( int cellClass, int position, int lineIndex ) const;

    CsCellClass    cellClass() const { return static_cast<CsCellClass>(mCellClass); }

    int            position() const { return mPosition; }

    int            lineIndex() const { return mLineIndex; }

    QString        partName() const { return mPartName; }

    void           move( CsCellCursorOperation oper, bool doSelect = false, int n = 1 );

  private:
    void           setPosition( int pos, int step );

    void           movePrevPart();

    void           moveNextPart();

    void           moveUp();

    void           moveDown();
  };

#endif // CSCELLCURSOR_H
