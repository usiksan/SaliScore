#ifndef CSNOTELINE_H
#define CSNOTELINE_H

#include "config.h"
#include "CsNote.h"

class CsNoteLine
  {
    CsNoteList mNoteList;
    int        mNumerator;
    int        mDenominator;
  public:
    CsNoteLine();

    auto &noteListGet() const { return mNoteList; }

    void  noteListSet( const CsNoteList &list ) { mNoteList = list; }


    int   numerator() const { return mNumerator; }
    void  setNumerator(int numerator) { mNumerator = numerator; }

    int   denominator() const { return mDenominator; }
    void  setDenominator(int denominator) { mDenominator = denominator; }

    void  jsonWrite( CsJsonWriter &js ) const;

    void  jsonRead( CsJsonReader &js );
  };

#endif // CSNOTELINE_H
