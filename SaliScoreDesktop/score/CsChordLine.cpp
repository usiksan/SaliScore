#include "CsChordLine.h"
#include "../SvJson/SvJsonIO.h"


CsChordLine::CsChordLine()
  {

  }

void CsChordLine::jsonWrite(SvJsonWriter &js) const
  {
  js.jsonList<CsChord>( "ChordList", mList );
  }

void CsChordLine::jsonRead(SvJsonReader &js)
  {
  js.jsonList<CsChord>( "ChordList", mList );
  }
