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
    int              mVersion;         //!< Version (time of last changes)
    CsPlayPartList   mPartList;        //!< Part list

    CsCompositionMap mCompositionsMap; //!< Map of composition Id and accosiated composition settings

    bool             mDirty;           //!< When true list need to be saved
  public:
    CsPlayList();

    bool                  dirty() const { return mDirty; }

    void                  dirtyReset();

    int                   version() const { return mVersion; }

    //====================================================================================
    //           Part list level
    int                   partCount() const { return mPartList.count(); }

    QString               partTitle( int i ) const { return mPartList.at(i).title(); }

    void                  partTitleSet( int i, const QString &tit );

    bool                  partAppend( const QString &partName );

    void                  partDelete( int i );


    //====================================================================================
    //           Single part level
    int                   partCompositionCount( int i ) const { return mPartList.at(i).compositionCount(); }

    QString               partCompositionId( int partIndex, int compositionIndex ) const { return mPartList.at(partIndex).compositionId(compositionIndex); }

    QString               partCompositionName( int partIndex, int compositionIndex ) const { return mCompositionsMap.value(partCompositionId(partIndex,compositionIndex)).name(); }

    bool                  partCompositionAppend( int partIndex, const QString &id );

    void                  partCompositionRemove(int partIndex, int compositionIndex );


    //====================================================================================
    //           Composition map
    CsCompositionSettings composition( const QString &id ) const { return mCompositionsMap.value(id); }

    //QString               compositionName( const QString &id ) const { return mCompositionsMap.value(id).name(); }

    void                  compositionSet( const CsComposition &comp );

    bool                  contains( const QString &id ) const { return mCompositionsMap.contains( id ); }

    QStringList           compositionList() const { return mCompositionsMap.keys(); }



    void                  jsonWrite(CsJsonWriter &js ) const;

    void                  jsonRead( CsJsonReader &js );


    QByteArray            toByteArray() const;

    void                  fromByteArray( const QByteArray &ar );


    void                  load();

    void                  save();

  private:
    void                  garbageCollection();

    static QString        fileName();
  };

#endif // CSPLAYLIST_H
