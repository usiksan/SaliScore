#ifndef CSNOTELINE_H
#define CSNOTELINE_H

#include "CsConfig.h"
#include "CsNote.h"

class CsNoteLine
  {
    CsNoteList mNoteList;
  public:
    CsNoteLine();

    auto &noteListGet() const { return mNoteList; }

    void  noteListSet( const CsNoteList &list ) { mNoteList = list; }

    void jsonWrite( CsJsonWriter &js ) const;

    void jsonRead( CsJsonReader &js );
  };

#endif // CSNOTELINE_H
