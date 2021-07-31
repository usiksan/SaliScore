#include "CsComposition.h"

#include "../SvJson/SvJsonIO.h"

CsComposition::CsComposition()
  {

  }

void CsComposition::lineInsert(int index, const CsLine &line)
  {
  if( index < 0 )
    mLineList.append( line );
  else
    mLineList.insert( index, line );
  }


void CsComposition::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonString( "CompTitle", mTitle );
  js.jsonString( "CompSinger", mSinger );
  js.jsonString( "Composer", mComposer );
  js.jsonString( "CompLyricist", mLyricist );
  js.jsonList<CsLine>( "LineList", mLineList );
  }



void CsComposition::jsonRead(CsJsonReader &js)
  {
  js.jsonString( "CompTitle", mTitle );
  js.jsonString( "CompSinger", mSinger );
  js.jsonString( "Composer", mComposer );
  js.jsonString( "CompLyricist", mLyricist );
  js.jsonList<CsLine>( "LineList", mLineList );
  }

