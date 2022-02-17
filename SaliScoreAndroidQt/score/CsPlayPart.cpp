#include "CsPlayPart.h"

CsPlayPart::CsPlayPart()
  {

  }

bool CsPlayPart::compositionAppend(const QString &id)
  {
  if( mCompositionIdList.indexOf( id ) >= 0 )
    return false;
  mCompositionIdList.append( id );
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
  }

