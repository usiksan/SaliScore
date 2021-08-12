#ifndef CSCURSOREDITLYRIC_H
#define CSCURSOREDITLYRIC_H

#include "CsCursorEditString.h"
#include "score/CsLyric.h"

class CsCursorEditLyric : public CsCursorEditString
  {
    CsLyricList mLyricList;
    int         mLyricIndex;
  public:
    CsCursorEditLyric(int lineIndex, int position, CsComposition &comp );

    // CsCursorEdit interface
  public:
    virtual void keyPress(int key, QChar ch, CsCursorEditPtr &ptr) override;
    virtual int  duration() const override;
    virtual void apply() override;
    virtual void cancel() override;
  };

#endif // CSCURSOREDITLYRIC_H
