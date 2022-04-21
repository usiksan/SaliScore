#include "CsDefinition.h"

CsDefinition::CsDefinition(const QString nm, const QString descr, const QString inst, int clef ) :
  mName(nm),
  mInstrument(inst),
  mDescription(descr),
  mClef(clef),
  mVisible(true)
  {

  }

void CsDefinition::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonString( "Name", mName );
  js.jsonString( "Description", mDescription );
  js.jsonString( "Instrument", mInstrument );
  js.jsonInt( "Clef", mClef );
  }

void CsDefinition::jsonRead(CsJsonReader &js)
  {
  js.jsonString( "Name", mName );
  js.jsonString( "Description", mDescription );
  js.jsonString( "Instrument", mInstrument );
  js.jsonInt( "Clef", mClef );
  mVisible = true;
  }
