#ifndef CSCURSOR_H
#define CSCURSOR_H

#include <QString>
#include <QVector>

enum CsClass {
  cccInvalid,

  cccTitle,

  cccVoice,
  cccSinger,

  cccVoiceDual,
  cccComposer,

  cccVoiceLeft,
  cccLyricist,

  cccStyle,
  cccAuthor,

  cccTempo,

  cccRemark,
  cccChord,
  cccNote,
  cccLyric,
  cccTranslation,

  cccLyricSymbol,

  cccChordLine,
  cccChordPlacehold,
  cccClef,
  cccNumerator,
  cccDenomenator,
  cccNoteLine,
  cccNotePlacehold,
  cccLyricLine,
  cccLyricPlacehold

  };


class CsCursor
  {
  protected:
    int     mClass;     //!< Class of cursor, one of CsClass
    int     mPosition;  //!< Position of cursor in tick
    int     mLineIndex; //!< Line index of cursor
    QString mPartName;  //!< Part name inside line
  public:
    CsCursor();

    //===============================================================
    //              Information

    //!
    //! \brief cellClass Returns class of cursor, one of CsClass
    //! \return          Class of cursor, one of CsClass
    //!
    CsClass  cellClass() const { return static_cast<CsClass>(mClass); }

    //!
    //! \brief position Returns position of cursor in tick
    //! \return         Position of cursor in tick
    //!
    int      position() const { return mPosition; }

    //!
    //! \brief lineIndex Returns line index of cursor
    //! \return          Line index of cursor
    //!
    int      lineIndex() const { return mLineIndex; }

    //!
    //! \brief partName Returns part name inside line for current cursor position
    //! \return         Part name inside line for current cursor position
    //!
    QString  partName() const { return mPartName; }

    //!
    //! \brief isMatchClass Returns true if cursor's class match to any of classes in vector
    //! \param v            Vector of classes
    //! \return             true if cursor's class match to any of classes in vector
    //!
    bool     isMatchClass( const QVector<int> &v ) const;

    //!
    //! \brief isMatch  Check if cursor match to theClass
    //! \param theClass Class which need to be checked
    //! \return         true if theClass equals cursor's class
    //!
    bool     isMatch( int theClass ) const { return mClass == theClass; }

    //!
    //! \brief isMatch   Check if cursor match to theClass, lineIndex and partName
    //! \param theClass  Class which need to be checked
    //! \param lineIndex Line index which need to be checked
    //! \param partName  Part name which need to be checked
    //! \return          true if theClass, lineIndex and partName exactly equals to cursor's one
    //!
    bool     isMatch( int theClass, int lineIndex, const QString &partName ) const;

    //!
    //! \brief isMatch   Check if cursor match to theClass, position, lineIndex and partName
    //! \param theClass  Class which need to be checked
    //! \param position  Position which need to be checked
    //! \param lineIndex Line index which need to be checked
    //! \param partName  Part name which need to be checked
    //! \return          true if theClass, position, lineIndex and partName exactly equals to cursor's one
    //!
    bool     isMatch( int theClass, int position, int lineIndex, const QString &partName ) const;

    //!
    //! \brief isMatch   Check if cursor match to theClass, position and lineIndex
    //! \param theClass  Class which need to be checked
    //! \param position  Position which need to be checked
    //! \param lineIndex Line index which need to be checked
    //! \return          true if theClass, position and lineIndex exactly equals to cursor's one
    //!
    bool     isMatch(int theClass, int position, int lineIndex ) const;


  };

#endif // CSCURSOR_H
