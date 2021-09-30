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
#include "CsConfig.h"
#include "CsDescrSong.h"
#include "SdLib/SdTime2x.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>

CsDescrSong::CsDescrSong() :
  mAuthor(),
  mSinger(),
  mName(),
  mAuthorSongId(0),
  mVersion(0),
  mIsPublic(false)
  {

  }





QString CsDescrSong::path() const
  {
  return homeDir( directory() ) + songId() + QStringLiteral(CS_BASE_EXTENSION);
  }




void CsDescrSong::authorSet(const QString &auth)
  {
  mAuthor = auth;
  mAuthorSongId = mVersion = SdTime2x::current();
  }




void CsDescrSong::versionUpdate()
  {
  mVersion = SdTime2x::current();
  }




int CsDescrSong::versionFromFile() const
  {
  //Try extract version from file
  QFile file( path() );
  if( file.open( QIODevice::ReadOnly ) ) {
    QByteArray content( file.readAll() );
    QJsonObject obj = QJsonDocument::fromJson( content ).object();
    int version = obj.value(QStringLiteral(CS_BASE_VERSION_KEY)).toInt();
    SvJsonReaderExtInt js( obj, &version );
    CsDescrSong comp;
    comp.jsonRead( js );
    return comp.version();
    }
  //File not exist. We return minimal version
  return 1;
  }




void CsDescrSong::clear()
  {
  mAuthor.clear();
  mSinger.clear();
  mName.clear();
  mAuthorSongId = mVersion = 0;
  mIsPublic = false;
  }




void CsDescrSong::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonString( "Author", mAuthor );
  js.jsonString( "Singer", mSinger );
  js.jsonString( "Name",   mName );
  js.jsonInt( "AuthorSongId", mAuthorSongId );
  js.jsonInt( "Version",      mVersion );
  js.jsonBool( "IsPublic",    mIsPublic );
  }




void CsDescrSong::jsonRead(CsJsonReader &js)
  {
  js.jsonString( "Author", mAuthor );
  js.jsonString( "Singer", mSinger );
  js.jsonString( "Name",   mName );
  js.jsonInt( "AuthorSongId", mAuthorSongId );
  js.jsonInt( "Version",      mVersion );
  js.jsonBool( "IsPublic",    mIsPublic );
  }





QString CsDescrSong::homeDir( const QString &subDir )
  {
  QString dirPath = QDir::homePath() + QStringLiteral(CS_DATA_DIRECTORY);
  if( !subDir.isEmpty() )
   dirPath.append( subDir );
  QDir dir;
  dir.mkpath( dirPath );
  return dirPath  + QStringLiteral("/");
  }
