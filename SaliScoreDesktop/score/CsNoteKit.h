#ifndef CSNOTEKIT_H
#define CSNOTEKIT_H

#include "CsNoteLine.h"

#include <QMap>

class CsNoteKit
  {
    QMap<QString,CsNoteLine> mNoteMap;
  public:
    CsNoteKit();

    auto &noteMapConst() const { return mNoteMap; }

    void jsonWrite( CsJsonWriter &js ) const;

    void jsonRead( CsJsonReader &js );
  };

#endif // CSNOTEKIT_H
