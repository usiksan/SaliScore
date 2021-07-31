#include "CsChordLine.h"


CsChordLine::CsChordLine()
  {

  }

void CsChordLine::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonList<CsChord>( "ChordList", mList );
  }

void CsChordLine::jsonRead(CsJsonReader &js)
  {
  js.jsonList<CsChord>( "ChordList", mList );
  }
