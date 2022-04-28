#include "CsSynthSfTrack.h"

#include <math.h>
#include <QDebug>

int CsSynthSfTrack::mAttenuator[1024];

CsSynthSfTrack::CsSynthSfTrack() :
  mSamples(nullptr),     //Samples vector
  mSampleEnd(0),
  mSampleLoopStart(0),
  mSampleLoopEnd(0),
  mExitLoop(0),        //Walk through end loop, else looping
  //Oscillator
  mSampleIndex(0),     //Current sample index
  mSampleSubIndex(0),  //Fractional part of sample index in 1/65536
  mSampleStep(0),      //Step of sample to provide a given sample rate. In 1/65536
  //Amplitude envelope modulator
  mVolumeInitial(0),      //In santiBell note default attenuation
  mVolume(0),             //Normal signal volume without regulation. In 1/32768
  mVolumePhase(vpStop),
  mVolumeTick(0),
  mAttenuation(0),        //Current attenuation level. At phase attack - linear multer in 1/32768 of volume
  mAttenuationStep(0),    //At attack phase it is linear step to increase result volume from zero to mVolume. In 1/32768 of volume
  mVolDelayEnvelope(0),   //Delay before sound start. In tick
  mVolAttackEnvelope(0),  //Attack time. Volume rich from zero to peak. In tick
  mVolHoldEnvelope(0),    //Hold time. Volume hold peak level
  mVolDecayEnvelope(0),   //Time for volume linearly ramps toward the sustain level
  mVolReleaseEnvelope(0), //Time
  mVolSustainLevel(0),    //This is the decrease in level, expressed in centibels, to which the Volume Envelope value ramps during the decay phase.
  mMasterVolume(nullptr)
  {
  if( !mAttenuator[0] ) {
    //Fill attenuator table
    for( int i = 0; i < 1024; i++ ) {
      double sb = i; //santiBell
      mAttenuator[i] = static_cast<int>( 1.0 / pow( 10.0, sb / 200.0 ) * 32768.0 );
      //qDebug() << i << mAttenuator[i];
      }
    }
  }



void CsSynthSfTrack::setup(int *masterVolume, int type, const qint16 *samples, int sampleEnd, int sampleLoopStart, int sampleLoopEnd,
                           bool exitLoop, int sampleStep, int volumeInitial, int volDelayEnvelope,
                           int volAttackEnvelope, int volHoldEnvelope, int volDecayEnvelope,
                           int volReleaseEnvelope, int volSustainLevel, int velRange)
  {
  if( volDelayEnvelope < 0 || volAttackEnvelope < 0 || volHoldEnvelope < 0 || volDecayEnvelope < 0 || volReleaseEnvelope < 0 ) {
    qDebug() << "fail delay" << volDelayEnvelope << volAttackEnvelope << volHoldEnvelope << volDecayEnvelope << volReleaseEnvelope;
    }
  mMasterVolume = masterVolume;
  mType = type;
  mSamples = samples;     //Samples vector
  mSampleEnd = sampleEnd;
  mSampleLoopStart = sampleLoopStart;
  mSampleLoopEnd = sampleLoopEnd;
  mExitLoop = exitLoop;        //Walk through end loop, else looping
  //Oscillator
  mSampleIndex = 0;     //Current sample index
  mSampleSubIndex = 0;  //Fractional part of sample index in 1/65536
  mSampleStep = sampleStep;      //Step of sample to provide a given sample rate. In 1/65536
  //Amplitude envelope modulator
  mVolumeInitial = volumeInitial;      //In santiBell note default attenuation
  mVolume = 0;             //Normal signal volume without regulation. In 1/32768
  mVolumePhase = vpStop;
  mVolumeTick = 0;
  mAttenuation = 0;        //Current attenuation level. At phase attack - linear multer in 1/32768 of volume
  mAttenuationStep = 0;    //At attack phase it is linear step to increase result volume from zero to mVolume. In 1/32768 of volume
  mVolDelayEnvelope = volDelayEnvelope;   //Delay before sound start. In tick
  mVolAttackEnvelope = volAttackEnvelope;  //Attack time. Volume rich from zero to peak. In tick
  mVolHoldEnvelope = volHoldEnvelope;    //Hold time. Volume hold peak level
  mVolDecayEnvelope = volDecayEnvelope;   //Time for volume linearly ramps toward the sustain level
  mVolReleaseEnvelope = volReleaseEnvelope; //Time
  mVolSustainLevel = volSustainLevel;    //This is the decrease in level, expressed in centibels, to which the Volume Envelope value ramps during the decay phase.
  mMinVelocity = velRange & 0x7f;
  mMaxVelocity = (velRange >> 8) & 0x7f;
  }




int CsSynthSfTrack::sample(bool &stopped)
  {
  if( mVolumePhase == vpStop )
    return mVolumeTick = 0;
  stopped = false;
  mVolumeTick++;
  if( mVolumePhase == vpDelay ) {
    if( mVolumeTick >= mVolDelayEnvelope ) {
      //Delay phase completed
      mVolumeTick = 0;
      mVolumePhase = vpAttack;
      mAttenuation = 0;
      if( mVolAttackEnvelope < 64 )
        mAttenuationStep = mVolume << 15;
      else
        mAttenuationStep = (mVolume << 15) / (mVolAttackEnvelope >> 6);
      }
    return 0;
    }

  if( mVolumePhase == vpAttack ) {
    if( mVolumeTick >= mVolAttackEnvelope ) {
      //Attack phase completed
      mVolumeTick = 0;
      mVolumePhase = vpHold;
      mAttenuation = 0;
      //qDebug() << "hold" << this;
      return nextSample() * mVolume >> 15;
      }
    if( (mVolumeTick & 0x3f) == 0 )
      mAttenuation += mAttenuationStep;
    return nextSample() * (mAttenuation >> 15) >> 15;
    }

  if( mVolumePhase == vpHold ) {
    if( mVolumeTick >= mVolHoldEnvelope ) {
      //Hold phase completed
      mVolumeTick = 0;
      mVolumePhase = vpDecay;
      mAttenuation = 0;
      //qDebug() << "decay" << this;
      if( mVolDecayEnvelope < 64 )
        mAttenuationStep = mVolSustainLevel << 15;
      else
        mAttenuationStep = (mVolSustainLevel << 15) / (mVolDecayEnvelope >> 6);
      }
    return nextSample() * mVolume >> 15;
    }

  if( mVolumePhase == vpDecay ) {
    if( (mVolumeTick & 0x3f) == 0 ) {
      mAttenuation += mAttenuationStep;
      //If in decay phase we rich -100dB then stop track
      if( mAttenuation >= (930 << 15) ) {
        mVolumePhase = vpStop;
        return 0;
        }
      }
    if( mVolumeTick >= mVolDecayEnvelope ) {
      //Decay phase completed
      mVolumePhase = vpSustain;
      mAttenuation = mVolSustainLevel << 15;
      //qDebug() << "sustain" << this;
      }
    return nextSample() * ( mVolume * mAttenuator[(mAttenuation >> 15) & 0x3ff] >> 15 ) >> 15;
    }

  if( mVolumePhase == vpSustain )
    return nextSample() * ( mVolume * mAttenuator[mVolSustainLevel] >> 15 ) >> 15;

  if( mVolumePhase == vpRelease ) {
    mAttenuation += mAttenuationStep;
//    if( mVolReleaseEnvelope < 64 )
//      mAttenuation += mAttenuationStep;
//    else if( (mVolumeTick & 0x3f) == 0 )
//      mAttenuation += mAttenuationStep;

    if( mAttenuation > (1000 << 15) ) mAttenuation = (1000 << 15);
    if( mVolumeTick >= mVolReleaseEnvelope )
      mVolumePhase = vpStop;
    int sm = nextSample() * ( mVolume * mAttenuator[(mAttenuation >> 15) & 0x3ff] >> 15 ) >> 15;
    //qDebug() << "sample" << sm;
    return sm;// nextSample() * ( mVolume * mAttenuator[(mAttenuation >> 15) & 0x3ff] >> 15 ) >> 15;
    }

  return 0;
  }




void CsSynthSfTrack::noteOff()
  {
//  qDebug() << "off from phase" << mVolumePhase;
  switch( mVolumePhase ) {
    case vpStop :
    case vpDelay :
      mVolumePhase = vpStop;
      break;
    case vpAttack :
      //Find appropriate attenuation level in santiBell
      mAttenuation >>= 15;
      for(int i = 0; i < 1000; i++ )
        if( mAttenuator[i] <= mAttenuation ) {
          mAttenuation = i << 15;
          break;
          }
//      mAttenuation = 0;
    [[fallthrough]];
    case vpHold :
    case vpDecay :
    case vpSustain :
    case vpRelease :
      //Start release phase
      if( mAttenuation > (1000 << 15) ) mAttenuation = (1000 << 15);
      if( mVolReleaseEnvelope == 0 )
        mAttenuationStep = ((1000 << 15) - mAttenuation);
      else
        mAttenuationStep = ((1000 << 15) - mAttenuation) / mVolReleaseEnvelope;
//      else if( mVolReleaseEnvelope < 64 )
//      else
//        mAttenuationStep = ((1000 << 15) - mAttenuation) / (mVolReleaseEnvelope >> 6);
//      qDebug() << "start release phase" << mAttenuation << mAttenuationStep << mVolReleaseEnvelope;
      mVolumeTick = 0;
      mVolumePhase = vpRelease;
      break;
    }
  }



bool CsSynthSfTrack::noteOn(quint8 pressure)
  {
  if( pressure == 0 ) noteOff(0);
  else if( mMinVelocity <= pressure && pressure <= mMaxVelocity ) {
//    qDebug() << "previous phase" << mVolumePhase;
    mVolumeTick = 0;
    mSampleSubIndex = 0;
    mSampleIndex = 0;
    mVolume = pressure * mAttenuator[mVolumeInitial] / 127;
    mVolumePhase = vpDelay;
    }
  return mVolumePhase != vpStop;
  }




int CsSynthSfTrack::nextSample()
  {
  //Calculate sample value as linear interpolation between neighboring samples on mSampleSubIndex value
  int curSample  = sampleAtIndex();
  int delta = sampleAtNextIndex() - curSample;
  curSample += delta * mSampleSubIndex >> 16;

  //Calculate next sample index
  mSampleSubIndex += mSampleStep;
  mSampleIndex += mSampleSubIndex >> 16; //Add integer part of index to sampleIndex
  mSampleSubIndex &= 0xffff; //Leave only fractional part

  if( mExitLoop ) {
    if( mSampleIndex + 1 >= mSampleEnd ) mVolumePhase = vpStop;
    }
  else {
    if( mSampleIndex >= mSampleLoopEnd ) {
      mSampleIndex -= mSampleLoopEnd;
      mSampleIndex += mSampleLoopStart;
      }
    }

  return curSample * (*mMasterVolume) >> 8;
  }



