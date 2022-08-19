#include "CsConfig.h"
#include "CsSynthSfVoice.h"
#include "soundFont2/CsSoundFont.h"
#include "audioOut/CsSoundBufferIODevice.h"

#include <math.h>
#include <QDebug>

inline int delay( quint16 time ) {
  double tm = static_cast<qint16>(time);
  double sec = pow( 2.0, tm / 1200.0 );
  return static_cast<int>( sec * CS_SAMPLES_PER_SECOND );
  }


CsSynthSfVoice::CsSynthSfVoice(int voiceId, const QString voiceName, CsSoundFontPtr soundFont, int preset) :
  mVoiceName(voiceName),    //!< Voice name
  mSoundFontPtr(soundFont), //!< Sound font synth base on
  mVoiceId(voiceId),        //!< Id of current voice
  mVolume(128)
  {
  for( int i = 0; i < 128; i++ )
    mNotes[i].setNote(i);

  mSoundFontPtr->buildPreset( preset, [this] ( quint16 *generator, const SfSample &sam, qint16 *samples ) ->bool {
    //Note range
    int maxNote = generator[sfpiKeyRange] >> 8;
    int minNote = generator[sfpiKeyRange] & 0xff;

    //Samples bounds
    int startSample = static_cast<int>(sam.dwStart) + static_cast<qint16>(generator[sfpiStartAddressOffset]) + (static_cast<qint16>(generator[sfpiStartAddressCoarseOffset]) << 16);
    int endSample   = static_cast<int>(sam.dwEnd) + static_cast<qint16>(generator[sfpiEndAddressOffset]) + (static_cast<qint16>(generator[sfpiEndAddressCoarseOffset]) << 16);
    int startLoop   = static_cast<int>(sam.dwStartloop) + static_cast<qint16>(generator[sfpiStartLoopAddressOffset]) + (static_cast<qint16>(generator[sfpiStartLoopCoarseOffset]) << 16);
    int endLoop     = static_cast<int>(sam.dwEndloop) + static_cast<qint16>(generator[sfpiEndLoopAddressOffset]) + (static_cast<qint16>(generator[sfpiEndLoopCoarseOffset]) << 16);

    samples   += startSample;
    endSample -= startSample;
    startLoop -= startSample;
    endLoop   -= startSample;


    int delayVolEnv = delay( generator[sfpiDelayVolEnv] );
    int attackVolEnv = delay( generator[sfpiAttackVolEnv] );
    int holdVolEnv = delay( generator[sfpiHoldVolEnv] );
    int decayVolEnv = delay( generator[sfpiDecayVolEnv] );
    int releaseVolEnv = delay( generator[sfpiReleaseVolEnv] );

    //Build each note track from range
    while( minNote <= maxNote )
      if( !mNotes[minNote++].addTrack( generator, sam, samples, endSample, startLoop, endLoop,
                                       delayVolEnv, attackVolEnv, holdVolEnv, decayVolEnv, releaseVolEnv, &mVolume ) )
        return false;
    return true;
    });
  }




void CsSynthSfVoice::playNote(int note, int velo)
  {
  if( velo == 0 )
    mNotes[note].noteOff();
  else if( mNotes[note].noteOn(velo) )
    CsSoundBufferIODevice::soundBuffer()->addSound( mNotes + note );
  }
