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

    void                  partAppend( const QString &partName );

    void                  partRename( const QString &prevName, const QString &newName );

    void                  partRemove( const QString &partName );



    CsCompositionSettings composition( const QString &id ) const { return mCompositionsMap.value(id); }

    void                  compositionSet( CsComposition &comp );



    void jsonWrite( SvJsonWriter &writer ) const;

    void jsonRead( SvJsonReaderExtInt &reader );
  };

#endif // CSPLAYLIST_H
