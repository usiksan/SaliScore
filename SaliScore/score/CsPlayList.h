#ifndef CSPLAYLIST_H
#define CSPLAYLIST_H

#include "CsPlayPart.h"
#include "CsCompositionInfo.h"
#include "SvLib/SvSignal.h"

#include <QMap>
#include <QTreeWidgetItem>
#include <functional>


class CsPlayList
  {
    int              mVersion;          //!< Version (time of last changes)
    CsPlayPartList   mPartList;         //!< Part list

    CsCompositionMap mCompositionsMap;  //!< Map of composition Id and accosiated composition settings

    bool             mDirty;            //!< When true list need to be saved

    CsPlayList();
  public:
    static CsPlayList    *pl();

    bool                  dirty() const { return mDirty; }

    void                  dirtyReset();

    int                   version() const { return mVersion; }

    //====================================================================================
    //           Part list level
    int                   partCount() const { return mPartList.count() + 1; }

    QString               partTitle( int i ) const;

    void                  partTitleSet( int i, const QString &tit );

    bool                  partAppend( const QString &partName );

    void                  partDelete( int i );


    //====================================================================================
    //           Single part level
    int                   partCompositionCount(int partIndex ) const;

    QString               partCompositionId( int partIndex, int compositionIndex ) const;

    QString               partCompositionName( int partIndex, int compositionIndex ) const { return mCompositionsMap.value(partCompositionId(partIndex,compositionIndex)).name(); }

    CsCompositionInfo     partCompositionInfo( int partIndex, int compositionIndex ) const { return mCompositionsMap.value(partCompositionId(partIndex,compositionIndex)); }

    bool                  partCompositionAppend( int partIndex, const QString &id );

    void                  partCompositionRemove(int partIndex, int compositionIndex );


    //====================================================================================
    //           Composition map
    CsCompositionInfo     composition( const QString &id ) const { return mCompositionsMap.value(id); }

    void                  compositionSet( const CsComposition &comp );

    //!
    //! \brief compositionVersion Return file version of composition
    //! \param id                 Ident of composition which version need to be returned
    //! \return                   File version of composition
    //!
    int                   compositionVersion( const QString &id ) const { return mCompositionsMap.value(id).version().toInt(); }

    bool                  contains( const QString &id ) const { return mCompositionsMap.contains( id ); }

    //!
    //! \brief compositionList Returns full file list as list of compositionId
    //! \return                List of compositionId
    //!
    QStringList           compositionList() const { return mCompositionsMap.keys(); }

    //!
    //! \brief compositionPath Full path to composition
    //! \param songId          Ident of composition
    //! \return                Full path to composition on local storage
    //!
    QString               compositionPath( const QString &id ) const;



    void                  jsonWrite(CsJsonWriter &js ) const;

    void                  jsonRead( CsJsonReader &js );


    QByteArray            toByteArray() const;

    bool                  fromByteArray( const QByteArray &ar );


    void                  load();

    void                  save();

    SvSignal<void()>      mChanged;
  private:
    void                  signalDirty();

    static QString        fileName();

  };

#endif // CSPLAYLIST_H
