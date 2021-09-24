#include "CsCursorEditProperty.h"
#include "score/CsComposition.h"

CsCursorEditProperty::CsCursorEditProperty(int propertyId, CsComposition &comp) :
  CsCursorEditString( comp )
  {
  mClass = propertyId;

  switch( propertyId ) {
    case cccTitle :
      setText( comp.title(), true );
      break;

    case cccVoice :
      setText( comp.voice(), true );
      break;

    case cccSinger :
      setText( comp.singer(), true );
      break;

    case cccVoiceDual :
      setText( comp.voiceDual(), true );
      break;

    case cccComposer :
      setText( comp.composer(), true );
      break;

    case cccVoiceLeft :
      setText( comp.voiceLeft(), true );
      break;

    case cccLyricist :
      setText( comp.lyricist(), true );
      break;

    case cccStyle :
      setText( comp.style(), true );
      break;

    case cccTempo :
      setText( comp.tempo(), true );
      break;
    }

  }





void CsCursorEditProperty::apply()
  {
  switch( mClass ) {
    case cccTitle :
      mComposition.titleSet( mString );
      break;

    case cccVoice :
      mComposition.voiceSet( mString );
      break;

    case cccSinger :
      mComposition.singerSet( mString );
      break;

    case cccVoiceDual :
      mComposition.voiceDualSet( mString );
      break;

    case cccComposer :
      mComposition.composerSet( mString );
      break;

    case cccVoiceLeft :
      mComposition.voiceLeftSet( mString );
      break;

    case cccLyricist :
      mComposition.lyricistSet( mString );
      break;

    case cccStyle :
      mComposition.styleSet( mString );
      break;

    case cccTempo :
      mComposition.tempoSet( mString );
      break;
    }
  }
