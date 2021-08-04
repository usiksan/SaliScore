#include "CsKaraokePage.h"


void CsKaraokeLine::paintSimple(CsPainter &cp, const CsComposition &comp) const
  {
  cp.drawLine( mPosY, mNewLine, comp.line(mNewLine) );
  }



void CsKaraokeLine::paintOpaque(int elapsedPercent, CsPainter &cp, const CsComposition &comp) const
  {
  if( elapsedPercent < 40 ) {
    cp.drawLine( mPosY, mPrevLine, comp.line(mPrevLine) );

    //From
    }
  if( elapsedPercent > 60 ) {
    cp.drawLine( mPosY, mNewLine, comp.line(mNewLine) );
    }
  }




CsKaraokePage::CsKaraokePage(CsKaraokeLineMode lineMode, CsKaraokeLineList lineList) :
  mLineMode(lineMode),
  mLineList(lineList)
  {

  }

void CsKaraokePage::paint(int elapsedPercent, CsPainter &cp, const CsComposition &comp) const
  {
  switch( mLineMode ) {
    case cklmSimple :
      for( auto const &line : mLineList )
        line.paintSimple( cp, comp );
      break;

    case cklmOpaque :
      for( auto const &line : mLineList )
        line.paintOpaque( elapsedPercent, cp, comp );
      break;

    case cklmRolled :
      for( auto const &line : mLineList )
        line.paintSimple( cp, comp );
      break;

    }
  }




