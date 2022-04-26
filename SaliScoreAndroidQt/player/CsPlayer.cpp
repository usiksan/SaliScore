#include "CsPlayer.h"
#include "score/CsComposition.h"


CsPlayer::CsPlayer(QObject *parent)
  : QObject{parent}
  {

  }
/*
void CsPlayer::tickNext(int tickAddon)
  {
  const CsLine &line = mComposition.line( mLineIndex );
  int lastTick = qMin( mLinePosition + tickAddon, mLineTickCount );
  const CsDefList chordDefList( mComposition.chordDefList() );
  for( auto const &def : qAsConst(chordDefList) ) {
    extractChords( line, def, lastTick );
    }
  //We extract all pla
  }



void CsPlayer::extractChords(const CsLine &line, const CsDefinition &def, int lastPosition)
  {
  if( def.mSynthes || def.mTeach ) {
    //Perform extraction
    const auto &list = line.chordListGet( def.mName );
    for( auto &chord : list ) {

      }
    }
  }
*/
