#include "CsNote.h"
#include "../SvJson/SvJsonIO.h"

CsNote::CsNote()
  {

  }

int CsNote::white() const
  {
  int octaveIndex = mPitch / 12;
  int note = mPitch % 12;
  static int whiteCode[12] = {
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



bool CsNote::isDies() const
  {
  static bool dies[12] = {
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
  js.jsonInt( "Duration", mDuration );
  js.jsonInt( "Pitch", mPitch );
  }



void CsNote::jsonRead(CsJsonReader &js)
  {
  CsPosition::jsonRead( js );
  js.jsonInt( "Duration", mDuration );
  js.jsonInt( "Pitch", mPitch );
  }
