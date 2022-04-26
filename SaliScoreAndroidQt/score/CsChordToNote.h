#ifndef CSCHORDTONOTE_H
#define CSCHORDTONOTE_H

#include "CsNoteChord.h"
#include <QObject>

class CsChordToNote
  {
    int mNotes[7];
    int mCount;
  public:
    CsChordToNote( int note, int chord );

    int pitch( int index ) const { Q_ASSERT( index >= 0 && index < mCount ); return mNotes[index]; }

    int count() const { return mCount; }
  };

#endif // CSCHORDTONOTE_H
