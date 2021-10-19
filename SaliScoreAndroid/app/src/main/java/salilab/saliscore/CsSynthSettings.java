package salilab.saliscore;

public class CsSynthSettings extends CsJsonWritable {
  private Integer  mVoice;
  private Integer  mVoiceDual;
  private Integer  mVoiceLeft;
  private Integer  mStyle;
  private Integer  mTempo;

  CsSynthSettings() {
    mVoice = 1;
    mVoiceDual = 0;
    mVoiceLeft = 0;
    mStyle = 1;
    mTempo = 120;
  }

  CsSynthSettings( CsSynthSettings ss ) {
    mVoice     = ss.mVoice.intValue();
    mVoiceDual = ss.mVoiceDual.intValue();
    mVoiceLeft = ss.mVoiceLeft.intValue();
    mStyle     = ss.mStyle.intValue();
    mTempo     = ss.mTempo.intValue();
  }

  String  voice() { return mVoice.toString(); }
  void    voiceSet( String voi ) {
    mVoice = Integer.decode(voi);
  }

  String  voiceDual() { return mVoiceDual.toString(); }
  void    voiceDualSet( String voi ) { mVoiceDual = Integer.decode(voi); }

  String  voiceLeft() { return mVoiceLeft.toString(); }
  void    voiceLeftSet( String voi ) { mVoiceLeft = Integer.decode(voi); }

  String  style() { return mStyle.toString(); }
  void    styleSet( String stl ) { mStyle = Integer.decode(stl); }

  String  tempo() { return mTempo.toString(); }
  void    tempoSet( String tmp ) { mTempo = Integer.decode(tmp); }

  void    clear() {
    mVoice = 1;
    mVoiceDual = 0;
    mVoiceLeft = 0;
    mStyle = 1;
    mTempo = 120;
  }

  //=================================================================
  //         Settings JSON io

  @Override
  void    jsonWrite( SvJsonWriter js ) throws Exception {
    js.jsonInt( "voice",      mVoice );
    js.jsonInt( "voiceDual",  mVoiceDual );
    js.jsonInt( "voiceLeft",  mVoiceLeft );
    js.jsonInt( "style",      mStyle );
    js.jsonInt( "tempo",      mTempo );
  }

  @Override
  void    jsonRead( SvJsonReader js ) throws Exception {
    js.jsonInt( "voice",      mVoice );
    js.jsonInt( "voiceDual",  mVoiceDual );
    js.jsonInt( "voiceLeft",  mVoiceLeft );
    js.jsonInt( "style",      mStyle );
    js.jsonInt( "tempo",      mTempo );
  }

  @Override
  CsJsonWritable copy() {
    return new CsSynthSettings( this );
  }
}
