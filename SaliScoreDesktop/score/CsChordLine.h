#ifndef CSCHORDLINE_H
#define CSCHORDLINE_H

#include "CsConfig.h"
#include "CsChord.h"

class CsChordLine
  {
    CsChordList mList;
  public:
    CsChordLine();

    void jsonWrite( SvJsonWriter &js ) const;

    void jsonRead( SvJsonReader &js );
  };

#endif // CSCHORDLINE_H
