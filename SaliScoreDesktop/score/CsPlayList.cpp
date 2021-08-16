#include "CsPlayList.h"

CsPlayList::CsPlayList()
  {

  }




void CsPlayList::partAppend(const QString &partName)
  {
  for( int i = 0; i < mPartList.count(); i++ )
    if( mPartList.at(i).title() == partName ) return;
  CsPlayPart part;
  part.titleSet( partName );
  mPartList.append( part );
  }




void CsPlayList::compositionSet(CsComposition &comp)
  {
  mCompositionsMap.insert( comp.header().songId(), CsCompositionSettings(comp) );
  mDirty = true;
  }




void CsPlayList::jsonWrite( CsJsonWriter &js ) const
  {
  js.jsonInt( "aversion", mVersion );
  js.jsonList<CsPlayPart>( "partList", mPartList );
  js.jsonMap<CsCompositionSettings>( "compositionMap", mCompositionsMap );
  }




void CsPlayList::jsonRead(CsJsonReader &js)
  {
  js.jsonInt( "aversion", mVersion );
  js.jsonList<CsPlayPart>( "partList", mPartList );
  js.jsonMap<CsCompositionSettings>( "compositionMap", mCompositionsMap );
  }
