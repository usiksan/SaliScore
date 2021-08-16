#include "CsSynthSettings.h"

CsSynthSettings::CsSynthSettings() :
  mVoice(1),
  mVoiceDual(0),
  mVoiceRight(0),
  mStyle(1),
  mTempo(120)
  {

  }




void CsSynthSettings::clear()
  {
  mVoice = 1;
  mVoiceDual = 0;
  mVoiceRight = 0;
  mStyle = 1;
  mTempo = 120;
  }




void CsSynthSettings::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonInt( "voice",      mVoice );
  js.jsonInt( "voiceDual",  mVoiceDual );
  js.jsonInt( "voiceRight", mVoiceRight );
  js.jsonInt( "style",      mStyle );
  js.jsonInt( "tempo",      mTempo );
  }



void CsSynthSettings::jsonRead(CsJsonReader &js)
  {
  js.jsonInt( "voice",      mVoice );
  js.jsonInt( "voiceDual",  mVoiceDual );
  js.jsonInt( "voiceRight", mVoiceRight );
  js.jsonInt( "style",      mStyle );
  js.jsonInt( "tempo",      mTempo );
  }
