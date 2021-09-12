#ifndef CSCURSOREDITLYRIC_H
#define CSCURSOREDITLYRIC_H

#include "CsCursorEdit.h"
#include "score/CsLyricSymbol.h"

class CsCursorEditLyric : public CsCursorEdit
  {
    CsLyricLine mLyricLine;  //!< Lyric line to edit
    int         mCharPos;    //!< Char cursor position
    int         mStartSel;   //!< Char start select position
    int         mStopSel;    //!< Char stop select position
  public:
    CsCursorEditLyric(int lineIndex, CsComposition &comp );

  protected:

    //!
    //! \brief movePos Move cursor (insert) position
    //! \param pos     New position of cursor
    //! \param sel     If true then text from current position to pos is make selected
    //!
    void movePos(int pos, bool sel);

    //!
    //! \brief setSel Define selection
    //! \param start  Start position of selection
    //! \param stop   Stop position of selection
    //!
    void setSel(int start, int stop);

    //!
    //! \brief insertText Insert text in current position with selection param
    //! \param str        Text to insert
    //! \param sel        If true then inserted text is make selected
    //!
    void insertText( const QString str, bool sel );

    //!
    //! \brief setText Clear old text and insert new one with selection param
    //! \param str     Text to set
    //! \param sel     If true then inserted text is make selected
    //!
    void setText( const QString str, bool sel );

    //!
    //! \brief delSelected Delete selection if present
    //!
    void delSelected();

    void insertAlign( int align );

    // CsCursorEdit interface
  public:
    virtual void         keyPress(int key, QChar ch, CsCursorEditPtr &ptr) override;
    virtual int          duration() const override;
    virtual void         apply() override;
    virtual CsLyricLine  getLyric() const override { return mLyricLine; }
    virtual bool         isSelectionPresent() const override;
    virtual int          selStart() const override { return mStartSel; }
    virtual int          selStop() const override { return mStopSel; }
    virtual int          charPosition() const override { return mCharPos; }
    virtual void         clipboardCut() override;
    virtual void         clipboardPaste() override;
    virtual void         clipboardCopy() override;

  };

#endif // CSCURSOREDITLYRIC_H
