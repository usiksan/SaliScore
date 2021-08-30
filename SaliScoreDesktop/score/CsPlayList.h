#ifndef CSPLAYLIST_H
#define CSPLAYLIST_H

#include "CsPlayPart.h"
#include "CsCompositionSettings.h"

#include <QMap>
#include <QTreeWidgetItem>
#include <functional>

using CsCompositionMap = QMap<QString,CsCompositionSettings>;

class CsPlayList
  {
    int              mVersion;
    CsPlayPartList   mPartList;

    CsCompositionMap mCompositionsMap;

    bool             mDirty;
  public:
    CsPlayList();

    bool             dirty() const { return mDirty; }

    void             dirtyReset();

    int              version() const { return mVersion; }

    int              partCount() const { return mPartList.count(); }

    QString          partTitle( int i ) const { return mPartList.at(i).title(); }

    void             partTitleSet( int i, const QString &tit );

    QTreeWidgetItem *partItem( int i ) const { return mPartList.at(i).mPartItem; }

    void             partItemSet( int i, QTreeWidgetItem *item ) { mPartList[i].mPartItem = item; }

    bool             partAppend( const QString &partName );

    int              partCompositionCount( int i ) const { return mPartList.at(i).compositionCount(); }

    QString          partCompositionId( int partIndex, int compositionIndex ) const { return mPartList.at(partIndex).compositionId(compositionIndex); }

    QTreeWidgetItem *partCompositionItem( int partIndex, int compositionIndex ) const { return mPartList.at(partIndex).mTreeItemList.at(compositionIndex); }

    void             partCompositionItemSet( int partIndex, int compositionIndex, QTreeWidgetItem *item );

    bool             partCompositionAppend( int partIndex, const QString &id );

//    void             forEachPart( std::function<void ( *)> fun1 )


    //void             partRename( const QString &prevName, const QString &newName );

    //void             partRemove( const QString &partName );



    CsCompositionSettings composition( const QString &id ) const { return mCompositionsMap.value(id); }

    void                  compositionSet( const CsComposition &comp );

    bool                  contains( const QString &id ) const { return mCompositionsMap.contains( id ); }

    QString               compositionUpload(int lasttime , const QString author);



    void                  jsonWrite(CsJsonWriter &js ) const;

    void                  jsonRead( CsJsonReader &js );


    QByteArray            toByteArray() const;

    void                  fromByteArray( const QByteArray &ar );


    void                  load();

    void                  save();
  };

#endif // CSPLAYLIST_H
