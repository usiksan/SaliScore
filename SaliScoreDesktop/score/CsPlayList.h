#ifndef CSPLAYLIST_H
#define CSPLAYLIST_H

#include "CsPlayPart.h"
#include "CsCompositionSettings.h"

#include <QMap>

using CsCompositionMap = QMap<QString,CsCompositionSettings>;

class CsPlayList
  {
    int              mVersion;
    CsPlayPartList   mPartList;

    CsCompositionMap mCompositionsMap;

    bool             mDirty;
  public:
    CsPlayList();

    const CsPlayPartList &partList() const { return mPartList; }

    int                   partCount() const { return mPartList.count(); }

    QString               partTitle( int i ) const { return mPartList.at(i).title(); }

    int                   partCompositionCount( int i ) const { return mPartList.at(i).compositionCount(); }

    QString               partCompositionId( int partIndex, int compositionIndex ) const { return mPartList.at(partIndex).compositionId(compositionIndex); }

    void                  partAppend( const QString &partName );

    void                  partRename( const QString &prevName, const QString &newName );

    void                  partRemove( const QString &partName );



    CsCompositionSettings composition( const QString &id ) const { return mCompositionsMap.value(id); }

    void                  compositionSet( CsComposition &comp );



    void                  jsonWrite(CsJsonWriter &js ) const;

    void                  jsonRead( CsJsonReader &js );
  };

#endif // CSPLAYLIST_H
