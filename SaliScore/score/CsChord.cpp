#include "CsChord.h"

using ccharp = const char*;
static ccharp notes[12] = {
  "C",  // noteC,
  "C#", // noteCd,
  "D",  // noteD,
  "D#", // noteDd,
  "E",  // noteE,
  "F",  // noteF,
  "F#", // noteFd,
  "G",  // noteG,
  "G#", // noteGd,
  "A",  // noteA,
  "A#", // noteAd,
  "B"   // noteB
  };

static ccharp chords[34] = {
  "",       // chordMaj,      Maj,       //0,4,7
  "maj6",   // chordMaj6,     Maj6,    //0,4,7,9
  "maj7",   // chordMaj7,     Maj7,     //0,4,7,11
  "7#11",   // chordMaj7sharp11,  M7#11, //0,4,6,7,11
  "add9",   // chordMaj_9_,   Madd9,    //0,2,4,7
  "7(9)",   // chordMaj7_9_,  M7(9),    //0,2,4,7,11
  "6(9)",   // chordMaj6_9_,  M6(9),    //0,2,4,7,9
  "aug",       // chordAug,   aug,       //0,4,8
  "m",      // chordMin,      m,    //0,3,7
  "m6",     // chordMin6,     m6,    //0,3,7,9
  "m7",     // chordMin7,     m7,    //0,3,7,10
  "m7b5",   // chordMin7b5,   m7b5,    //0,3,6,10
  "m(9)",   // chordMin_9_,   m(9),    //0,2,3,7
  "m7(9)",  // chordMin7_9_,  m7(9),    //0,2,3,7,10
  "m7(11)", // chordMin7_11_, m7(11),    //0,3,5,7,10
  "mM7",    // chordMinMaj7,  mM7,    //0,3,7,11
  "mM7(9)", // chordMinMaj7_9_, mM7(9),  //0,2,3,7,11
  "dim",    // chordDim,      dim,    //0,3,6
  "dim7",   // chordDim7,     dim7,    //0,3,6,9
  "7",      // chord7th,      7,    //0,4,7,10
  "7sus4",  // chord7sus4,    7sus4,    //0,5,7,10
  "7b5",    // chord7b5,      7b5,    //0,4,6,10
  "7(9)",   // chord7_9_,     7(9),    //0,2,4,7,10
  "7(#11)", // chord7sharp11, 7(#11),    //0,4,6,7,10
  "7(13)",  // chord7_13_,    7(13),    //0,4,7,9,10
  "7(b9)",  // chord7_b9_,    7(b9),    //0,1,4,7,10
  "7(b13)", // chord7_b13_,   7(b13),    //0,4,7,8,10
  "7(#9)",  // chord7_sharp9_,7(#9),    //0,3,4,7,10
  "M7aug",  // chordMaj7aug,  M7aug,    //0,4,8,11
  "7aug",   // chord7aug,     7aug,    //0,4,8,10
  "1+8",    // chord1plus8,   1+8,    //0
  "1+5",    // chord1plus5,   1+5,    //0,7
  "sus4",   // chordSus4,     sus4    //0,5,7
  "1+2+5"   // chord1plus2plus5, 1+2+5 //0,2,7
  };

CsChord::CsChord() :
  CsPosition(0, 0)
  {

  }

CsChord::CsChord(int pos, int duration, int note, int chord) :
  CsPosition(pos, duration),
  mNote(qBound<int>(noteC,note,noteB)),
  mChord(qBound<int>(chordMaj,chord,chord1plus2plus5))
  {

  }



QString CsChord::chordText() const
  {
  return QString(notes[mNote]) + QString(chords[mChord]);
  }



CsChord CsChord::fromString(const QString &str)
  {
  QStringList list = str.split( QChar(',') );
  if( list.count() == 4 )
    return CsChord( list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt(), list.at(3).toInt() );
  return CsChord{};
  }





void CsChord::json(SvJsonWriter &js) const
  {
  CsPosition::json( js );
  js.jsonInt( "note", mNote );
  js.jsonInt( "chord", mChord );
  }

void CsChord::json(SvJsonReader &js)
  {
  CsPosition::json( js );
  js.jsonInt( "note", mNote );
  js.jsonInt( "chord", mChord );
  }
