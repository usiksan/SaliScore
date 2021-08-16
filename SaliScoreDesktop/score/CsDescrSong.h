#ifndef CSDESCRSONG_H
#define CSDESCRSONG_H

#include "CsJsonIO.h"

#include <QString>
#include <QMap>
#include <QList>

class CsDescrSong
  {
    QString mAuthor;
    QString mSinger;
    QString mName;
    int     mAuthorSongId;
    int     mVersion;
    bool    mIsPublic;
    bool    mMelodyPresent;
  public:
    CsDescrSong();

    QString songId() const { return mAuthor + QChar('_') + QString::number(mAuthorSongId,16); }

    QString directory() const { return QString::number(mAuthorSongId,32).right(1); }

    QString path() const;

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

    bool    isMelodyPresent() const { return mMelodyPresent; }
    void    isMelodyPresentSet( bool mp ) { mMelodyPresent = mp; }


    virtual void clear();

    //=================================================================
    //         Settings JSON io

    virtual void   jsonWrite( CsJsonWriter &js ) const;

    virtual void   jsonRead( CsJsonReader &js );

    static QString homeDir( const QString &subDir );
  };

using CsDescrSongList = QList<CsDescrSong>;

using CsDescrSongMap = QMap<QString,CsDescrSong>;

#endif // CSDESCRSONG_H
