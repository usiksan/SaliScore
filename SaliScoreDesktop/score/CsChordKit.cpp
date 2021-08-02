#include "CsChordKit.h"
#include "../SvJson/SvJsonIO.h"

CsChordKit::CsChordKit()
  {

  }

void CsChordKit::chordRename(const QString &prevPart, const QString &newPart)
  {
  mChordMap.insert( newPart, mChordMap.value(prevPart) );
  mChordMap.remove( prevPart );
  }


void CsChordKit::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonMap<CsChordLine>( "ChordMap", mChordMap );
  }


void CsChordKit::jsonRead(CsJsonReader &js)
  {
  js.jsonMap<CsChordLine>( "ChordMap", mChordMap );
  }
