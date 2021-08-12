#ifndef CSCURSOREDITCHORD_H
#define CSCURSOREDITCHORD_H

#include "CsCursorEdit.h"
#include "score/CsChordLine.h"

class CsCursorEditChord : public CsCursorEdit
  {
    CsChord     mChord;      //!< Edited chord
    CsChordList mChordList;  //!< Chord line where edited chord resides
    int         mChordIndex; //!< Index of edited chord
  public:
    CsCursorEditChord( int lineIndex, int position, const QString &part, CsComposition &comp );

    // CsCursorEdit interface
  public:
    virtual void    keyPress(int key, QChar ch, CsCursorEditPtr &ptr) override;
    virtual QString getStr() const override;
    virtual void    cancel() override;
    virtual void    apply() override;
  };

#endif // CSCURSOREDITCHORD_H
