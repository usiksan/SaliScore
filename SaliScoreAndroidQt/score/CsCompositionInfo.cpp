#include "CsCompositionInfo.h"
#include "CsComposition.h"

CsCompositionInfo::CsCompositionInfo()
  {

  }

CsCompositionInfo::CsCompositionInfo(const CsComposition &comp) :
  mName(comp.attributeGet( CS_ATTR_NAME )),
  mAuthor(comp.attributeGet( CS_ATTR_AUTHOR )),
  mSinger(comp.attributeGet( CS_ATTR_SINGER )),
  mVersion(comp.attributeGet( CS_ATTR_VERSION ))
  {
  }

void CsCompositionInfo::jsonRead( SvJsonReader &js )
  {
  js.jsonString( "Name", mName );
  js.jsonString( "Author", mAuthor );
  js.jsonString( "Singer", mSinger );
  js.jsonString( "Version", mVersion );
  }

void CsCompositionInfo::jsonWrite( SvJsonWriter &js ) const
  {
  js.jsonString( "Name", mName );
  js.jsonString( "Author", mAuthor );
  js.jsonString( "Singer", mSinger );
  js.jsonString( "Version", mVersion );
  }
