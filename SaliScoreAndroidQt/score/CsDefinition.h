#ifndef CSDEFINITION_H
#define CSDEFINITION_H

#include "config.h"
#include "CsJsonIO.h"
#include "CsNoteChord.h"

#include <QString>
#include <QList>

struct CsDefinition
  {
    QString mName;
    QString mDescription;
    bool    mVisible;

    CsDefinition( const QString nm = QString{}, const QString descr = QString{} );

    bool visibleToggle() { return mVisible = !mVisible; }

    void jsonWrite( CsJsonWriter &js ) const;

    void jsonRead( CsJsonReader &js );

  };

using CsDefList = QList<CsDefinition>;

#endif // CSDEFINITION_H
