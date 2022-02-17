/*
Project "SaliScore Score music edit, view and tutorial programm"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Song description. Contains some field with common information about composition
*/
#ifndef CSDESCRSONG_H
#define CSDESCRSONG_H

#include "CsJsonIO.h"

#include <QString>
#include <QMap>
#include <QList>

class CsDescrSong
  {
    QString mAuthor;         //!< Author who creates composition score
    QString mSinger;         //!< Singer is common singer of composition
    QString mName;           //!< Composition name
    int     mAuthorSongId;   //!< Unical id of composition for this author
    int     mVersion;        //!< Version of composition score (time of last edit)
    bool    mIsPublic;       //!< When true, then author make it available for public, otherway its private for author
  public:
    CsDescrSong();
    virtual ~CsDescrSong() {}

    QString songId() const { return mAuthor + QChar('_') + QString::number(mAuthorSongId,16); }

    QString author() const { return mAuthor; }
    void    authorSet( const QString &auth );

    QString singer() const { return mSinger; }
    void    singerSet( const QString &sing ) { mSinger = sing; }

    QString name() const { return mName; }
    void    nameSet( const QString &nam ) { mName = nam; }

    int     version() const { return mVersion; }
    void    versionUpdate();

    bool    isPublic() const { return mIsPublic; }
    void    isPublicSet( bool pub ) { mIsPublic = pub; }

    virtual void clear();

    //=================================================================
    //         Settings JSON io

    virtual void   jsonWrite( CsJsonWriter &js ) const;

    virtual void   jsonRead( CsJsonReader &js );
  };

using CsDescrSongList = QList<CsDescrSong>;

using CsDescrSongMap = QMap<QString,CsDescrSong>;

#endif // CSDESCRSONG_H
