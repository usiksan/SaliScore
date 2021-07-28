#ifndef CSPAINTER_H
#define CSPAINTER_H

#include "score/CsRemark.h"
#include "score/CsChordLine.h"
#include "score/CsNoteLine.h"
#include "score/CsLyric.h"

class CsPainter
  {
  public:
    CsPainter();

    void drawRemark( const CsRemark &remark );

    void drawChordLine( const CsChordLine &chordLine );

    void drawNoteLine( const CsNoteLine &noteLine );

    void drawLyric( const CsLyric &lyric );
  };

#endif // CSPAINTER_H
