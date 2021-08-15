#include "CsDescrSong.h"
#include "SdLib/SdTime2x.h"


CsDescrSong::CsDescrSong() :
  mAuthor(),
  mSinger(),
  mName(),
  mAuthorSongId(0),
  mVersion(0),
  mIsPublic(false),
  mMelodyPresent(false)
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

void CsDescrSong::jsonWrite(CsJsonWriter &js) const
  {

  }

void CsDescrSong::jsonRead(CsJsonReader &js)
  {

  }
