#ifndef CSNOTELINE_H
#define CSNOTELINE_H

#include "CsConfig.h"
#include "CsNote.h"

class CsNoteLine
  {
    CsNoteList mNoteList;
  public:
    CsNoteLine();

    auto &noteList() const { return mNoteList; }

    void jsonWrite( CsJsonWriter &js ) const;

    void jsonRead( CsJsonReader &js );
  };

#endif // CSNOTELINE_H
