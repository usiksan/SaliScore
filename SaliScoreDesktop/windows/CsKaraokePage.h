#ifndef CSKARAOKEPAGE_H
#define CSKARAOKEPAGE_H

#include "CsPainter.h"
#include "score/CsComposition.h"

#include <QList>
#include <QMap>

enum CsKaraokeLineMode {
  cklmSimple,
  cklmOpaque,
  cklmRolled
};

struct CsKaraokeLine
  {
    int mPosY;
    int mPrevLine;
    int mNewLine;

    CsKaraokeLine( int posy, int lineIndex ) : mPosY(posy), mPrevLine(lineIndex), mNewLine(lineIndex) {}
    CsKaraokeLine( int posy, int prevLine, int newLine ) : mPosY(posy), mPrevLine(prevLine), mNewLine(newLine) {}

    void paintSimple( CsPainter &cp, const CsComposition &comp ) const;

    void paintOpaque( int elapsedPercent, CsPainter &cp, const CsComposition &comp ) const;
  };


using CsKaraokeLineList = QList<CsKaraokeLine>;


class CsKaraokePage
  {
    CsKaraokeLineMode mLineMode;
    CsKaraokeLineList mLineList;
  public:
    CsKaraokePage() : mLineMode(cklmSimple) {}
    CsKaraokePage( CsKaraokeLineMode lineMode, CsKaraokeLineList lineList );

    void          paint( int elapsedPercent, CsPainter &cp, const CsComposition &comp ) const;

    int           lineCount() const { return mLineList.count(); }

    CsKaraokeLine line( int index ) const { return mLineList.at(index); }
  };

using CsKaraokePageMap = QMap<int,CsKaraokePage>;

#endif // CSKARAOKEPAGE_H
