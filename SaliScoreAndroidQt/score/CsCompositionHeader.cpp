#include "CsCompositionHeader.h"

CsCompositionHeader::CsCompositionHeader()
  {

  }




void CsCompositionHeader::clear()
  {
  CsDescrSong::clear();
  mSettings.clear();
  }




void CsCompositionHeader::jsonWrite(CsJsonWriter &js) const
  {
  CsDescrSong::jsonWrite( js );
  mSettings.jsonWrite( js );
  js.jsonObject<CsTrainInterval>( "fragment0", mFragments[0] );
  js.jsonObject<CsTrainInterval>( "fragment1", mFragments[1] );
  }




void CsCompositionHeader::jsonRead(CsJsonReader &js)
  {
  CsDescrSong::jsonRead( js );
  mSettings.jsonRead( js );
  js.jsonObject<CsTrainInterval>( "fragment0", mFragments[0] );
  js.jsonObject<CsTrainInterval>( "fragment1", mFragments[1] );
  }
