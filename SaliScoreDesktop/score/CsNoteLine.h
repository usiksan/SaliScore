#ifndef CSNOTELINE_H
#define CSNOTELINE_H

#include "CsConfig.h"
#include "CsNote.h"

class CsNoteLine
  {
    CsNoteList mNoteList;
  public:
    CsNoteLine();

    void jsonWrite( SvJsonWriter &js ) const;

    void jsonRead( SvJsonReader &js );
  };

#endif // CSNOTELINE_H
