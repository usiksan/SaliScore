#ifndef CSCURSOREDITNOTE_H
#define CSCURSOREDITNOTE_H

#include "CsCursorEdit.h"
#include "score/CsNote.h"

class CsCursorEditNote : public CsCursorEdit
  {
    CsNote      mNote;       //!< Edited note
    CsNoteList  mNoteList;   //!< Note line where edited note resides
    int         mNoteIndex;  //!< Index of edited note
  public:
    CsCursorEditNote( int lineIndex, int position, const QString &part, CsComposition &comp );

    // CsCursorEdit interface
  public:
    virtual void keyPress(int key, QChar ch, CsCursorEditPtr &ptr) override;
    virtual int  duration() const override;
    virtual int  noteWhite() const override;
    virtual bool noteDies() const override;
    virtual void apply() override;
  };

#endif // CSCURSOREDITNOTE_H
