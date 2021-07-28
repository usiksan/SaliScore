#ifndef CSNOTE_H
#define CSNOTE_H

#include "CsConfig.h"
#include "CsPosition.h"

#include <QList>

class CsNote : public CsPosition
  {
    int mDuration;
  public:
    CsNote();
  };

using CsNoteList = QList<CsNote>;

#endif // CSNOTE_H
