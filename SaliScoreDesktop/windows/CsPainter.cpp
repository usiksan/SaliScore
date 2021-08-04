#include "CsPainter.h"

#include <QTransform>
#include <QSettings>
#include <QJsonDocument>

CsPainter::CsPainter(QPainter *painter, const QString &keyViewSettings, const CsComposition &comp, const CsPlay &player ) :
  mPainter(painter),
  mPlayer(player),
  mVisibleRemark(comp.remarkVisible()),
  mVisibleChord(comp.chordVisible()),
  mVisibleNote(comp.noteVisible()),
  mVisibleTranslate(comp.translationVisible()),
  mClefMap(comp.noteClefMap())
  {
  QSettings s;
  if( s.contains(keyViewSettings) ) {
    }

  //Calculate font height
  mRemarkTextHeight      = fontHeight( mSettings.mRemarkFontSize );
  mChordTextHeight       = fontHeight( mSettings.mChordFontSize );
  mLyricTextHeight       = fontHeight( mSettings.mLyricFontSize );
  mTranslationTextHeight = fontHeight( mSettings.mTranslationFontSize );
  mTitleHeight           = fontHeight( mSettings.mTitleFontSize );
  mPropertiesHeight      = fontHeight( mSettings.mPropertiesFontSize );

  //First takt offset
  mClefPos = mSettings.mLeftMenuSize + 15;
  mDenominatorPos = mClefPos + 20;
  mLeftGap = mDenominatorPos + 20;
  }




int CsPainter::drawTitleAndProperties(int y, QSize size, const CsComposition &comp)
  {
  //Draw title and properties
  mCurY = y;

  //Draw title on horizontal center
  mPainter->setFont( QFont(mSettings.mFontName, mSettings.mTitleFontSize) );
  QRect r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, comp.title() );
  int x = (r.width() - size.width()) / 2;
  mPainter->drawText( x, y + mTitleHeight, comp.title() );
  mCurY += mTitleHeight + mSettings.mTextGap;

  //At left side properties
  mPainter->setFont( QFont(mSettings.mFontName, mSettings.mPropertiesFontSize) );
  QString singer = QObject::tr("Singer:");
  QString composer = QObject::tr("Composer");
  QString lyricist = QObject::tr("Lyricist:");
  QString author = QObject::tr("Author:");
  r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, singer );
  int w = r.width();

  r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, composer );
  w = qMax( r.width(), w );

  r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, lyricist );
  w = qMax( r.width(), w );

  r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, author );
  w = qMax( r.width(), w );
  w += 5;
  drawPropertyImpl( w, singer, comp.singer() );
  drawPropertyImpl( w, composer, comp.composer() );
  drawPropertyImpl( w, lyricist, comp.lyricist() );
  drawPropertyImpl( w, author, comp.author() );

  return mCurY;
  }




int CsPainter::drawLine(int y, int lineIndex, const CsLine &line)
  {
  mLineIndex = lineIndex;

  //Calculate line height
  int lineHeight;
  if( line.isRemark() )
    //Remark only
    lineHeight = (mRemarkTextHeight + mSettings.mTextGap) * mVisibleRemark.count();
  else {
    //Chords
    lineHeight = (mChordTextHeight + mSettings.mTextGap) * mVisibleChord.count();
    //Notes
    lineHeight += 9 * mSettings.mScoreLineDistance * mVisibleNote.count();
    //Lyric
    lineHeight += (mLyricTextHeight + mSettings.mTextGap);
    //Translations
    lineHeight += (mTranslationTextHeight + mSettings.mTextGap) * mVisibleTranslate.count();
    }

  mCurY = y;
  if( line.isRemark() )
    drawRemark( line.remarkConst().remarkMapConst() );
  else {
    drawChord( line.taktCount(), line.chordKitConst().chordMapConst() );
    drawNote( line.taktCount(), line.noteKitConst().noteMapConst() );
    drawLyric( line.lyricListConst() );
    drawTranslation( line.translationConst() );
    mCurY += mSettings.mLineGap;
    }

  return mCurY;
  }


int CsPainter::lineRemarkHeight() const
  {
  //Remark only
  return (mRemarkTextHeight + mSettings.mTextGap) * mVisibleRemark.count();
  }



int CsPainter::lineSongHeight() const
  {
  //Chords
  int lineHeight = (mChordTextHeight + mSettings.mTextGap) * mVisibleChord.count();
  //Notes
  lineHeight += 9 * mSettings.mScoreLineDistance * mVisibleNote.count();
  //Lyric
  lineHeight += (mLyricTextHeight + mSettings.mTextGap);
  //Translations
  lineHeight += (mTranslationTextHeight + mSettings.mTextGap) * mVisibleTranslate.count();

  return lineHeight;
  }






void CsPainter::drawRemark(const QMap<QString, QString> &remarkMap)
  {
  if( mVisibleRemark.count() > 0 ) {
    mPainter->setFont( QFont(mSettings.mFontName, mSettings.mRemarkFontSize) );
    mPainter->setPen( mSettings.mColorRemark );
    }

  //For each remark translations which visible we perform drawing
  for( const auto &lang : qAsConst(mVisibleRemark) ) {
    mCurY += mRemarkTextHeight;
    drawRemarkImpl( 20, mCurY, remarkMap.value(lang) );
    mCurY += mSettings.mTextGap;
    }
  }




void CsPainter::drawChord( int taktCount, const QMap<QString, CsChordLine> &chordMap)
  {
  if( mVisibleChord.count() > 0 )
    mPainter->setFont( QFont(mSettings.mFontName, mSettings.mChordFontSize) );

  //For each chord line which visible we perform drawing
  for( const auto &chordKey : qAsConst(mVisibleChord) ) {
    drawTaktLines( taktCount, mCurY, mCurY + mChordTextHeight );
    mCurY += mChordTextHeight;
    drawChordImpl( chordMap.value(chordKey) );
    mCurY += mSettings.mTextGap;
    }
  }




void CsPainter::drawNote(int taktCount, const QMap<QString, CsNoteLine> &noteMap)
  {
  if( mVisibleNote.count() > 0 )
    mPainter->setFont( QFont( mSettings.mFontName, 4 * mSettings.mScoreLineDistance ) );

  //For each note line which visible we perform drawing
  for( const auto &noteKey : qAsConst(mVisibleNote) ) {
    drawNoteImpl( mClefMap.value(noteKey), taktCount, noteMap.value(noteKey) );
    mCurY += 9 * mSettings.mScoreLineDistance;
    }
  }




void CsPainter::drawLyric(const CsLyricList &lyricList)
  {
  mCurY += mLyricTextHeight;
  if( lyricList.count() > 0 )
    mPainter->setFont( QFont( mSettings.mFontName, mSettings.mLyricFontSize ) );

  //Paint each lyric
  for( auto const &lyric : lyricList ) {
    int visx = visualX( mLeftGap, lyric.position() );
    mPainter->drawText( visx, mCurY, lyric.lyric() );
    }
  mCurY += mSettings.mTextGap;
  }




void CsPainter::drawTranslation(const QMap<QString, QString> &translationMap)
  {
  if( mVisibleTranslate.count() > 0 ) {
    mPainter->setFont( QFont(mSettings.mFontName, mSettings.mTranslationFontSize) );
    mPainter->setPen( mSettings.mColorTranslation );
    }

  //For each translations which visible we perform drawing
  for( const auto &lang : qAsConst(mVisibleTranslate) ) {
    mCurY += mTranslationTextHeight;
    drawTranslationImpl( 20, mCurY, translationMap.value(lang) );
    mCurY += mSettings.mTextGap;
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






void CsPainter::drawChordImpl( const CsChordLine &chordLine )
  {
  auto &chordList = chordLine.chordListConst();
  //Paint each chord
  for( auto const &chord : chordList ) {
    int visx = visualX( mLeftGap, chord.position() );
    mPainter->setPen( isHighlight( chord.position(), 256 ) ? mSettings.mColorChordHighlight : mSettings.mColorChord );
    mPainter->drawText( visx, mCurY, chord.chordText() );
    }
  }




static QString unicode4( uint code )
  {
  return QString::fromUcs4( &code, 1 );
  }


static QString noteFraction( int duration, int noteDuration )
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


static QString noteText( int duration, QString &fraction )
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




void CsPainter::drawNoteImpl( int clef, int taktCount, const CsNoteLine &noteLine)
  {
  int scoreY = mCurY + mSettings.mScoreLineDistance * 2;

  //Draw takt lines
  drawTaktLines( taktCount, scoreY, mCurY + 6 * mSettings.mScoreLineDistance );

  mPainter->setPen( mSettings.mColorNote );
  //Draw five lines of score
  for( int i = 0; i < 5; i++ )
    mPainter->drawLine( mClefPos - 5, scoreY + i * mSettings.mScoreLineDistance, visualX( mLeftGap, taktCount * 256) + 5, scoreY + i * mSettings.mScoreLineDistance );

  //Note abowe line 0
  int noteStart = whiteOctaveFirst + whiteC;
  //Clef
  if( clef == noteG ) {
    mPainter->drawText( mClefPos, scoreY + 4 * mSettings.mScoreLineDistance, unicode4(119070) );
    noteStart = whiteOctaveSecond + whiteG;
    }
  else if( clef == noteF ) {
    mPainter->drawText( mClefPos, scoreY + 4 * mSettings.mScoreLineDistance, unicode4(119074) );
    noteStart = whiteOctaveSmall + whiteB;
    }
  else if( clef == noteC ) {
    mPainter->drawText( mClefPos, scoreY + 4 * mSettings.mScoreLineDistance, unicode4(119073) );
    noteStart = whiteOctaveFirst + whiteB;
    }

  //Draw notes
  auto &noteList = noteLine.noteListGet();
  for( auto const &note : qAsConst(noteList) ) {
    mPainter->setPen( mPlayer.isHit( note.position(), note.duration() ) ? mSettings.mColorNoteHighlight : mSettings.mColorNote );
    int visX = visualX( mLeftGap, note.position() );
    int yOffset = noteStart - note.white();
    int yPos = scoreY + yOffset * mSettings.mScoreLineDistance / 2;
    if( yOffset <= -2 ) {
      //Appended line above must be drawn
      mPainter->drawLine( visX, scoreY - mSettings.mScoreLineDistance, visX + 20, scoreY - mSettings.mScoreLineDistance );
      }
    else if( yOffset > 10 ) {
      //Appended line below must be drawn
      mPainter->drawLine( visX, scoreY + 5 * mSettings.mScoreLineDistance, visX + 20, scoreY + 5 * mSettings.mScoreLineDistance );
      if( yOffset > 12 )
        //Second appended line below must be drawn
        mPainter->drawLine( visX, scoreY + 6 * mSettings.mScoreLineDistance, visX + 20, scoreY + 6 * mSettings.mScoreLineDistance );
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
      m *= QTransform::fromTranslate( visX + r.width(), yPos + 1 - mSettings.mScoreLineDistance );
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
  }





void CsPainter::drawTranslationImpl(int x, int y, const QString &tran)
  {
  //For empty string nothing done
  if( tran.isEmpty() )
    return;

  //Draw simple text
  mPainter->drawText( x, y, tran );
  }




void CsPainter::drawPropertyImpl(int xtab, const QString &title, const QString &value)
  {
  mCurY += mPropertiesHeight;
  mPainter->drawText( 10, mCurY, title );
  mPainter->drawText( 10 + xtab, mCurY, value );
  mCurY += mSettings.mTextGap;
  }



void CsPainter::drawTaktLines(int taktCount, int y0, int y1)
  {
  if( taktCount ) {
    mPainter->setPen( mSettings.mColorTakt );
    for( int i = 0; i < taktCount + 1; i++ ) {
      int x = visualX( mLeftGap, i * 256 );
      mPainter->drawLine( x, y0, x, y1 );
      }
    }
  }



int CsPainter::visualX(int x, int pos)
  {
  return x + (pos * mSettings.mPixelPerTakt >> 8);
  }




int CsPainter::fontHeight(int fontSize) const
  {
  mPainter->setFont( QFont( mSettings.mFontName, fontSize ) );
  QRect r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, QStringLiteral("H") );
  return r.height();
  }



bool CsPainter::isHighlight(int position, int duration) const
  {
  return isPlayerOnCurrentLine() && mPlayer.isHit( position, duration );
  }
