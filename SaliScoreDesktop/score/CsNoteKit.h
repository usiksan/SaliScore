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

    auto  noteListGet( const QString &part ) const { return mNoteMap.value(part).noteListGet(); }

    void  noteListSet( const QString &part, const CsNoteList &list ) { mNoteMap[part].noteListSet(list); }

    void jsonWrite( CsJsonWriter &js ) const;

    void jsonRead( CsJsonReader &js );
  };

#endif // CSNOTEKIT_H
