#include "CsCursorEditProperty.h"
#include "score/CsComposition.h"

CsCursorEditProperty::CsCursorEditProperty(int propertyId, CsComposition &comp) :
  CsCursorEditString( comp )
  {
  mClass = propertyId;

  switch( propertyId ) {
    case cccTitle :
      setText( comp.title(), false );
      break;

    case cccVoice :
      setText( comp.voice(), false );
      break;

    case cccSinger :
      setText( comp.singer(), false );
      break;

    case cccVoiceDual :
      setText( comp.voiceDual(), false );
      break;

    case cccComposer :
      setText( comp.composer(), false );
      break;

    case cccVoiceRight :
      setText( comp.voiceRight(), false );
      break;

    case cccLyricist :
      setText( comp.lyricist(), false );
      break;

    case cccStyle :
      setText( comp.style(), false );
      break;

    case cccTempo :
      setText( comp.tempo(), false );
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

    case cccVoiceRight :
      mComposition.voiceRightSet( mString );
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
