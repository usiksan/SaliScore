#ifndef CSDEFINITION_H
#define CSDEFINITION_H

#include "CsConfig.h"
#include "CsJsonIO.h"
#include "CsNoteChord.h"

#include <QString>
#include <QList>

struct CsDefinition
  {
    QString mName;
    QString mInstrument;
    QString mDescription;
    int     mClef;
    bool    mVisible;

    CsDefinition( const QString nm = QString{}, const QString descr = QString{}, const QString inst = QString{}, int mClef = noteC );

    bool visibleToggle() { mVisible = !mVisible; return mVisible; }

    void jsonWrite( CsJsonWriter &js ) const;

    void jsonRead( CsJsonReader &js );

  };

using CsDefList = QList<CsDefinition>;

#endif // CSDEFINITION_H
