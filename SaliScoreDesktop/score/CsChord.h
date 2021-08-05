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
