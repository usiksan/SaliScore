#include "CsVisualWinMain.h"

CsVisualWinMain::CsVisualWinMain(QWidget *parent)
  : QMainWindow{parent}
  {

  }

void CsVisualWinMain::extractChords(const CsLine &line, const CsDefinition &def, int lastPosition)
  {
  if( def.mSynthes || def.mTeach ) {
    //Perform extraction
    const auto &list = line.chordListGet( def.mName );
    for( auto &chord : list ) {
      if( chord.isBeginInside( mLinePosition, lastPosition ) ) {
        //This chord hit in current interval - append chord to array
        if( def.mSynthes ) {
          //Append chord to synthes array
          mEventList.append( CsPlayerEvent( def.channel(), chord.note(), chord.duration() ) );
          }
        }
      }
    }
  }
