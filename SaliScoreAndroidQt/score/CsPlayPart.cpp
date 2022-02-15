#include "CsPlayPart.h"

CsPlayPart::CsPlayPart() :
  mPartItem(nullptr)
  {

  }

bool CsPlayPart::compositionAppend(const QString &id)
  {
  if( mCompositionIdList.indexOf( id ) >= 0 )
    return false;
  mCompositionIdList.append( id );
  mTreeItemList.append( nullptr );
  return true;
  }



void CsPlayPart::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonString( "title", mTitle );
  js.jsonListString( "idList", mCompositionIdList );
  }




void CsPlayPart::jsonRead(CsJsonReader &js)
  {
  js.jsonString( "title", mTitle );
  js.jsonListString( "idList", mCompositionIdList );
  mTreeItemList.clear();
  mTreeItemList.reserve( mCompositionIdList.count() );
  for( int i = 0; i < mCompositionIdList.count(); i++ )
    mTreeItemList.append( nullptr );
  }

