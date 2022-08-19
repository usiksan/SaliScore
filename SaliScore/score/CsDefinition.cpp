#include "CsDefinition.h"

CsDefinition::CsDefinition(int csClass, const QString nm, const QString descr, const QString inst, int clef ) :
  mClass(csClass),
  mName(nm),
  mInstrument(inst),
  mDescription(descr),
  mClef(clef),
  mVisible(true),
  mSynthes(true),
  mTrain(true)
  {

  }

void CsDefinition::json(SvJsonWriter &js) const
  {
  js.jsonInt( "class", mClass );
  js.jsonString( "name", mName );
  js.jsonString( "description", mDescription );
  js.jsonString( "instrument", mInstrument );
  js.jsonInt( "clef", mClef );
  js.jsonBool( "visible", mVisible );
  js.jsonBool( "synthes", mSynthes );
  js.jsonBool( "train", mTrain );
  }


void CsDefinition::json(SvJsonReader &js)
  {
  js.jsonInt( "class", mClass );
  js.jsonString( "name", mName );
  js.jsonString( "description", mDescription );
  js.jsonString( "instrument", mInstrument );
  js.jsonInt( "clef", mClef );
  js.jsonBool( "visible", mVisible );
  js.jsonBool( "synthes", mSynthes );
  js.jsonBool( "train", mTrain );
  }



