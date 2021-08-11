#ifndef CSCURSOREDIT_H
#define CSCURSOREDIT_H

#include "score/CsCursor.h"

class CsComposition;

class CsCursorEdit : public CsCursor
  {
  protected:
    CsComposition &mComposition;
    bool           mShift;
    bool           mControl;
  public:
    using CsCursorEditPtr = CsCursorEdit*;

    CsCursorEdit( CsComposition &comp );
    virtual ~CsCursorEdit() {}

    virtual void         keyPress( int key, QChar ch, CsCursorEditPtr &ptr );

    virtual void         keyRelease( int key, QChar ch );

    virtual QString      getStr() const { return QString{}; }

    virtual bool         isSelectionPresent() const { return false; }

    virtual int          selStart() const { return 0; }

    virtual int          selStop() const { return 0; }

    virtual int          charPosition() const { return 0; }

    virtual void         cancel() {}

    virtual void         apply() {}

    virtual void         clipboardCut() {}

    virtual void         clipboardPaste() {}

    virtual void         clipboardCopy() {}

    static CsCursorEdit *build( CsCursor &src, CsComposition &comp );
  };

#endif // CSCURSOREDIT_H