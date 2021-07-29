#include "CsComposition.h"

#include "../SvJson/SvJsonIO.h"

CsComposition::CsComposition()
  {

  }

void CsComposition::jsonWrite(SvJsonWriter &js) const
  {
  js.jsonString( "Title", mTitle );
  js.jsonString( "Singer", mSinger );
  js.jsonString( "Composer", mComposer );
  js.jsonString( "Lyricist", mLyricist );
  js.jsonList<CsLine>( "LineList", mLineList );
  }



void CsComposition::jsonRead(SvJsonReader &js)
  {
  js.jsonString( "Title", mTitle );
  js.jsonString( "Singer", mSinger );
  js.jsonString( "Composer", mComposer );
  js.jsonString( "Lyricist", mLyricist );
  js.jsonList<CsLine>( "LineList", mLineList );
  }

