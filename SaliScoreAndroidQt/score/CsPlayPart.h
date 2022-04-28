#ifndef CSPLAYPART_H
#define CSPLAYPART_H

#include "CsJsonIO.h"

#include <QString>
#include <QList>
#include <QTreeWidgetItem>


class CsPlayPart
  {
    QString            mTitle;
    QStringList        mCompositionIdList;
  public:
    CsPlayPart();

    QString     title() const { return mTitle; }

    void        titleSet( const QString &tit ) { mTitle = tit; }

    QStringList compositionIdList() const { return mCompositionIdList; }

    int         compositionCount() const { return mCompositionIdList.count(); }

    QString     compositionId( int index ) const { return mCompositionIdList.at(index); }

    bool        compositionAppend( const QString &id );

    void        compositionRemoveById( const QString &id ) { mCompositionIdList.removeAll( id ); }

    void        compositionRemove( int index ) { mCompositionIdList.removeAt(index); }

    void        jsonWrite( CsJsonWriter &js ) const;

    void        jsonRead( CsJsonReader &js );

  };

using CsPlayPartList = QList<CsPlayPart>;

#endif // CSPLAYPART_H
