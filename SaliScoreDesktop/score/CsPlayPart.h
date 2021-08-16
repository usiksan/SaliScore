#ifndef CSPLAYPART_H
#define CSPLAYPART_H

#include "CsJsonIO.h"

#include <QString>
#include <QList>
#include <QTreeWidgetItem>

using QTreeWidgetItemPtr = QTreeWidgetItem*;

using CsTreeItemList = QList<QTreeWidgetItemPtr>;

class CsPlayPart
  {
    QString            mTitle;
    QStringList        mCompositionIdList;
  public:
    QTreeWidgetItemPtr mPartItem;
    CsTreeItemList     mTreeItemList;

    CsPlayPart();

    QString     title() const { return mTitle; }

    void        titleSet( const QString &tit ) { mTitle = tit; }

    QStringList compositionIdList() const { return mCompositionIdList; }

    int         compositionCount() const { return mCompositionIdList.count(); }

    QString     compositionId( int index ) const { return mCompositionIdList.at(index); }

    bool        compositionAppend( const QString &id );

    //void        compositionRemove( const QString &id ) { mCompositionIdList.removeAll( id ); }

    //void        compositionRename( int index, const QString &id ) { mCompositionIdList[index] = id; }

    void        jsonWrite( CsJsonWriter &js ) const;

    void        jsonRead( CsJsonReader &js );

  };

using CsPlayPartList = QList<CsPlayPart>;

#endif // CSPLAYPART_H
