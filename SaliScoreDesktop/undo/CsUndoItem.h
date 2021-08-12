#ifndef CSUNDOITEM_H
#define CSUNDOITEM_H

#include <QList>

class CsUndoItem
  {
  public:
    CsUndoItem();
    virtual ~CsUndoItem() {}

    virtual bool isBegin() const { return false; }

    virtual void undo() = 0;

    virtual void redo() = 0;
  };

using CsUndoItemPtr = CsUndoItem*;

using CsUndoItemPtrList = QList<CsUndoItemPtr>;

#endif // CSUNDOITEM_H
