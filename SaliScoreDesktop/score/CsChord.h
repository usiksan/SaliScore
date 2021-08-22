/*
Project "SaliScore Score music edit, view and tutorial programm"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Chord model
*/
#ifndef CSCHORD_H
#define CSCHORD_H

#include "CsConfig.h"
#include "CsPosition.h"
#include "CsNoteChord.h"

#include <QList>



class CsChord : public CsPosition
  {
    int mNote;  //!< Chord tonica, one of CsNotes
    int mChord; //!< Chord variant, one of CsChordVar
  public:
    CsChord();
    CsChord( int pos, int duration, int note, int chord );

    QString chordText() const;

    int     note() const { return mNote; }
    void    noteSet( int n ) { mNote = qBound<int>( noteC, n, noteB ); }

    int     chord() const { return mChord; }
    void    chordSet( int ch ) { mChord = qBound<int>( chordMaj, ch, chord1plus2plus5 ); }

    // CsPosition interface
  public:
    virtual void jsonWrite(CsJsonWriter &js) const override;
    virtual void jsonRead(CsJsonReader &js) override;
  };

//using CsChordPtr

using CsChordList = QList<CsChord>;

#endif // CSCHORD_H
