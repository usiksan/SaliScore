#ifndef CSNOTEKIT_H
#define CSNOTEKIT_H

#include "CsNoteLine.h"

#include <QMap>

class CsNoteKit
  {
    QMap<QString,CsNoteLine> mNoteKit;
  public:
    CsNoteKit();
  };

#endif // CSNOTEKIT_H
