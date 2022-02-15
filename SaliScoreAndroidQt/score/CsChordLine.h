#ifndef CSCHORDLINE_H
#define CSCHORDLINE_H

#include "config.h"
#include "CsChord.h"

class CsChordLine
  {
    CsChordList mList;
  public:
    CsChordLine();
    CsChordLine( const CsChordList &list ) : mList(list) {}

    auto &chordListConst() const { return mList; }

    void jsonWrite( CsJsonWriter &js ) const;

    void jsonRead( CsJsonReader &js );
  };

#endif // CSCHORDLINE_H
