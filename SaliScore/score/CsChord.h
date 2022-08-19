/*
Project "SaliScore Score music edit, view and tutorial program"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Chord model. Chord consists tonica and chord variant, all are predefined enumerations
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

    QString toString() const { return QStringLiteral("%1,%2,%3,%4").arg( position() ).arg( duration() ).arg( mNote ).arg( mChord ); }

    static  CsChord fromString( const QString &str );
    // CsPosition interface
  public:
    virtual void json(SvJsonWriter &js) const override;
    virtual void json(SvJsonReader &js) override;
  };

//using CsChordPtr

using CsChordList = QList<CsChord>;

#endif // CSCHORD_H
