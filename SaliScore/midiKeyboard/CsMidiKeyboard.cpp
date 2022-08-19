#include "CsMidiKeyboard.h"
#include "synth/CsSynthVoiceId.h"

#include <QDebug>


CsMidiKeyboard::CsMidiKeyboard(QObject *parent)
  : QObject{parent}
  , mBank(0)
  {

  }



void CsMidiKeyboard::midiSignal( quint8 control, quint8 data0, quint8 data1 )
  {
  switch( control & 0xf0 ) {
    case 0 :
      //Note off
      emit midiNote( data0, 0 );
      break;

    case 0x10 :
      //Note on
      emit midiNote( data0, data1 );
      break;

    case 0x30 :
      //Controller
      if( data0 == 0 )
        //MSB bank
        mBank = (data1 << 7) | (mBank & 0x7f);
      else if( data0 == 0x20 )
        mBank = data1 | (mBank & 0x3f80);
      else
        qDebug() << "midi ctrl" << data0 << "d=" << data1;
      break;

    case 0x40 :
      //Voice changed
      emit midiVoice( csVoiceId( mBank, data0 ) );
      break;
    }
  }
