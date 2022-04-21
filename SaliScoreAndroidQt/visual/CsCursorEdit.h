#ifndef CSCURSOREDIT_H
#define CSCURSOREDIT_H

#include "score/CsCursor.h"
#include "score/CsLyricSymbol.h"

class CsComposition;

class CsCursorEdit : public CsCursor
  {
  protected:
    CsComposition &mComposition;
    bool           mShift;
    bool           mControl;
    bool           mIsEdit;      //!< True when cursor edit already existing element, and false when newly created
  public:
    using CsCursorEditPtr = CsCursorEdit*;

    CsCursorEdit( CsComposition &comp );
    virtual ~CsCursorEdit() {}

    virtual void         keyPress( int key, QChar ch, CsCursorEditPtr &ptr );

    virtual void         keyRelease( int key, QChar ch );

    virtual QString      getStr() const { return QString{}; }

    virtual CsLyricLine  getLyric() const { return CsLyricLine{}; }

    virtual int          duration() const { return 0; }

    virtual int          noteWhite() const { return 0; }

    virtual bool         noteDies() const { return false; }

    virtual bool         isSelectionPresent() const { return false; }

    virtual int          selStart() const { return 0; }

    virtual int          selStop() const { return 0; }

    virtual int          charPosition() const { return 0; }

    virtual void         cancel() {}

    virtual void         apply() {}

    virtual void         clipboardCut() {}

    virtual void         clipboardPaste() {}

    virtual void         clipboardCopy() {}

    static CsCursorEdit *build(CsCursor &src, CsComposition &comp );

    static CsCursorEdit *build(CsClass cellClass, int lineIndex, int position, const QString partName, CsComposition &comp );
  };

#endif // CSCURSOREDIT_H
