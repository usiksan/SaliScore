#ifndef CSNOTECHORD_H
#define CSNOTECHORD_H


enum CsNotes {
  noteC,
  noteCd,
  noteD,
  noteDd,
  noteE,
  noteF,
  noteFd,
  noteG,
  noteGd,
  noteA,
  noteAd,
  noteB
  };



enum CsOctave {
  octaveNoName   = 0,
  octaveSubContr = 12,
  octaveContr    = 24,
  octaveBig      = 36,
  octaveSmall    = 48,
  octaveFirst    = 60,
  octaveSecond   = 72,
  octaveThird    = 84,
  octaveFourth   = 96,
  octaveFifth    = 108,
  octaveSixth    = 120
  };

inline CsNotes csNoteInOctave( int note ) { return (CsNotes)(note % 12); }


inline bool csKeyNoBlackFromLeft( int note ) { note %= 12; return (note == noteC || note == noteF); }
inline bool csKeyNoBlackFromRight( int note ) { note %= 12; return (note == noteE || note == noteB); }
inline bool csKeyIsBlack( int note ) { note %= 12; return note == noteCd || note == noteDd || note == noteFd || note == noteGd || note == noteAd; }
inline bool csKeyIsWhite( int note ) { return !csKeyIsBlack(note); }


enum CsWhiteKey {
  whiteC,
  whiteD,
  whiteE,
  whiteF,
  whiteG,
  whiteA,
  whiteB
  };


enum CsWhiteOctave {
  whiteOctaveNoName   = 0,
  whiteOctaveSubContr = 7,
  whiteOctaveContr    = 14,
  whiteOctaveBig      = 21,
  whiteOctaveSmall    = 28,
  whiteOctaveFirst    = 35,
  whiteOctaveSecond   = 42,
  whiteOctaveThird    = 49,
  whiteOctaveFourth   = 56,
  whiteOctaveFifth    = 63,
  whiteOctaveSixth    = 70
  };



enum CsChordVar {
  chordMaj,          //0,4,7
  chordMaj6,         //0,4,7,9
  chordMaj7,         //0,4,7,11
  chordMaj7sharp11,  //0,4,6,7,11
  chordMaj_9_,       //0,2,4,7
  chordMaj7_9_,      //0,2,4,7,11
  chordMaj6_9_,      //0,2,4,7,9
  chordAug,          //0,4,8
  chordMin,          //0,3,7
  chordMin6,         //0,3,7,9
  chordMin7,         //0,3,7,10
  chordMin7b5,       //0,3,6,10
  chordMin_9_,       //0,2,3,7
  chordMin7_9_,      //0,2,3,7,10
  chordMin7_11_,     //0,3,5,7,10
  chordMinMaj7,      //0,3,7,11
  chordMinMaj7_9_,   //0,2,3,7,11
  chordDim,          //0,3,6
  chordDim7,         //0,3,6,9
  chord7th,          //0,4,7,10
  chord7sus4,        //0,5,7,10
  chord7b5,          //0,4,6,10
  chord7_9_,         //0,2,4,7,10
  chord7sharp11,     //0,4,6,7,10
  chord7_13_,        //0,4,7,9,10
  chord7_b9_,        //0,1,4,7,10
  chord7_b13_,       //0,4,7,8,10
  chord7_sharp9_,    //0,3,4,7,10
  chordMaj7aug,      //0,4,8,11
  chord7aug,         //0,4,8,10
  chord1plus8,       //0
  chord1plus5,       //0,7
  chordSus4,         //0,5,7
  chord1plus2plus5,  //0,2,7
  chordCancel
  };




#define duraBreve                  512 // 2
#define duraHole                   256 // 1
#define duraHalf                   128 // 1/2
#define duraQuarter                 64 // 1/4
#define duraEighth                  32 // 1/8
#define duraSixteenth               16 // 1/16
#define duraThirtySecond             8 // 1/32
#define duraSixtyFourth              4 // 1/64
#define duraOneHundredTwentyEighth   2 // 1/128

#endif // CSNOTECHORD_H
