#include "CsPainter.h"

#include <QTransform>
#include <QSettings>
//#include <QJsonDocument>
#include <QVector>

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
    drawLyric( line.lyricLineConst() );
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



bool CsPainter::isNotEditNote(const QString &part, int position, int x, int scoreY, int noteStart)
  {
  Q_UNUSED(part)
  Q_UNUSED(position)
  Q_UNUSED(x)
  Q_UNUSED(scoreY)
  Q_UNUSED(noteStart)
  return true;
  }




bool CsPainter::isNotEditLyric(QVector<CsLyricDisposition> &disposition)
  {
  Q_UNUSED(disposition)
  return true;
  }




bool CsPainter::isNotEditTranslation(const QString &part, int x, int y)
  {
  Q_UNUSED(part)
  Q_UNUSED(x)
  Q_UNUSED(y)
  return true;
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






QRect CsPainter::drawNoteSingle(int x, int scoreY, int noteStart, int noteWhite, int noteDuration, bool noteDies)
  {
  int yOffset = noteStart - noteWhite;
  int yPos = scoreY + yOffset * mSettings.mScoreLineDistance / 2;
  if( yOffset <= -2 ) {
    //Appended line above must be drawn
    mPainter->drawLine( x, scoreY - mSettings.mScoreLineDistance, x + 20, scoreY - mSettings.mScoreLineDistance );
    }
  else if( yOffset > 10 ) {
    //Appended line below must be drawn
    mPainter->drawLine( x, scoreY + 5 * mSettings.mScoreLineDistance, x + 20, scoreY + 5 * mSettings.mScoreLineDistance );
    if( yOffset > 12 )
      //Second appended line below must be drawn
      mPainter->drawLine( x, scoreY + 6 * mSettings.mScoreLineDistance, x + 20, scoreY + 6 * mSettings.mScoreLineDistance );
    }

  QRect over;
  if( yOffset < 5 ) {
    //Rotate symbol
    //Create text with mirror order
    QString fraction;
    QString noteSign = noteText( noteDuration, fraction );
    QString fullNote = fraction + noteSign;
    if( noteDies )
      fullNote.append( QStringLiteral(" ") + unicode4(9839) );

    QRect r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, fullNote );
    //At frist - rotation
    QTransform m = QTransform::fromScale( 1.0, 1.0 ).rotate( -180 );

    //At second - origin offset
    m *= QTransform::fromTranslate( x + r.width(), yPos + 1 - mSettings.mScoreLineDistance );
    mPainter->setTransform( m );
    mPainter->drawText( 0, 0, fullNote );
    over = m.mapRect( r );
    mPainter->resetTransform();
    }
  else {
    //Simple text
    QString fraction;
    QString noteSign = noteText( noteDuration, fraction );
    QString fullNote;
    if( noteDies )
      fullNote = unicode4(9839);
    fullNote.append( noteSign + QStringLiteral(" ") + fraction );
    mPainter->drawText( x, yPos, fullNote );
    over = mPainter->boundingRect( x,yPos, 0,0, Qt::AlignLeft | Qt::AlignTop, fullNote );
    over.moveTop( yPos - over.height() );
    }
  return over;
  }







void CsPainter::buildDisposition(QVector<CsLyricDisposition> &disposition, const CsLyricLine &lyricLine)
  {
  //Preserve amount of internal array
  if( lyricLine.count() > disposition.count() )
    disposition.resize( lyricLine.count() );


  //At first, we calculate width of each symbol
  int curX = visualX( mLeftGap, 0 );
  int pos = 0;
  for( int i = 0; i < lyricLine.count(); i++ ) {
    if( lyricLine.at(i).isAlign() ) {
      pos = (pos & ~0xff) + lyricLine.at(i).align();
      curX = visualX( mLeftGap, pos );
      disposition[i].mWidth = 2;
      //Align all previous non-delimiter symbols to right
      int prevX = curX;
      for( int k = i - 1; k >= 0 && !lyricLine.at(k).isDelimiter(); k-- ) {
        prevX -= disposition[k].mWidth;
        disposition[k].mPosX = prevX;
        }
      }
    else {
      QRect r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, lyricLine.at(i).string() );
      disposition[i].mWidth = r.width();
      }
    disposition[i].mPosX = curX;
    curX += disposition[i].mWidth;
    disposition[i].mHighlight = isHighlight( pos, 256 - (pos & 0xff) );
    }
  }






int CsPainter::visualX(int x, int pos)
  {
  return x + (pos * mSettings.mPixelPerTakt >> 8);
  }






void CsPainter::drawRemark(const QMap<QString, QString> &remarkMap)
  {
  if( mVisibleRemark.count() > 0 ) {
    mPainter->setFont( QFont(mSettings.mFontName, mSettings.mRemarkFontSize) );
    mPainter->setPen( mSettings.mColorRemark );
    }

  //For each remark translations which visible we perform drawing
  for( const auto &lang : qAsConst(mVisibleRemark) ) {
    if( mCellCursor != nullptr ) {
      int width = drawCellText( mLeftGap, mCurY, remarkMap.value(lang), mTranslationTextHeight,
                    mCellCursor->isMatch( cccRemark, mLineIndex, lang ) );
      mReferenceList.append( CsReference( mLeftGap, mCurY, width, mTranslationTextHeight,
                                          cccRemark, mLineIndex, lang, 0 ) );
      }

    mCurY += mRemarkTextHeight;
    if( isNotEditRemark( lang, mLeftGap, mCurY ) )
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
    drawChordImpl( chordKey, chordMap.value(chordKey) );
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
    drawNoteImpl( mClefMap.value(noteKey), taktCount, noteKey, noteMap.value(noteKey) );
    mCurY += 9 * mSettings.mScoreLineDistance;
    }
  }









void CsPainter::drawLyric(const CsLyricLine &lyricLine)
  {
  static QVector<CsLyricDisposition> lyricDisposition(1024);

  mCurY += mLyricTextHeight;
  if( lyricLine.count() > 0 )
    mPainter->setFont( QFont( mSettings.mFontName, mSettings.mLyricFontSize ) );

  if( isNotEditLyric( lyricDisposition ) ) {
    buildDisposition( lyricDisposition, lyricLine );

    //Paint each lyric symbol
    for( int i = 0; i < lyricLine.count(); i++ ) {
      mPainter->setPen( lyricDisposition[i].mHighlight ? mSettings.mColorLyricHighlight : mSettings.mColorLyric );
      int x = lyricDisposition[i].mPosX;
      if( lyricLine.at(i).isAlign() ) {
        //Draw align line
        mPainter->drawLine( x, mCurY, x, mCurY - mLyricTextHeight );
        }
      else
        mPainter->drawText( x, mCurY, lyricLine.at(i).string() );
      }

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
    if( mCellCursor != nullptr ) {
      int width = drawCellText( mLeftGap, mCurY, translationMap.value(lang), mTranslationTextHeight,
                    mCellCursor->isMatch( cccTranslation, mLineIndex, lang ) );
      mReferenceList.append( CsReference( mLeftGap, mCurY, width, mTranslationTextHeight,
                                          cccTranslation, mLineIndex, lang, 0 ) );
      }

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






void CsPainter::drawChordImpl( const QString &part, const CsChordLine &chordLine )
  {
  auto &chordList = chordLine.chordListConst();
  //Paint each chord
  for( auto const &chord : chordList ) {
    int visx = visualX( mLeftGap, chord.position() );
    mPainter->setPen( isHighlight( chord.position(), chord.duration() ) ? mSettings.mColorChordHighlight : mSettings.mColorChord );
    if( isNotEditChord( part, chord.position(), visx, mCurY ) )
      mPainter->drawText( visx, mCurY, chord.chordText() );
    }
  }








void CsPainter::drawNoteImpl( int clef, int taktCount, const QString &part, const CsNoteLine &noteLine)
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
    if( isNotEditNote( part, note.position(), visX, scoreY, noteStart ) )
      drawNoteSingle( visX, scoreY, noteStart, note.white(), note.duration(), note.isDies() );
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

  int width = drawCellText( x, y, value, height, mCellCursor->cellClass() == propertyId );

  mReferenceList.append( CsReference( x, y, width, height, propertyId ) );
  }




int CsPainter::drawCellText(int x, int y, const QString &value, int height, bool isCurrent)
  {
  //Calculate width of cell. Width is max of width of value string and 50 pixels
  int width = 50;
  if( !value.isEmpty() ) {
    QRect r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, value );
    if( r.width() > 50 )
      width = r.width();
    }

  drawCell( x, y, width, height, isCurrent );

  return width;
  }



void CsPainter::drawCellChord(int y, int tickCount, const QString &part )
  {
  if( mCellCursor == nullptr )
    return;

  for( int tick = 0; tick < tickCount; tick += mStepChord ) {
    int x = visualX( mLeftGap, tick );
    drawCell( x, y, mStepPixChord, mChordTextHeight,
              mCellCursor->isMatch( cccChord, tick, mLineIndex, part ) );
    mReferenceList.append( CsReference( x, y, mStepPixChord, mChordTextHeight, cccChord, mLineIndex, part, tick ) );
    }
  }




void CsPainter::drawCellNote(int y, int tickCount, const QString &part)
  {
  if( mCellCursor == nullptr )
    return;

  for( int tick = 0; tick < tickCount; tick += mStepNote ) {
    int x = visualX( mLeftGap, tick );
    drawCell( x, y, mStepPixNote, 9 * mSettings.mScoreLineDistance,
              mCellCursor->isMatch( cccNote, tick, mLineIndex, part ) );
    mReferenceList.append( CsReference( x, y, mStepPixNote, 9 * mSettings.mScoreLineDistance, cccNote, mLineIndex, part, tick ) );
    }
  }





void CsPainter::drawCellLyric(int y, int tickCount)
  {
  if( mCellCursor == nullptr )
    return;

  for( int tick = 0; tick < tickCount; tick += mStepLyric ) {
    int x = visualX( mLeftGap, tick );
    drawCell( x, y, mStepPixLyric, mLyricTextHeight,
              mCellCursor->isMatch( cccLyric, tick, mLineIndex ) );
    mReferenceList.append( CsReference( x, y, mStepPixLyric, mLyricTextHeight, cccLyric, mLineIndex, QString{}, tick ) );
    }
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
