#include "CsPainter.h"

#include <QTransform>

CsPainter::CsPainter(QPainter *painter, const CsComposition &comp ) :
  mPainter(painter),
  mVisibleRemark(comp.remarkVisible()),
  mVisibleChord(comp.chordVisible()),
  mVisibleNote(comp.noteVisible()),
  mVisibleTranslate(comp.translationVisible()),
  mCurY(30),
  mPixPerX(15),
  mScoreLineDistance(16)
  {

  }

void CsPainter::drawLine(const CsLine &line)
  {
  if( line.isRemark() )
    drawRemark( line.remarkConst().remarkMapConst() );
  else {
    drawChord( line.chordKitConst().chordMapConst() );
    drawNote( line.noteKitConst().noteMapConst() );
    drawLyric( line.lyricListConst() );
    drawTranslation( line.translationConst() );
    }
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
    mCurY += 9 * mScoreLineDistance;
    }
  }

void CsPainter::drawLyric(const CsLyricList &lyricList)
  {
  //Paint each lyric
  for( auto const &lyric : lyricList ) {
    int visx = visualX( 20, lyric.position() );
    mPainter->drawText( visx, mCurY, lyric.lyric() );
    }
  mCurY += 15;
  }

void CsPainter::drawTranslation(const QMap<QString, QString> &translationMap)
  {
  //For each translations which visible we perform drawing
  for( const auto &lang : qAsConst(mVisibleTranslate) ) {
    drawTranslationImpl( 20, mCurY, translationMap.value(lang) );
    mCurY += 15;
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
  int scoreY = y + mScoreLineDistance * 2;
  //Draw five lines of score
  for( int i = 0; i < 5; i++ )
    mPainter->drawLine( x, scoreY + i * mScoreLineDistance, x+1280, scoreY + i * mScoreLineDistance );

  QFont prev = mPainter->font();
  mPainter->setFont( QFont(QStringLiteral("sans"), 4 * mScoreLineDistance ) );

  //At frist - rotation
  QTransform m = QTransform::fromScale( 1.0, 1.0 ).rotate( -180 );

  //At second - origin offset
  m *= QTransform::fromTranslate( 200, scoreY+1 );
  mPainter->setTransform( m );

  static uint nt[30] = { 119135, 119136, 119137, 119070 };
  mPainter->drawText( 0, 0, QString::fromUcs4( nt, 1 ) );

  mPainter->resetTransform();

  mPainter->drawText( 20, scoreY + 4 * mScoreLineDistance, QString::fromUcs4( nt + 3, 1 ) );

  mPainter->drawText( 240, scoreY + mScoreLineDistance / 2, QString::fromUcs4( nt + 1, 1 ) );

  mPainter->drawText( 260, scoreY + mScoreLineDistance, QString::fromUcs4( nt + 2, 1 ) );

  mPainter->setFont( prev );
  //Draw notes
  auto &noteList = noteLine.noteListGet();
  for( auto const &note : noteList ) {

    }
  }

void CsPainter::drawTranslationImpl(int x, int y, const QString &tran)
  {
  //For empty string nothing done
  if( tran.isEmpty() )
    return;

  //mPainter->setFont( QFont(QString("sans"),15) );
  //Draw simple text
  mPainter->drawText( x, y, tran );
  }

int CsPainter::visualX(int x, int pos)
  {
  return x + pos / 2;
  }
