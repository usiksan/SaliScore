#include "CsSynthSfVoiceInfo.h"
#include "CsSynthSfVoice.h"
#include "CsSynthVoiceId.h"
#include "soundFont2/CsSoundFont.h"
#include "soundFont2/CsSoundFontMap.h"

CsSynthSfVoiceInfo::CsSynthSfVoiceInfo()
  {
  }




void CsSynthSfVoiceInfo::jsonWrite(SvJsonWriter &js) const
  {
  js.jsonString( "font", mFontName );
  js.jsonInt( "preset", mPreset );
  js.jsonString( "name", mName );
  js.jsonString( "class", mTembrClass );
  js.jsonInt( "bank", mMidiBank );
  js.jsonInt( "program", mMidiProgram );
  }



void CsSynthSfVoiceInfo::jsonRead(SvJsonReader &js)
  {
  js.jsonString( "font", mFontName );
  js.jsonInt( "preset", mPreset );
  js.jsonString( "name", mName );
  js.jsonString( "class", mTembrClass );
  js.jsonInt( "bank", mMidiBank );
  js.jsonInt( "program", mMidiProgram );

  CsSoundFontPtr soundFont = CsSoundFontMap::font( mFontName );
  if( !soundFont.isNull() ) {
    //Sound font present
    mVoice.reset( new CsSynthSfVoice( csVoiceId( mMidiBank, mMidiProgram ), mName, soundFont, mPreset ) );
    }

  }
