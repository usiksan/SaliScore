#include "CsDefinition.h"

CsDefinition::CsDefinition(const QString nm, const QString descr) :
  mName(nm),
  mDescription(descr),
  mVisible(true)
  {

  }

void CsDefinition::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonString( "Name", mName );
  js.jsonString( "Description", mDescription );
  js.jsonBool( "Visible", mVisible );
  }

void CsDefinition::jsonRead(CsJsonReader &js)
  {
  js.jsonString( "Name", mName );
  js.jsonString( "Description", mDescription );
  js.jsonBool( "Visible", mVisible );
  }
