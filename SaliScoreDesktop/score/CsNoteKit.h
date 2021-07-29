#ifndef CSNOTEKIT_H
#define CSNOTEKIT_H

#include "CsNoteLine.h"

#include <QMap>

class CsNoteKit
  {
    QMap<QString,CsNoteLine> mNoteMap;
  public:
    CsNoteKit();

    void jsonWrite( SvJsonWriter &js ) const;

    void jsonRead( SvJsonReader &js );
  };

#endif // CSNOTEKIT_H
