#include "CsPlayPart.h"

CsPlayPart::CsPlayPart()
  {

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

