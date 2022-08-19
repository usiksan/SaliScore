#include "CsDefinition.h"

CsDefinition::CsDefinition(const QString nm, const QString descr, const QString inst, int clef ) :
  mName(nm),
  mInstrument(inst),
  mDescription(descr),
  mClef(clef),
  mVisible(true),
  mSynthes(true),
  mTrain(true)
  {

  }

void CsDefinition::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonString( "Name", mName );
  js.jsonString( "Description", mDescription );
  js.jsonString( "Instrument", mInstrument );
  js.jsonInt( "Clef", mClef );
  js.jsonBool( "visible", mVisible );
  js.jsonBool( "synthes", mSynthes );
  js.jsonBool( "train", mTrain );
  }

void CsDefinition::jsonRead(CsJsonReader &js)
  {
  js.jsonString( "Name", mName );
  js.jsonString( "Description", mDescription );
  js.jsonString( "Instrument", mInstrument );
  js.jsonInt( "Clef", mClef );
  if( js.property()->mStatus ) {
    //Perform read with all status
//    js.jsonBool( "visible", mVisible );
//    js.jsonBool( "synthes", mSynthes );
//    js.jsonBool( "train", mTrain );
    }
  else {
    mVisible = true;
    mSynthes = true;
    mTrain   = true;
    }
  }
