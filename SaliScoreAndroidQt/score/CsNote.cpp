/*
Project "SaliScore Score music edit, view and tutorial program"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  CsNote - is score note

  Which subclass of CsPosition CsNote contains global position in score
  and extend with pitch of note and midi command (for future extension)
*/
#include "CsNote.h"

CsNote::CsNote() :
  CsPosition(0,0)
  {

  }




//!
//! \brief noteSet Sets new halftone for note inside current octave
//! \param nt      One of 12 halftone
//!
void CsNote::noteSet(int nt)
  {
  int octaveIndex = mPitch / 12;
  mPitch = octaveIndex * 12 + qBound<int>( noteC, nt, noteB );
  }



//!
//! \brief noteShift Shift pitch of note up or down by one halftone
//! \param up        When true then pitch shifted up
//!
void CsNote::noteShift(bool up)
  {
  mPitch = qBound( 0, mPitch + (up ? 1 : -1), 127 );
  }




//!
//! \brief durationShift Shift duration of note by 2 times
//! \param less          When true duration decreased by 2 times else increased by 2 times
//!
void CsNote::durationShift(bool less)
  {
  durationSet( qBound( duraOneHundredTwentyEighth, less ? duration() * 2 : duration() / 2, duraBreve ) );
  }



//!
//! \brief octaveShift Shift pitch of note on one octave up or down
//! \param up          When true octave shifted up
//!
void CsNote::octaveShift(bool up)
  {
  mPitch = qBound( 0, mPitch + (up ? 12 : -12), 127 );
  }



//!
//! \brief white Returns note tone index from NoName octave (7 notes per octave)
//! \return      Note tone index from NoName octave (7 notes per octave)
//!
int CsNote::white() const
  {
  int octaveIndex = mPitch / 12;
  int note = mPitch % 12;
  static const int whiteCode[12] = {
    whiteC,
    whiteC,
    whiteD,
    whiteD,
    whiteE,
    whiteF,
    whiteF,
    whiteG,
    whiteG,
    whiteA,
    whiteA,
    whiteB
    };

  return whiteCode[note] + octaveIndex * 7;
  }



//!
//! \brief isDies Return true when dies need to be drawn
//! \return       true when dies need to be drawn
//!
bool CsNote::isDies() const
  {
  static const bool dies[12] = {
    false, //noteC
    true,  //noteCd,
    false, //noteD,
    true,  //noteDd,
    false, //noteE,
    false, //noteF,
    true,  //noteFd,
    false, //noteG,
    true,  //noteGd,
    false, //noteA,
    true,  //noteAd,
    false  //noteB
    };
  return dies[mPitch % 12];
  }


void CsNote::jsonWrite(CsJsonWriter &js) const
  {
  CsPosition::jsonWrite( js );
  js.jsonInt( "Pitch", mPitch );
  js.jsonInt( "Command", mCommand );
  }



void CsNote::jsonRead(CsJsonReader &js)
  {
  CsPosition::jsonRead( js );
  js.jsonInt( "Pitch", mPitch );
  js.jsonInt( "Command", mCommand );
  }
