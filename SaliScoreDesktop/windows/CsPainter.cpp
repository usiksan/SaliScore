#include "CsPainter.h"

CsPainter::CsPainter(QPainter *painter, const CsState &st) :
  mPainter(painter),
  mVisibleRemark(st.visibleRemark()),
  mVisibleChord(st.visibleChord()),
  mVisibleNote(st.visibleNote()),
  mVisibleTranslate(st.visibleTranslation()),
  mCurY(30),
  mPixPerX(15)
  {

  }

void CsPainter::drawLine(const CsLine &line)
  {
  drawRemark( line.remarkConst().remarkMapConst() );
  drawChord( line.chordKitConst().chordMapConst() );
  drawNote( line.noteKitConst().noteMapConst() );
  drawLyric( line.lyricListConst() );
  }

void CsPainter::drawRemark(const QMap<QString, QString> &remarkMap)
  {
  //For each remark translations which visible we perform drawing
  for( const auto &lang : qAsConst(mVisibleRemark) ) {
    drawRemarkImpl( 20, mCurY, remarkMap.value(lang) );
    mCurY += 15;
    }
  }

void CsPainter::drawChord(const QMap<QString, CsChordLine> &chordMap)
  {
  //For each chord line which visible we perform drawing
  for( const auto &chordKey : qAsConst(mVisibleChord) ) {
    drawChordImpl( 20, mCurY, chordMap.value(chordKey) );
    mCurY += 15;
    }
  }

void CsPainter::drawNote(const QMap<QString, CsNoteLine> &noteMap)
  {
  //For each note line which visible we perform drawing
  for( const auto &noteKey : qAsConst(mVisibleNote) ) {
    drawNoteImpl( 20, mCurY, noteMap.value(noteKey) );
    mCurY += 40;
    }
  }

void CsPainter::drawLyric(const CsLyricList &lyricList)
  {
  //Paint each lyric
  for( auto const &lyric : lyricList ) {
    int visx = visualX( 20, lyric.position() );
    mPainter->drawText( visx, mCurY, lyric.lyric() );
    }
  }





void CsPainter::drawRemarkImpl(int x, int y, const QString &rem)
  {
  //For empty string nothing done
  if( rem.isEmpty() )
    return;

  //Draw simple text
  mPainter->drawText( x, y, rem );
  }

void CsPainter::drawChordImpl(int x, int y, const CsChordLine &chordLine)
  {
  auto &chordList = chordLine.chordListConst();
  //Paint each chord
  for( auto const &chord : chordList ) {
    int visx = visualX( x, chord.position() );
    mPainter->drawText( visx, y, chord.chordText() );
    }
  }

void CsPainter::drawNoteImpl(int x, int y, const CsNoteLine &noteLine)
  {
  //Draw five lines of score
  for( int i = 0; i < 5; i++ )
    mPainter->drawLine( x, y + i * 5, x+1280, y + i * 5 );
  //Draw notes
  auto &noteList = noteLine.noteList();
  for( auto const &note : noteList ) {

    }
  }

int CsPainter::visualX(int x, int pos)
  {
  return x + pos / 2;
  }
