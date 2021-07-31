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



QStringList CsComposition::visibleList(const CsDefList &src)
  {
  QStringList list;
  for( const auto &def : src )
    if( def.mVisible )
      list.append( def.mName );
  return list;
  }




int CsComposition::defListIndex(const CsDefList &list, const QString &key)
  {
  //Prepare index for finding
  int i = 0;

  //Scan list and test for equal name
  for( const auto &def : list )
    if( def.mName == key )
      return i;
    else
      i++;

  //Key not found in list
  return -1;
  }

