#include "CsPainter.h"

#include <QTransform>
#include <QSettings>
#include <QJsonDocument>

CsPainter::CsPainter(QPainter *painter, const QString &keyViewSettings, const CsComposition &comp, const CsPlay &player , int offsetX, QSize size, CsCellCursor *cellCursor) :
  mPainter(painter),
  mPlayer(player),
  mVisibleRemark(comp.remarkVisible()),
  mVisibleChord(comp.chordVisible()),
  mVisibleNote(comp.noteVisible()),
  mVisibleTranslate(comp.translationVisible()),
  mClefMap(comp.noteClefMap()),
  mCellCursor(cellCursor),
  mOffsetX(offsetX),
  mSize(size)
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
  mClefPos = mSettings.mLeftMenuSize + 15 - mOffsetX;
  mDenominatorPos = mClefPos + 20 - mOffsetX;
  mLeftGap = mDenominatorPos + 20 - mOffsetX;

  //Step width in pixels
  mStepPixChord = visualX( 0, mStepChord = comp.stepChord() );
  mStepPixNote  = visualX( 0, mStepNote = comp.stepNote() );
  mStepPixLyric = visualX( 0, mStepLyric = comp.stepLyric() );
  }




int CsPainter::drawTitleAndProperties(int y, const CsComposition &comp)
  {
  //Draw title and properties
  mCurY = y;

  //Draw title on horizontal center
  mPainter->setFont( QFont(mSettings.mFontName, mSettings.mTitleFontSize) );
  QRect r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, comp.title() );
  int x = (mSize.width() - r.width()) / 2;
  drawCellProperty( x - mOffsetX, y, comp.title(), mTitleHeight, cccTitle );
  if( isNotEditProperty( cccTitle, x - mOffsetX, y + mTitleHeight )  )
    mPainter->drawText( x - mOffsetX, y + mTitleHeight, comp.title() );
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
  drawPropertyImpl( mClefPos, w, singer, comp.singer(), cccSinger );
  drawPropertyImpl( mClefPos, w, composer, comp.composer(), cccComposer );
  drawPropertyImpl( mClefPos, w, lyricist, comp.lyricist(), cccLyricist );
  drawPropertyImpl( mClefPos, w, author, comp.author(), cccAuthor );

  return mCurY;
  }




int CsPainter::drawLine(int y, int lineIndex, const CsLine &line, bool fullDrawing )
  {
  mLineIndex = lineIndex;

  //Calculate line height
  int lineHeight;
  int fullLineHeight;
  int playMarkPositionHeight = 0;
  if( line.isRemark() )
    //Remark only
    fullLineHeight = lineHeight = lineRemarkHeight();
  else {
    //Chords
    lineHeight = (mChordTextHeight + mSettings.mTextGap) * mVisibleChord.count();
    //Notes
    lineHeight += 9 * mSettings.mScoreLineDistance * mVisibleNote.count();
    playMarkPositionHeight = lineHeight;
    //Lyric
    lineHeight += (mLyricTextHeight + mSettings.mTextGap);
    //Translations
    lineHeight += (mTranslationTextHeight + mSettings.mTextGap) * mVisibleTranslate.count();
    fullLineHeight = lineHeight + mSettings.mLineGap;
    }

  if( !fullDrawing && ((y < 0 && (y + lineHeight) < 0) || (y > mSize.height())) )
    //All drawing is outside viewport
    return y + fullLineHeight;

  mCurY = y;
  if( line.isRemark() )
    drawRemark( line.remarkConst().remarkMapConst() );
  else {
    //Draw cursor position
    drawPlayPosition( playMarkPositionHeight );
    //Draw line content
    drawChord( line.taktCount(), line.chordKitConst().chordMapConst() );
    drawNote( line.taktCount(), line.noteKitConst().noteMapConst() );
    drawCellLyric( mCurY, line.taktCount() * 256 );
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




bool CsPainter::isNotEditProperty(int propertyId, int x, int y)
  {
  Q_UNUSED(propertyId)
  Q_UNUSED(x)
  Q_UNUSED(y)
  return true;
  }



bool CsPainter::isNotEditRemark(const QString &part, int x, int y)
  {
  Q_UNUSED(part)
  Q_UNUSED(x)
  Q_UNUSED(y)
  return true;
  }



bool CsPainter::isNotEditChord(const QString &part, int position, int x, int y)
  {
  Q_UNUSED(part)
  Q_UNUSED(position)
  Q_UNUSED(x)
  Q_UNUSED(y)
  return true;
  }



bool CsPainter::isNotEditNote(const QString &part, int position, int x, int y)
  {
  Q_UNUSED(part)
  Q_UNUSED(position)
  Q_UNUSED(x)
  Q_UNUSED(y)
  return true;
  }




bool CsPainter::isNotEditTranslation(const QString &part, int x, int y)
  {
  Q_UNUSED(part)
  Q_UNUSED(x)
  Q_UNUSED(y)
  return true;
  }






void CsPainter::drawRemark(const QMap<QString, QString> &remarkMap)
  {
  if( mVisibleRemark.count() > 0 ) {
    mPainter->setFont( QFont(mSettings.mFontName, mSettings.mRemarkFontSize) );
    mPainter->setPen( mSettings.mColorRemark );
    }

  //For each remark translations which visible we perform drawing
  for( const auto &lang : qAsConst(mVisibleRemark) ) {
    if( mCellCursor != nullptr )
      drawCellText( mLeftGap, mCurY, remarkMap.value(lang), mTranslationTextHeight,
                    mCellCursor->isMatch( cccRemark, mLineIndex, lang ) );

    mCurY += mRemarkTextHeight;
    drawRemarkImpl( mLeftGap, mCurY, remarkMap.value(lang) );
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
    drawCellChord( mCurY, taktCount * 256, chordKey );
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
    drawCellNote( mCurY, taktCount * 256, noteKey );
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
    mPainter->setPen( isHighlight( lyric.position(), lyric.duration() ) ? mSettings.mColorLyricHighlight : mSettings.mColorLyric );
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
    if( mCellCursor != nullptr )
      drawCellText( mLeftGap, mCurY, translationMap.value(lang), mTranslationTextHeight,
                    mCellCursor->isMatch( cccTranslation, mLineIndex, lang ) );

    mCurY += mTranslationTextHeight;
    drawTranslationImpl( mLeftGap, mCurY, translationMap.value(lang) );
    mCurY += mSettings.mTextGap;
    }
  }



void CsPainter::drawPlayPosition( int markHeight )
  {
  if( !isPlayerOnCurrentLine() || markHeight <= 0 )
    return;

  int posx = visualX( mLeftGap, mPlayer.lineTickIndex() );
  mPainter->setPen( QPen( QBrush(mSettings.mPlayMarkColor), mSettings.mPlayMarkWidth ) );
  mPainter->drawLine( posx, mCurY, posx, mCurY + markHeight );
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
    mPainter->setPen( isHighlight( chord.position(), chord.duration() ) ? mSettings.mColorChordHighlight : mSettings.mColorChord );
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




void CsPainter::drawPropertyImpl(int xorigin, int xtab, const QString &title, const QString &value, int propertyId)
  {
  mCurY += mPropertiesHeight;
  mPainter->drawText( xorigin, mCurY, title );
  drawCellProperty( xorigin + xtab, mCurY - mPropertiesHeight, value, mPropertiesHeight, propertyId );
  if( isNotEditProperty( propertyId, xorigin + xtab, mCurY )  )
    mPainter->drawText( xorigin + xtab, mCurY, value );
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










void CsPainter::drawCellProperty(int x, int y, const QString &value, int height, int propertyId)
  {
  if( mCellCursor == nullptr )
    return;

  drawCellText( x, y, value, height, mCellCursor->cellClass() == propertyId );
  }




void CsPainter::drawCellText(int x, int y, const QString &value, int height, bool isCurrent)
  {
  //Calculate width of cell. Width is max of width of value string and 50 pixels
  int width = 50;
  if( !value.isEmpty() ) {
    QRect r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, value );
    if( r.width() > 50 )
      width = r.width();
    }

  drawCell( x, y, width, height, isCurrent );
  }



void CsPainter::drawCellChord(int y, int tickCount, const QString &part )
  {
  if( mCellCursor == nullptr )
    return;

  for( int tick = 0; tick < tickCount; tick += mStepChord )
    drawCell( visualX( mLeftGap, tick ), y, mStepPixChord, mChordTextHeight,
              mCellCursor->isMatch( cccChord, tick, mLineIndex, part ) );
  }




void CsPainter::drawCellNote(int y, int tickCount, const QString &part)
  {
  if( mCellCursor == nullptr )
    return;

  for( int tick = 0; tick < tickCount; tick += mStepNote )
    drawCell( visualX( mLeftGap, tick ), y, mStepPixNote, 9 * mSettings.mScoreLineDistance,
              mCellCursor->isMatch( cccNote, tick, mLineIndex, part ) );
  }





void CsPainter::drawCellLyric(int y, int tickCount)
  {
  if( mCellCursor == nullptr )
    return;

  for( int tick = 0; tick < tickCount; tick += mStepLyric )
    drawCell( visualX( mLeftGap, tick ), y, mStepPixLyric, mLyricTextHeight,
              mCellCursor->isMatch( cccLyric, tick, mLineIndex ) );
  }




void CsPainter::drawCell(int x, int y, int width, int height, bool isCurrent)
  {
  if( isCurrent ) {
    //Draw current cell background
    mPainter->setPen( Qt::transparent );
    mPainter->setBrush( mSettings.mColorCellCurrent );
    mPainter->drawRect( x, y, width, height );
    mCellCursorRect.setRect( x, y, width, height );
    }
  else {
    //Cell is not current
    //Draw cell bound
    mPainter->setPen( QPen(mSettings.mColorGrid, 1, Qt::DotLine) );
    mPainter->setBrush( Qt::transparent );
    mPainter->drawRect( x, y, width, height );
    //qDebug() << "rect" << x << y << width <<  height;
    }

  mPainter->setPen( Qt::black );

  }
