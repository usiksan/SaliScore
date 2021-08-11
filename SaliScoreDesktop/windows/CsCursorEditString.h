#ifndef CSCURSOREDITSTRING_H
#define CSCURSOREDITSTRING_H

#include "CsCursorEdit.h"

class CsCursorEditString : public CsCursorEdit
  {
  protected:
    //Line text editor
    QString mString;   //!< Work string
    int     mCharPos;  //!< Char cursor position
    int     mStartSel; //!< Char start select position
    int     mStopSel;  //!< Char stop select position
  public:
    CsCursorEditString( CsComposition &comp );

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


    // CsCursorEdit interface
  public:
    virtual void    keyPress(int key, QChar ch, CsCursorEditPtr &ptr) override;
    virtual QString getStr() const override;
    virtual bool    isSelectionPresent() const override;
    virtual int     selStart() const override;
    virtual int     selStop() const override;
    virtual int     charPosition() const override;
    virtual void    clipboardCut() override;
    virtual void    clipboardPaste() override;
    virtual void    clipboardCopy() override;
  };

#endif // CSCURSOREDITSTRING_H
