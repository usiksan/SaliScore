#include "CsChordToNote.h"

static int noteOffsets[chordCancel][5] = {
  { 4, 7, 0},         //  chordMaj,          //0,4,7
  { 4, 7, 9, 0 },     //  chordMaj6,         //0,4,7,9
  { 4, 7, 11, 0 },    //  chordMaj7,         //0,4,7,11
  { 4, 6, 7, 11, 0 }, //  chordMaj7sharp11,  //0,4,6,7,11
  { 2, 4, 7, 0 },     //  chordMaj_9_,       //0,2,4,7
  { 2, 4, 7, 11, 0 }, //  chordMaj7_9_,      //0,2,4,7,11
  { 2, 4, 7, 9, 0 },  //  chordMaj6_9_,      //0,2,4,7,9
  { 4, 8, 0 },        //  chordAug,          //0,4,8
  { 3, 7, 0 },        //  chordMin,          //0,3,7
  { 3, 7, 9, 0 },     //  chordMin6,         //0,3,7,9
  { 3, 7, 10, 0 },    //  chordMin7,         //0,3,7,10
  { 3, 6, 10, 0 },    //  chordMin7b5,       //0,3,6,10
  { 2, 3, 7, 0 },     //  chordMin_9_,       //0,2,3,7
  { 2, 3, 7, 10, 0 }, //  chordMin7_9_,      //0,2,3,7,10
  { 3, 5, 7, 10, 0 }, //  chordMin7_11_,     //0,3,5,7,10
  { 3, 7, 11, 0 },    //  chordMinMaj7,      //0,3,7,11
  { 2, 3, 7, 11, 0 }, //  chordMinMaj7_9_,   //0,2,3,7,11
  { 3, 6, 0 },        //  chordDim,          //0,3,6
  { 3, 6, 9, 0 },     //  chordDim7,         //0,3,6,9
  { 4, 7, 10, 0 },    //  chord7th,          //0,4,7,10
  { 5, 7, 10, 0 },    //  chord7sus4,        //0,5,7,10
  { 4, 6, 10, 0 },    //  chord7b5,          //0,4,6,10
  { 2, 4, 7, 10, 0 }, //  chord7_9_,         //0,2,4,7,10
  { 4, 6, 7, 10, 0 }, //  chord7sharp11,     //0,4,6,7,10
  { 4, 7, 9, 10, 0 }, //  chord7_13_,        //0,4,7,9,10
  { 1, 4, 7, 10, 0 }, //  chord7_b9_,        //0,1,4,7,10
  { 4, 7, 8, 10, 0 }, //  chord7_b13_,       //0,4,7,8,10
  { 3, 4, 7, 10, 0 }, //  chord7_sharp9_,    //0,3,4,7,10
  { 4, 8, 11, 0 },    //  chordMaj7aug,      //0,4,8,11
  { 4, 8, 10, 0 },    //  chord7aug,         //0,4,8,10
  { 0 },              //  chord1plus8,       //0
  { 7, 0 },           //  chord1plus5,       //0,7
  { 5, 7, 0 },        //  chordSus4,         //0,5,7
  { 2, 7, 0 }         //  chord1plus2plus5,  //0,2,7
};


CsChordToNote::CsChordToNote(int note, int chord)
  {
  for( mCount = 0; mCount < 7; mCount++ ) {
    mNotes[mCount] = note + noteOffsets[chord][mCount];
    if( noteOffsets[chord][mCount] == 0 ) {
      mCount++;
      break;
      }
    }
  }
