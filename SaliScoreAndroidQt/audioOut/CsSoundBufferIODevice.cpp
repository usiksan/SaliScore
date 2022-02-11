/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Sound buffer device - interface device with sound card of computer.
   Main idea: sound synthesizes by notes. Simultaneously supported a lot of notes.
   Each of note synthesizes sound separately and SoundBufferIODevice mixes them into
   single stream. When we need to play some note we append it SoundBufferIODevice
   notes play list. When note end playing we remove it from notes play list.
*/
#include "CsSoundBufferIODevice.h"
#include <QDebug>
#include <QAudioOutput>

QAudioOutput *audio;

static int audioBufferSize;

CsSoundBufferIODevice::CsSoundBufferIODevice() :
  QIODevice()
  {
  }




//!
//! \brief addNote Append note to notes play list
//! \param notePtr Note to append
//!
void CsSoundBufferIODevice::addSound(CsSoundSourcePtr soundPtr)
  {
  if( !mActiveSounds.contains(soundPtr) )
    mActiveSounds.append( soundPtr );
  }




bool CsSoundBufferIODevice::isSequential() const
  {
  return true;
  }




static int fillBuffer = 0;

qint64 CsSoundBufferIODevice::bytesAvailable() const
  {
  if( fillBuffer <= 14 || (audioBufferSize - audio->bytesFree()) < CS_SAMPLES_PER_20MS * 4  )
    return CS_SAMPLES_PER_20MS * 2;
  return 0;
  }





qint64 CsSoundBufferIODevice::readData(char *data, qint64 maxlen)
  {
  Q_UNUSED(maxlen)

  //With this we fight with average
  //When average occur we decrement this and all sound samples downscales to 1/16 part
  static int sampleScaler = 16;


  qint16 *outSamples = static_cast<qint16*>( static_cast<void*>(data) );

  //Fill next samples
  for( int i = 0; i < CS_SAMPLES_PER_20MS; i++ ) {
    //Summ all channels
    CsSoundSample sample;
    for( auto ptr : qAsConst(mActiveSounds) )
      sample += ptr->sample();

    //Ajust scale factor
    if( qMax( qAbs(sample.mLeft), qAbs(sample.mRight) ) > 32767 ) {
      sampleScaler--;
      qDebug() << "top average left" << sample.mLeft << sampleScaler;
      }

    //Scale
    sample.mLeft = sample.mLeft * sampleScaler >> 4;
    //Average sample
    if( sample.mLeft > 32767 )  sample.mLeft = 32767;
    if( sample.mLeft < -32768 ) sample.mLeft = -32768;

    sample.mRight = sample.mRight * sampleScaler >> 4;
    if( sample.mRight > 32767 )  sample.mRight = 32767;
    if( sample.mRight < -32768 ) sample.mRight = -32768;

    //Store sample to output buffer
    outSamples[i] = static_cast<qint16>(sample.mLeft);
    }

  //Check and remove stopped notes
  for( int i = mActiveSounds.count() - 1; i >= 0; i-- )
    if( mActiveSounds.at(i)->isStopped() ) {
      mActiveSounds.removeAt(i);
//      qDebug() << "remove" << i;
      }

  return CS_SAMPLES_PER_20MS * 2;
  }





qint64 CsSoundBufferIODevice::writeData(const char *data, qint64 len)
  {
  Q_UNUSED(data);
  Q_UNUSED(len);
  return 0;
  }
