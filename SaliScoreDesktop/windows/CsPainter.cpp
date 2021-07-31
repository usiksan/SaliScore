#include "CsPainter.h"

#include <QTransform>

CsPainter::CsPainter(QPainter *painter, const CsComposition &comp ) :
  mPainter(painter),
  mVisibleRemark(comp.remarkVisible()),
  mVisibleChord(comp.chordVisible()),
  mVisibleNote(comp.noteVisible()),
  mVisibleTranslate(comp.translationVisible()),
  mClefMap(comp.noteClefMap()),
  mCurY(30),
  mPixPerX(15),
  mScoreLineDistance(8),
  mFontSize(12),
  mTextGap(4),
  mLineGap(20)
  {
  mPainter->setFont( QFont(QStringLiteral("Serif"), mFontSize ) );
  QRect r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, QStringLiteral("H") );
  mTextHeight = r.height();
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
    mCurY += mLineGap;
    }
  }

void CsPainter::drawRemark(const QMap<QString, QString> &remarkMap)
  {
  //For each remark translations which visible we perform drawing
  for( const auto &lang : qAsConst(mVisibleRemark) ) {
    mCurY += mTextHeight;
    drawRemarkImpl( 20, mCurY, remarkMap.value(lang) );
    mCurY += mTextGap;
    }
  }

void CsPainter::drawChord(const QMap<QString, CsChordLine> &chordMap)
  {
  //For each chord line which visible we perform drawing
  for( const auto &chordKey : qAsConst(mVisibleChord) ) {
    mCurY += mTextHeight;
    drawChordImpl( 20, mCurY, chordMap.value(chordKey) );
    mCurY += mTextGap;
    }
  }

void CsPainter::drawNote(const QMap<QString, CsNoteLine> &noteMap)
  {
  //For each note line which visible we perform drawing
  for( const auto &noteKey : qAsConst(mVisibleNote) ) {
    drawNoteImpl( 20, mCurY, mClefMap.value(noteKey), noteMap.value(noteKey) );
    mCurY += 9 * mScoreLineDistance;
    }
  }

void CsPainter::drawLyric(const CsLyricList &lyricList)
  {
  mCurY += mTextHeight;
  //Paint each lyric
  for( auto const &lyric : lyricList ) {
    int visx = visualX( 20, lyric.position() );
    mPainter->drawText( visx, mCurY, lyric.lyric() );
    }
  mCurY += mTextGap;
  }

void CsPainter::drawTranslation(const QMap<QString, QString> &translationMap)
  {
  //For each translations which visible we perform drawing
  for( const auto &lang : qAsConst(mVisibleTranslate) ) {
    mCurY += mTextHeight;
    drawTranslationImpl( 20, mCurY, translationMap.value(lang) );
    mCurY += mTextGap;
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


QString unicode4( uint code )
  {
  return QString::fromUcs4( &code, 1 );
  }


QString noteFraction( int duration, int noteDuration )
  {
  duration -= noteDuration;
  noteDuration /= 2;
  if( duration == noteDuration )
    return QStringLiteral(".");

  duration -= noteDuration;
  noteDuration /= 2;
  if( duration == noteDuration )
    return QStringLiteral("..");

  return QStringLiteral("...");
  }


QString noteText( int duration, QString &fraction )
  {
  //Exact values
  int code = 0;
  switch( duration ) {
    case duraBreve : code = 119132; break;
    case duraHole : code = 119133; break;
    case duraHalf : code = 119134; break;
    case duraQuarter : code = 119135; break;
    case duraEighth : code = 119136; break;
    case duraSixteenth : code = 119137; break;
    case duraThirtySecond : code = 119138; break;
    case duraSixtyFourth : code = 119139; break;
    case duraOneHundredTwentyEighth : code = 119140; break;
    }
  if( code )
    return unicode4( code );

  if( duration > duraBreve ) {
    fraction = noteFraction( duration, duraBreve );
    return noteText( duraBreve, fraction );
    }
  if( duration > duraHole ) {
    fraction = noteFraction( duration, duraHole );
    return noteText( duraHole, fraction );
    }
  if( duration > duraHalf ) {
    fraction = noteFraction( duration, duraHalf );
    return noteText( duraHalf, fraction );
    }
  if( duration > duraQuarter ) {
    fraction = noteFraction( duration, duraQuarter );
    return noteText( duraQuarter, fraction );
    }
  if( duration > duraEighth ) {
    fraction = noteFraction( duration, duraEighth );
    return noteText( duraEighth, fraction );
    }
  if( duration > duraSixteenth ) {
    fraction = noteFraction( duration, duraSixteenth );
    return noteText( duraSixteenth, fraction );
    }
  if( duration > duraThirtySecond ) {
    fraction = noteFraction( duration, duraThirtySecond );
    return noteText( duraThirtySecond, fraction );
    }
  if( duration > duraSixtyFourth ) {
    fraction = noteFraction( duration, duraSixtyFourth );
    return noteText( duraSixtyFourth, fraction );
    }
  fraction = noteFraction( duration, duraOneHundredTwentyEighth );
  return noteText( duraOneHundredTwentyEighth, fraction );
  }


void CsPainter::drawNoteImpl(int x, int y, int clef, const CsNoteLine &noteLine)
  {
  int scoreY = y + mScoreLineDistance * 2;
  //Draw five lines of score
  for( int i = 0; i < 5; i++ )
    mPainter->drawLine( x, scoreY + i * mScoreLineDistance, x+1280, scoreY + i * mScoreLineDistance );

  QFont prev = mPainter->font();
  mPainter->setFont( QFont(QStringLiteral("Serif"), 4 * mScoreLineDistance ) );

  //Note abowe line 0
  int noteStart = whiteOctaveFirst + whiteC;
  //Clef
  if( clef == noteG ) {
    mPainter->drawText( 20, scoreY + 4 * mScoreLineDistance, unicode4(119070) );
    noteStart = whiteOctaveSecond + whiteG;
    }
  else if( clef == noteF ) {
    mPainter->drawText( 20, scoreY + 4 * mScoreLineDistance, unicode4(119074) );
    noteStart = whiteOctaveSmall + whiteB;
    }
  else if( clef == noteC ) {
    mPainter->drawText( 20, scoreY + 4 * mScoreLineDistance, unicode4(119073) );
    noteStart = whiteOctaveFirst + whiteB;
    }

  //Draw notes
  auto &noteList = noteLine.noteListGet();
  for( auto const &note : qAsConst(noteList) ) {
    int visX = visualX( x, note.position() );
    int yOffset = noteStart - note.white();
    int yPos = scoreY + yOffset * mScoreLineDistance / 2;
    if( yOffset <= -2 ) {
      //Appended line above must be drawn
      mPainter->drawLine( visX, scoreY - mScoreLineDistance, visX + 20, scoreY - mScoreLineDistance );
      }
    else if( yOffset > 10 ) {
      //Appended line below must be drawn
      mPainter->drawLine( visX, scoreY + 5 * mScoreLineDistance, visX + 20, scoreY + 5 * mScoreLineDistance );
      if( yOffset > 12 )
        //Second appended line below must be drawn
        mPainter->drawLine( visX, scoreY + 6 * mScoreLineDistance, visX + 20, scoreY + 6 * mScoreLineDistance );
      }

    if( yOffset < 5 ) {
      //Rotate symbol
      //Create text with mirror order
      QString fraction;
      QString noteSign = noteText( note.duration(), fraction );
      QString fullNote = fraction + noteSign;
      if( note.isDies() )
        fullNote.append( QStringLiteral(" ") + unicode4(9839) );

      QRect r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, fullNote );
      //At frist - rotation
      QTransform m = QTransform::fromScale( 1.0, 1.0 ).rotate( -180 );

      //At second - origin offset
      m *= QTransform::fromTranslate( visX + r.width(), yPos + 1 - mScoreLineDistance );
      mPainter->setTransform( m );
      mPainter->drawText( 0, 0, fullNote );
      mPainter->resetTransform();
      }
    else {
      //Simple text
      QString fraction;
      QString noteSign = noteText( note.duration(), fraction );
      QString fullNote;
      if( note.isDies() )
        fullNote = unicode4(9839);
      fullNote.append( noteSign + QStringLiteral(" ") + fraction );
      mPainter->drawText( visX, yPos, fullNote );
      }
    }
  mPainter->setFont( prev );
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
  return x + pos * 3 / 2;
  }
