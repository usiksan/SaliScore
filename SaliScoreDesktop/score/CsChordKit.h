/*
Project "SaliScore Score music edit, view and tutorial programm"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Chord line variations for different variants

  All variations are organized into map, each element of them
  represents single variant
*/
#ifndef CSCHORDKIT_H
#define CSCHORDKIT_H

#include "CsConfig.h"
#include "CsChordLine.h"

#include <QMap>

class CsPainter;

class CsChordKit
  {
    QMap<QString,CsChordLine> mChordMap; //!< Chord line variations map
  public:
    CsChordKit();

    auto &chordMapConst() const { return mChordMap; }

    auto  chordLineGet( const QString &part ) const { return mChordMap.value(part); }

    void  chordLineSet( const QString &part, const CsChordLine &line ) { mChordMap.insert( part, line ); }

    auto  chordListGet( const QString &part ) const { return mChordMap.value(part).chordListConst(); }

    void  chordListSet( const QString &part, const CsChordList &list ) { mChordMap.insert( part, CsChordLine(list) ); }

    void  chordRename( const QString &prevPart, const QString &newPart );

    void  chordRemove( const QString &part ) { mChordMap.remove( part ); }

    void  jsonWrite( CsJsonWriter &js ) const;

    void  jsonRead( CsJsonReader &js );
  };

#endif // CSCHORDKIT_H
