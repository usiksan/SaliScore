#ifndef CSCHORDKIT_H
#define CSCHORDKIT_H

#include "CsConfig.h"
#include "CsChordLine.h"

#include <QMap>

class CsPainter;

class CsChordKit
  {
    QMap<QString,CsChordLine> mChordMap;
  public:
    CsChordKit();

    auto &chordMapConst() const { return mChordMap; }

    auto  chordLineGet( const QString &part ) const { return mChordMap.value(part); }

    void  chordLineSet( const QString &part, const CsChordLine &line ) { mChordMap.insert( part, line ); }

    auto  chordListGet( const QString &part ) const { return mChordMap.value(part).chordListConst(); }

    void  chordListSet( const QString &part, const CsChordList &list ) { mChordMap.insert( part, CsChordLine(list) ); }

    void  jsonWrite( CsJsonWriter &js ) const;

    void  jsonRead( CsJsonReader &js );
  };

#endif // CSCHORDKIT_H
