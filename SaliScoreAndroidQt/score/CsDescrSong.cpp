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
#include "config.h"
#include "CsDescrSong.h"
#include "SdLib/SdTime2x.h"

CsDescrSong::CsDescrSong() :
  mAuthor(),
  mSinger(),
  mName(),
  mAuthorSongId(0),
  mVersion(0),
  mIsPublic(false)
  {

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


