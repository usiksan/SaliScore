#ifndef CSCHORD_H
#define CSCHORD_H

#include "CsConfig.h"
#include "CsPosition.h"

#include <QList>

//enum CsChordBase {
//  chordA,
//  chordB,
//  chordC,
//  chordD,
//  chordE,
//  chordF,
//  chordG
//  };

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


class CsChord : public CsPosition
  {
    int mNote;  //!< Chord tonica, one of CsNotes
    int mChord; //!< Chord variant, one of CsChordVar
  public:
    CsChord();
    CsChord( int pos, int note, int chord );

    QString chordText() const;

//    static QString note2text( int note );
//    static QString chord2text( int chord );

    // CsPosition interface
  public:
    virtual void jsonWrite(CsJsonWriter &js) const override;
    virtual void jsonRead(CsJsonReader &js) override;
  };

//using CsChordPtr

using CsChordList = QList<CsChord>;

#endif // CSCHORD_H
