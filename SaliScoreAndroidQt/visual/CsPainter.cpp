#include "CsPainter.h"

#include <QTransform>
#include <QSettings>
//#include <QJsonDocument>
#include <QVector>
#include <QFontMetrics>
#include <QtSvg/QSvgRenderer>

CsPainter::CsPainter(QPainter *painter, const QString &keyViewSettings, const CsComposition &comp, const CsCursorPosition *player , int offsetX, QSize size, CsCellCursor *cellCursor) :
  mPainter(painter),
  mPlayer(player),
  mVisibleRemark(comp.remarkVisible()),
  mVisibleChord(comp.chordVisible()),
  mVisibleNote(comp.noteVisible()),
  mVisibleTranslate(comp.translationVisible()),
  mLineStartOffset(comp.lineStartOffset()),
  mCellCursor(cellCursor),
  mOffsetX(offsetX),
  mSize(size)
  {
  QSettings s;
  if( s.contains(keyViewSettings) ) {
    }

  mNumerator = QString::number( comp.partPerTakt() );
  mDenominator = QString::number( comp.denominator() );

  //Calculate font height
  mRemarkTextHeight      = fontHeight( mSettings.mRemarkFontSize );
  mChordTextHeight       = fontHeight( mSettings.mChordFontSize );
  mLyricTextHeight       = fontHeight( mSettings.mLyricFontSize );
  mTranslationTextHeight = fontHeight( mSettings.mTranslationFontSize );
  mTitleHeight           = fontHeight( mSettings.mTitleFontSize );
  mPropertiesHeight      = fontHeight( mSettings.mPropertiesFontSize );

  //Get tick per part
  mTickPerTakt = comp.tickPerTakt();
  if( mTickPerTakt <= 0 )
    mTickPerTakt = 256;

  //First takt offset
  mClefPos = mSettings.mLeftMenuSize + 25 - mOffsetX;
  mDenominatorPos = mClefPos + 25;
  mLeftGap = mDenominatorPos + 15;

  //Step width in pixels
  mStepPixChord = visualX( 0, mStepChord = comp.stepChord() );
  mStepPixNote  = visualX( 0, mStepNote = comp.stepNote() );
  mStepPixLyric = visualX( 0, mStepLyric = comp.stepLyric() );

  //Fill clef map
  for( auto &part : qAsConst(mVisibleNote) )
    mClefMap.insert( part, comp.noteClefGet(part) );
  }




void CsPainter::fillBackground()
  {
  mPainter->fillRect( QRect( QPoint(), mSize ), mSettings.mColorBackground );
  }




int CsPainter::drawTitleAndProperties(int y, const CsComposition &comp)
  {
  QMap<QString,QString> attrTitles;
  //Build default titles
  attrTitles.insert( CS_ATTR_SINGER, QObject::tr("Singer:") );
  attrTitles.insert( CS_ATTR_COMPOSER, QObject::tr("Composer:") );
  attrTitles.insert( CS_ATTR_LYRICIST, QObject::tr("Lyricist:") );
  attrTitles.insert( CS_ATTR_AUTHOR, QObject::tr("Author:") );

  attrTitles.insert( CS_ATTR_VOICE, QObject::tr("Voice:") );
  attrTitles.insert( CS_ATTR_VOICE_DUAL, QObject::tr("Voice dual:") );
  attrTitles.insert( CS_ATTR_VOICE_LEFT, QObject::tr("Voice left:") );
  attrTitles.insert( CS_ATTR_STYLE, QObject::tr("Style:") );
  attrTitles.insert( CS_ATTR_TEMPO, QObject::tr("Tempo:") );

  //Draw title and properties
  mCurY = y;

  //Draw title on horizontal center
  mPainter->setFont( QFont(mSettings.mFontName, mSettings.mTitleFontSize) );
  QString title = comp.attributeGet(CS_ATTR_NAME);
  QRect r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, title );
  int x = (mSize.width() - r.width()) / 2;
  drawCellProperty( x - mOffsetX, y + mTitleHeight.mOffset, title, mTitleHeight.mHeight, CS_ATTR_NAME );
  if( isNotEditProperty( CS_ATTR_NAME, x - mOffsetX, y + mTitleHeight.mHeight )  )
    mPainter->drawText( x - mOffsetX, y + mTitleHeight.mHeight, title );
  mCurY += mTitleHeight.mHeight + mSettings.mTextGap;

  //At left side properties
  mPainter->setFont( QFont(mSettings.mFontName, mSettings.mPropertiesFontSize) );
  QStringList attrList( { CS_ATTR_SINGER, CS_ATTR_COMPOSER, CS_ATTR_LYRICIST, CS_ATTR_AUTHOR } );
  int w = 0;
  for( auto &attrId : qAsConst(attrList) ) {
    r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, attrTitles.value(attrId) );
    w = qMax( r.width(), w );
    }
  w += 5;
  int storedY = mCurY;
  for( auto &attrId : qAsConst(attrList) )
    drawPropertyImpl( mClefPos, w, attrTitles.value(attrId), comp.attributeGet(attrId), attrId );

  //Restore Y to draw right side of properties
  mCurY = storedY;

  //At right side properties
  attrList = QStringList( { CS_ATTR_VOICE, CS_ATTR_VOICE_DUAL, CS_ATTR_VOICE_LEFT, CS_ATTR_STYLE, CS_ATTR_TEMPO } );
  w = 0;
  for( auto &attrId : qAsConst(attrList) ) {
    r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, attrTitles.value(attrId) );
    w = qMax( r.width(), w );
    }

  int rightX = mSize.width() - w * 2;
  for( auto &attrId : qAsConst(attrList) )
    drawPropertyImpl( rightX, w, attrTitles.value(attrId), comp.attributeGet(attrId), attrId );

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
    lineHeight = (mChordTextHeight.mHeight + mSettings.mTextGap) * mVisibleChord.count();
    //Notes
    lineHeight += 9 * mSettings.mScoreLineDistance * mVisibleNote.count();
    playMarkPositionHeight = lineHeight;
    //Lyric
    lineHeight += (mLyricTextHeight.mHeight + mSettings.mTextGap);
    //Translations
    lineHeight += (mTranslationTextHeight.mHeight + mSettings.mTextGap) * mVisibleTranslate.count();
    fullLineHeight = lineHeight + mSettings.mLineGap;
    }

  if( !fullDrawing && ((y < 0 && (y + lineHeight) < 0) || (y > mSize.height())) )
    //All drawing is outside viewport
    return y + fullLineHeight;

  mCurY = y;

  //Fill line background
  drawLineBackground( fullLineHeight );

  //Draw menu zone
  if( mCellCursor != nullptr )
    drawCell( -mOffsetX, y, mSettings.mLeftMenuSize, fullLineHeight, false );
  mReferenceList.append( CsReference( -mOffsetX, y, mSettings.mLeftMenuSize, fullLineHeight, cccLineSelect, mLineIndex, QString{}, 0 ) );

  if( line.isRemark() )
    drawRemarkLine( line );
  else {
    //Draw cursor position
    drawPlayPosition( playMarkPositionHeight );
    //Draw line content
    drawChord( line.taktCount(), line );
    drawNote( line.taktCount(), line );
    drawCellLyric( mCurY, line.taktCount() * mTickPerTakt );
    drawLyric( line.lyricGet() );
    drawTranslation( line );
    mCurY += mSettings.mLineGap;
    }

  return mCurY;
  }


int CsPainter::lineRemarkHeight() const
  {
  //Remark only
  return (mRemarkTextHeight.mHeight + mSettings.mTextGap) * mVisibleRemark.count();
  }



int CsPainter::lineSongHeight() const
  {
  //Chords
  int lineHeight = (mChordTextHeight.mHeight + mSettings.mTextGap) * mVisibleChord.count();
  //Notes
  lineHeight += 9 * mSettings.mScoreLineDistance * mVisibleNote.count();
  //Lyric
  lineHeight += (mLyricTextHeight.mHeight + mSettings.mTextGap);
  //Translations
  lineHeight += (mTranslationTextHeight.mHeight + mSettings.mTextGap) * mVisibleTranslate.count();

  return lineHeight;
  }




bool CsPainter::isNotEditProperty( const QString &propertyId, int x, int y)
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



//!
//! \brief isNotEditNote Test if this position not edited
//! \param part          Part of score line
//! \param position      Time position inside line
//! \param x             x position of note in pixels
//! \param scoreY        y position of score line
//! \param noteStart     base note of score line (clef)
//! \return              true if this position not edited
//!
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


static QSvgRenderer *noteSvg( bool up, int duration, QString &fraction )
  {
  //At now we use svg pictures for display notes because on android font display incorrect
  //Note pictures
  //Up notes
  static QSvgRenderer note128( QStringLiteral(":/pic/Music-hundredtwentyeighthnote.svg") );
  static QSvgRenderer note64( QStringLiteral(":/pic/Music-sixtyfourthnote.svg") );
  static QSvgRenderer note32( QStringLiteral(":/pic/Music-thirtysecondnote.svg") );
  static QSvgRenderer note16( QStringLiteral(":/pic/Music-sixteenthnote.svg") );
  static QSvgRenderer note8( QStringLiteral(":/pic/Music-eighthnote.svg") );
  static QSvgRenderer note4( QStringLiteral(":/pic/Music-quarternote.svg") );
  static QSvgRenderer note2( QStringLiteral(":/pic/Music-halfnote.svg") );
  static QSvgRenderer note1( QStringLiteral(":/pic/Music-wholenote.svg") );
  static QSvgRenderer noteX2( QStringLiteral(":/pic/Music-doublewholenote.svg") );
  //Down notes
  static QSvgRenderer noteD128( QStringLiteral(":/pic/MusicDnHundredtwentyeighthnote.svg") );
  static QSvgRenderer noteD64( QStringLiteral(":/pic/MusicDnSixtyfourthnote.svg") );
  static QSvgRenderer noteD32( QStringLiteral(":/pic/MusicDnThirtysecondnote.svg") );
  static QSvgRenderer noteD16( QStringLiteral(":/pic/MusicDnSixteenthnote.svg") );
  static QSvgRenderer noteD8( QStringLiteral(":/pic/MusicDnEighthnote.svg") );
  static QSvgRenderer noteD4( QStringLiteral(":/pic/MusicDnQuarternote.svg") );
  static QSvgRenderer noteD2( QStringLiteral(":/pic/MusicDnHalfnote.svg") );
  static QSvgRenderer noteD1( QStringLiteral(":/pic/MusicDnWholenote.svg") );
  static QSvgRenderer noteDX2( QStringLiteral(":/pic/MusicDnDoublewholenote.svg") );
  static bool first = true;
  if( first ) {
    //At first time we adjust each svg display mode
    first = false;
    note128.setAspectRatioMode( Qt::KeepAspectRatio );
    note64.setAspectRatioMode( Qt::KeepAspectRatio );
    note32.setAspectRatioMode( Qt::KeepAspectRatio );
    note16.setAspectRatioMode( Qt::KeepAspectRatio );
    note8.setAspectRatioMode( Qt::KeepAspectRatio );
    note4.setAspectRatioMode( Qt::KeepAspectRatio );
    note2.setAspectRatioMode( Qt::KeepAspectRatio );
    note2.setAspectRatioMode( Qt::KeepAspectRatio );
    note1.setAspectRatioMode( Qt::KeepAspectRatio );
    noteX2.setAspectRatioMode( Qt::KeepAspectRatio );

    noteD128.setAspectRatioMode( Qt::KeepAspectRatio );
    noteD64.setAspectRatioMode( Qt::KeepAspectRatio );
    noteD32.setAspectRatioMode( Qt::KeepAspectRatio );
    noteD16.setAspectRatioMode( Qt::KeepAspectRatio );
    noteD8.setAspectRatioMode( Qt::KeepAspectRatio );
    noteD4.setAspectRatioMode( Qt::KeepAspectRatio );
    noteD2.setAspectRatioMode( Qt::KeepAspectRatio );
    noteD2.setAspectRatioMode( Qt::KeepAspectRatio );
    noteD1.setAspectRatioMode( Qt::KeepAspectRatio );
    noteDX2.setAspectRatioMode( Qt::KeepAspectRatio );
    }

  if( up ) {
    switch( duration ) {
      case duraBreve : return &noteX2;
      case duraHole : return &note1;
      case duraHalf : return &note2;
      case duraQuarter : return &note4;
      case duraEighth : return &note8;
      case duraSixteenth : return &note16;
      case duraThirtySecond : return &note32;
      case duraSixtyFourth : return &note64;
      case duraOneHundredTwentyEighth : return &note128;
      }
    }
  else {
    switch( duration ) {
      case duraBreve : return &noteDX2;
      case duraHole : return &noteD1;
      case duraHalf : return &noteD2;
      case duraQuarter : return &noteD4;
      case duraEighth : return &noteD8;
      case duraSixteenth : return &noteD16;
      case duraThirtySecond : return &noteD32;
      case duraSixtyFourth : return &noteD64;
      case duraOneHundredTwentyEighth : return &noteD128;
      }
    }

  if( duration > duraBreve ) {
    fraction = noteFraction( duration, duraBreve );
    return noteSvg( up, duraBreve, fraction );
    }
  if( duration > duraHole ) {
    fraction = noteFraction( duration, duraHole );
    return noteSvg( up, duraHole, fraction );
    }
  if( duration > duraHalf ) {
    fraction = noteFraction( duration, duraHalf );
    return noteSvg( up, duraHalf, fraction );
    }
  if( duration > duraQuarter ) {
    fraction = noteFraction( duration, duraQuarter );
    return noteSvg( up, duraQuarter, fraction );
    }
  if( duration > duraEighth ) {
    fraction = noteFraction( duration, duraEighth );
    return noteSvg( up, duraEighth, fraction );
    }
  if( duration > duraSixteenth ) {
    fraction = noteFraction( duration, duraSixteenth );
    return noteSvg( up, duraSixteenth, fraction );
    }
  if( duration > duraThirtySecond ) {
    fraction = noteFraction( duration, duraThirtySecond );
    return noteSvg( up, duraThirtySecond, fraction );
    }
  if( duration > duraSixtyFourth ) {
    fraction = noteFraction( duration, duraSixtyFourth );
    return noteSvg( up, duraSixtyFourth, fraction );
    }
  fraction = noteFraction( duration, duraOneHundredTwentyEighth );
  return noteSvg( up, duraOneHundredTwentyEighth, fraction );
  }







//!
//! \brief drawNoteSingle Draws single note
//! \param x              x position of note in pixel
//! \param scoreY         y position of note score line
//! \param noteStart      note of bottom score line
//! \param noteWhite      note white key index (black notes display with sharp)
//! \param noteDuration   note duration
//! \param noteDies       note dies
//! \param opasity        opasity of display
//! \return               rectangle of note
//!
QRect CsPainter::drawNoteSingle(int x, int scoreY, int noteStart, int noteWhite, int noteDuration, bool noteDies, bool opacity )
  {
  int yOffset = noteStart - noteWhite;
  int yPos = scoreY + yOffset * mSettings.mScoreLineDistance / 2;
  if( yOffset <= -1 ) {
    //Appended line above must be drawn
    mPainter->drawLine( x - 5, scoreY - mSettings.mScoreLineDistance, x + 15, scoreY - mSettings.mScoreLineDistance );
    if( yOffset <= -3 ) {
      //Second appended line above must be drawn
      mPainter->drawLine( x - 5, scoreY - 2 * mSettings.mScoreLineDistance, x + 15, scoreY - 2 * mSettings.mScoreLineDistance );
      if( yOffset <= -5 )
        //Third appended line above must be drawn
        mPainter->drawLine( x - 5, scoreY - 3 * mSettings.mScoreLineDistance, x + 15, scoreY - 3 * mSettings.mScoreLineDistance );
      }
    }
  else if( yOffset > 10 ) {
    //Appended line below must be drawn
    mPainter->drawLine( x - 5, scoreY + 5 * mSettings.mScoreLineDistance, x + 15, scoreY + 5 * mSettings.mScoreLineDistance );
    if( yOffset > 12 ) {
      //Second appended line below must be drawn
      mPainter->drawLine( x - 5, scoreY + 6 * mSettings.mScoreLineDistance, x + 15, scoreY + 6 * mSettings.mScoreLineDistance );
      if( yOffset > 14 )
        //Third appended line below must be drawn
        mPainter->drawLine( x - 5, scoreY + 7 * mSettings.mScoreLineDistance, x + 15, scoreY + 7 * mSettings.mScoreLineDistance );
      }
    }

  if( opacity )
    mPainter->setOpacity( 0.3 );

  //Draw sharp
  if( noteDies ) {
    //Sharp picture
    static QSvgRenderer sharp( QStringLiteral(":/pic/Music-sharp.svg") );
    static bool first = true;
    if( first ) {
      //At first time we adjust each svg display mode
      first = false;
      sharp.setAspectRatioMode( Qt::KeepAspectRatio );
      }

    int sw = 3 * mSettings.mScoreLineDistance * 25 / 70;
    int sh = sw * 70 / 25;
    sharp.render( mPainter, QRectF( x - sw, yPos - 2 * mSettings.mScoreLineDistance, sw, sh ) );
    }

  //All of svg pictures are same dimensions
  int ew = 5 * mSettings.mScoreLineDistance / 2;
  int eh = ew * 16 / 6;

  //Simple text
  QString fraction;
  QRect over;
  QSvgRenderer *note = noteSvg( yOffset >= 5, noteDuration, fraction );
  if( yOffset < 5 ) {
    over = QRect( QPoint(x,yPos - mSettings.mScoreLineDistance), QSize( ew, eh ) );
    if( note != nullptr )
      note->render( mPainter, over );

    if( !fraction.isEmpty() )
      mPainter->drawText( QPoint( x + ew/2, yPos ), fraction );

//    mPainter->drawRoundedRect( QRectF( QPoint(x,yPos + 1 - mSettings.mScoreLineDistance), QSize( ew, eh ) ), 1, 1 );
    }
  else {
    over = QRect( QPoint(x,yPos-eh), QSize( ew, eh ) );
    if( note != nullptr )
      note->render( mPainter, over );

    if( !fraction.isEmpty() )
      mPainter->drawText( QPoint( x + ew/2, yPos ), fraction );

//    mPainter->drawRoundedRect( QRectF( QPoint(x,yPos-eh), QSize( ew, eh ) ), 1, 1 );
    }

  if( opacity )
    mPainter->setOpacity( 1.0 );

  return over;
  }







void CsPainter::buildDisposition(QVector<CsLyricDisposition> &disposition, const CsLyricLine &lyricLine)
  {
  //Preserve amount of internal array
  if( lyricLine.count() > disposition.count() )
    disposition.resize( lyricLine.count() );


  //At first, we calculate width of each symbol and their positions
  int curX = visualX( mLeftGap, 0 );
  int pos = 0;
  for( int i = 0; i < lyricLine.count(); i++ ) {
    if( lyricLine.at(i).isAlign() ) {
      if( pos >= mLineStartOffset )
        pos = mLineStartOffset + lyricLine.at(i).align();
//        pos = mLineStartOffset + ((pos - mLineStartOffset) & ~0xff) + lyricLine.at(i).align();
      else
        pos = lyricLine.at(i).align();
      curX = visualX( mLeftGap, pos );
      disposition[i].mWidth = 2;
      //Align all previous non-delimiter symbols to right
      int prevX = curX;
      for( int k = i - 1; k >= 0 && !lyricLine.at(k).isDelimiter(); k-- ) {
        prevX -= disposition[k].mWidth;
        disposition[k].mPosX = prevX;
        //Change highlighting previous symbols to previous quarter
        disposition[k].mHighlight = isPlayerHighlight( pos - duraQuarter, duraQuarter );
        }
      }
    else {
      QRect r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, lyricLine.at(i).string() );
      disposition[i].mWidth = r.width();
      }
    disposition[i].mPosX = curX;
    curX += disposition[i].mWidth;
    disposition[i].mHighlight = isPlayerHighlight( pos, mTickPerTakt - (pos % mTickPerTakt) );
    }

  //Remove symbols overlapping
  //int appendOffset = 0;
  for( int i = 0; i < lyricLine.count() - 1; i++ ) {
    if( disposition[i].after() > disposition[i+1].mPosX ) {
      disposition[i+1].mPosX = disposition[i].after();
      }
    }
  }






int CsPainter::visualX(int x, int pos)
  {
  return x + (pos * mSettings.mPixelPerTakt >> 8);
  }






void CsPainter::drawRemarkLine(const CsLine &line)
  {
  if( mVisibleRemark.count() > 0 ) {
    mPainter->setFont( QFont(mSettings.mFontName, mSettings.mRemarkFontSize) );
    mPainter->setPen( mSettings.mColorRemark );
    }

  //For each remark translations which visible we perform drawing
  for( const auto &lang : qAsConst(mVisibleRemark) ) {
    if( mCellCursor != nullptr ) {
      int width = drawCellText( mLeftGap, mCurY + mRemarkTextHeight.mOffset, line.remarkGet(lang), mRemarkTextHeight.mHeight,
                    mCellCursor->isMatch( cccRemark, mLineIndex, lang ) );
      mReferenceList.append( CsReference( mLeftGap, mCurY + mRemarkTextHeight.mOffset, width, mRemarkTextHeight.mHeight,
                                          cccRemark, mLineIndex, lang, 0 ) );
      }

    mCurY += mRemarkTextHeight.mHeight;
    if( isNotEditRemark( lang, mLeftGap, mCurY ) )
      drawRemarkImpl( mLeftGap, mCurY, line.remarkGet(lang) );
    mCurY += mSettings.mTextGap;
    }
  }




void CsPainter::drawChord(int taktCount, const CsLine &line)
  {
  if( mVisibleChord.count() > 0 )
    mPainter->setFont( QFont(mSettings.mFontName, mSettings.mChordFontSize) );

  //For each chord line which visible we perform drawing
  for( const auto &chordKey : qAsConst(mVisibleChord) ) {
    drawTaktLines( taktCount, mCurY - mChordTextHeight.mOffset, mCurY + mChordTextHeight.mHeight );
    drawCellChord( mCurY, taktCount * mTickPerTakt, chordKey );
    mCurY += mChordTextHeight.mHeight;
    drawChordImpl( chordKey, line.chordListGet(chordKey) );
    mCurY += mSettings.mTextGap;
    }
  }




void CsPainter::drawNote(int taktCount, const CsLine &line)
  {
  //For each note line which visible we perform drawing
  for( const auto &noteKey : qAsConst(mVisibleNote) ) {
    drawCellNote( mCurY, taktCount * mTickPerTakt, noteKey );
    drawNoteImpl( mClefMap.value(noteKey), taktCount, noteKey, line.noteListGet(noteKey) );
    mCurY += 9 * mSettings.mScoreLineDistance;
    }
  }









void CsPainter::drawLyric(const CsLyricLine &lyricLine)
  {
  static QVector<CsLyricDisposition> lyricDisposition(1024);

  mCurY += mLyricTextHeight.mHeight;
  mPainter->setFont( QFont( mSettings.mFontName, mSettings.mLyricFontSize ) );

  if( isNotEditLyric( lyricDisposition ) ) {
    buildDisposition( lyricDisposition, lyricLine );

    //Append lyric symbol reference
    for( int i = 0; i < lyricLine.count(); i++ ) {
      //Position x of symbol
      int x = lyricDisposition[i].mPosX;
      //Width of area ocupied by symbol
      int w = (i + 1) < lyricLine.count() ? lyricDisposition[i + 1].mPosX - x : lyricDisposition[i].mWidth;
      //Append reference
      mReferenceList.append( CsReference( QRect( x, mCurY - mLyricTextHeight.mHeight + mLyricTextHeight.mOffset, w, mLyricTextHeight.mHeight ), cccLyricSymbol, mLineIndex, QString{}, i ) );
      }

    //Paint each lyric symbol
    for( int i = 0; i < lyricLine.count(); i++ ) {
      mPainter->setPen( lyricDisposition[i].mHighlight ? mSettings.mColorLyricHighlight : mSettings.mColorLyric );
      int x = lyricDisposition[i].mPosX;
      if( lyricLine.at(i).isAlign() ) {
        //Draw align line
        mPainter->drawLine( x, mCurY, x, mCurY - mLyricTextHeight.mHeight );
        }
      else
        mPainter->drawText( x, mCurY, lyricLine.at(i).string() );
      }

    }

  mCurY += mSettings.mTextGap;
  }




void CsPainter::drawTranslation(const CsLine &line)
  {
  if( mVisibleTranslate.count() > 0 ) {
    mPainter->setFont( QFont(mSettings.mFontName, mSettings.mTranslationFontSize) );
    mPainter->setPen( mSettings.mColorTranslation );
    }

  //For each translations which visible we perform drawing
  for( const auto &lang : qAsConst(mVisibleTranslate) ) {
    if( mCellCursor != nullptr ) {
      int width = drawCellText( mLeftGap, mCurY + mTranslationTextHeight.mOffset, line.translationGet(lang), mTranslationTextHeight.mHeight,
                    mCellCursor->isMatch( cccTranslation, mLineIndex, lang ) );
      mReferenceList.append( CsReference( mLeftGap, mCurY + mTranslationTextHeight.mOffset, width, mTranslationTextHeight.mHeight,
                                          cccTranslation, mLineIndex, lang, 0 ) );
      }

    mCurY += mTranslationTextHeight.mHeight;
    drawTranslationImpl( mLeftGap, mCurY, line.translationGet(lang) );
    mCurY += mSettings.mTextGap;
    }
  }



void CsPainter::drawPlayPosition( int markHeight )
  {
  if( !isPlayerOnCurrentLine() || markHeight <= 0 )
    return;

  int posx = visualX( mLeftGap, mPlayer->linePosition() );

  //Set current play position
  mPlayerLine = QLine( posx, mCurY, posx, mCurY + markHeight );

  mPainter->setPen( QPen( QBrush(mSettings.mPlayMarkColor), mSettings.mPlayMarkWidth ) );
  mPainter->drawLine( mPlayerLine );
  }








void CsPainter::drawRemarkImpl(int x, int y, const QString &rem)
  {
  //For empty string nothing done
  if( rem.isEmpty() )
    return;

  //Draw simple text
  mPainter->drawText( x, y, rem );
  }






void CsPainter::drawChordImpl(const QString &part, const QList<CsChord> &chordList )
  {
  //Paint each chord
  for( auto const &chord : chordList ) {
    int visx = visualX( mLeftGap, chord.position() );
    mPainter->setPen( isPlayerHighlight( chord ) ? mSettings.mColorChordHighlight : mSettings.mColorChord );
    if( isNotEditChord( part, chord.position(), visx, mCurY ) )
      mPainter->drawText( visx, mCurY, chord.chordText() );
    }
  }








void CsPainter::drawNoteImpl(int clef, int taktCount, const QString &part, const QList<CsNote> &noteList)
  {
  int scoreY = mCurY + mSettings.mScoreLineDistance * 2;

  //Draw takt lines
  drawTaktLines( taktCount, scoreY, mCurY + 6 * mSettings.mScoreLineDistance );

  mPainter->setPen( mSettings.mColorNote );
  //Draw five lines of score
  for( int i = 0; i < 5; i++ )
    mPainter->drawLine( mClefPos - 5, scoreY + i * mSettings.mScoreLineDistance, visualX( mLeftGap, taktCount * mTickPerTakt) + 5, scoreY + i * mSettings.mScoreLineDistance );

  //Numerator and denominator
  mPainter->setFont( QFont( mSettings.mFontName, 2 * mSettings.mScoreLineDistance ) );
  mPainter->drawText( mDenominatorPos, scoreY + 2 * mSettings.mScoreLineDistance, mNumerator );
  mReferenceList.append( CsReference( mDenominatorPos, scoreY, 2 * mSettings.mScoreLineDistance, 2 * mSettings.mScoreLineDistance, cccDenomenator, mLineIndex, part, 0 ) );
  mPainter->drawText( mDenominatorPos, scoreY + 4 * mSettings.mScoreLineDistance, mDenominator );
  mReferenceList.append( CsReference( mDenominatorPos, scoreY + 2 * mSettings.mScoreLineDistance, 2 * mSettings.mScoreLineDistance, 2 * mSettings.mScoreLineDistance, cccNumerator, mLineIndex, part, 0 ) );

  mPainter->setFont( QFont( mSettings.mFontName, 4 * mSettings.mScoreLineDistance ) );

  //Note abowe line 0
  int noteStart = whiteOctaveFirst + whiteC;

  //Clef
  //There are three variants of clef
  //We use svg pictures for its
  static QSvgRenderer clefG( QStringLiteral(":/pic/Music-GClef.svg") );
  static QSvgRenderer clefC( QStringLiteral(":/pic/Music-Cclef.svg") );
  static QSvgRenderer clefF( QStringLiteral(":/pic/Music-Fclef.svg") );
  static bool first = true;
  if( first ) {
    //For first time we tune picture display
    first = false;
    clefG.setAspectRatioMode( Qt::KeepAspectRatio );
    clefC.setAspectRatioMode( Qt::KeepAspectRatio );
    clefF.setAspectRatioMode( Qt::KeepAspectRatio );
    }

  //Clef position. Its different for different clefs
  int ex = mClefPos;
  int ey = scoreY;
  int ew = 10, eh = 10;
  if( clef == noteG ) {
    ey = scoreY - 1 * mSettings.mScoreLineDistance;
    ew = 24 * mSettings.mScoreLineDistance / 10;
    eh = ew * 17 / 6;
    clefG.render( mPainter, QRectF( QPoint( ex, ey ), QSize( ew, eh ) ) );
    noteStart = whiteOctaveSecond + whiteG;
    }
  else if( clef == noteF ) {
    ey = scoreY;
    ew = 3 * mSettings.mScoreLineDistance;
    eh = ew * 25 / 23;
    clefF.render( mPainter, QRectF( QPoint( ex, ey ), QSize( ew, eh ) ) );
    noteStart = whiteOctaveSmall + whiteB;
    }
  else if( clef == noteC ) {
    ey = scoreY;
    ew = 42 * mSettings.mScoreLineDistance * 2 / 30;
    eh = ew * 3 / 2;
    clefC.render( mPainter, QRectF( QPoint( ex, ey ), QSize( ew, eh ) ) );
    noteStart = whiteOctaveFirst + whiteB;
    }
  mReferenceList.append( CsReference( ex, ey, ew, eh, cccClef, mLineIndex, part, 0 ) );

  //Not match to edit cursor, so we draw current cell note if match
  if( !disableDrawActiveNote() && mCellCursor != nullptr && mCellCursor->isMatch( cccNote, mLineIndex, part ) ) {
    //Draw active note in current cell
    int visX = visualX( mLeftGap, mCellCursor->linePosition() );
    drawNoteSingle( visX, scoreY, noteStart, mCellCursor->note().white(), mCellCursor->note().duration(), mCellCursor->note().isDies(), true );
    }


  //Draw notes
  for( auto const &note : qAsConst(noteList) ) {
    mPainter->setPen( isPlayerHighlight( note ) ? mSettings.mColorNoteHighlight : mSettings.mColorNote );
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




void CsPainter::drawPropertyImpl(int xorigin, int xtab, const QString &title, const QString &value, const QString &propertyId )
  {
  mCurY += mPropertiesHeight.mHeight;
  mPainter->drawText( xorigin, mCurY, title );
  drawCellProperty( xorigin + xtab, mCurY - mPropertiesHeight.mHeight + mPropertiesHeight.mOffset, value, mPropertiesHeight.mHeight, propertyId );
  if( isNotEditProperty( propertyId, xorigin + xtab, mCurY )  )
    mPainter->drawText( xorigin + xtab, mCurY, value );
  mCurY += mSettings.mTextGap;
  }



void CsPainter::drawTaktLines(int taktCount, int y0, int y1)
  {
  if( taktCount ) {
    mPainter->setPen( mSettings.mColorTakt );
    //If no offset of line start then takt line on one more
    int tc = taktCount + (mLineStartOffset == 0 ? 1 : 0);
    for( int i = 0; i < tc; i++ ) {
      int x = visualX( mLeftGap, mLineStartOffset + i * mTickPerTakt );
      mPainter->drawLine( x, y0, x, y1 );
      }
    }
  }





CsTextHeight CsPainter::fontHeight(int fontSize) const
  {
  QFontMetrics met( QFont( mSettings.mFontName, fontSize ) );
  QRect r = met.boundingRect( QChar(u'Д') );
  //qDebug() << r;
  CsTextHeight hs;
  hs.mHeight = r.height() + 1;
  hs.mOffset = r.height() + r.y();
  //qDebug() << hs.mHeight << hs.mOffset;
  return hs;
  }



bool CsPainter::isPlayerHighlight(int position, int duration) const
  {
  return isPlayerOnCurrentLine() && mPlayer->isHit( position, duration );
  }










void CsPainter::drawCellProperty(int x, int y, const QString &value, int height, const QString &propertyId )
  {
  if( mCellCursor == nullptr )
    return;

  int width = drawCellText( x, y, value, height, mCellCursor->cellClass() == cccAttribute && mCellCursor->partName() == propertyId );

  mReferenceList.append( CsReference( x, y, width, height, cccAttribute, 0, propertyId, 0 ) );
  }




int CsPainter::drawCellText(int x, int y, const QString &value, int height, bool isCurrent)
  {
  //Calculate width of cell. Width is max of width of value string and 200 pixels
  int width = 200;
  if( !value.isEmpty() ) {
    QRect r = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, value );
    width = qMax( 50, r.width() );
    }

  drawCell( x, y, width, height, isCurrent );

  return width;
  }



void CsPainter::drawCellChord(int y, int tickCount, const QString &part )
  {
  for( int tick = 0; tick < tickCount; tick += mStepChord ) {
    int x = visualX( mLeftGap, tick );
    if( mCellCursor != nullptr )
      drawCell( x, y + mChordTextHeight.mOffset, mStepPixChord, mChordTextHeight.mHeight,
              mCellCursor->isMatch( cccChord, tick, mLineIndex, part ) );
    mReferenceList.append( CsReference( x, y + mChordTextHeight.mOffset, mStepPixChord, mChordTextHeight.mHeight, cccChord, mLineIndex, part, tick ) );
    }
  }




void CsPainter::drawCellNote(int y, int tickCount, const QString &part)
  {
  for( int tick = 0; tick < tickCount; tick += mStepNote ) {
    int x = visualX( mLeftGap, tick );
    if( mCellCursor != nullptr ) {
      bool match = mCellCursor->isMatch( cccNote, tick, mLineIndex, part );
      drawCell( x, y, mStepPixNote, 9 * mSettings.mScoreLineDistance, match );
      }
    mReferenceList.append( CsReference( x, y, mStepPixNote, 9 * mSettings.mScoreLineDistance, cccNote, mLineIndex, part, tick ) );
    }
  }





void CsPainter::drawCellLyric(int y, int tickCount)
  {
  int x = visualX( mLeftGap, 0 );
  int w = visualX( mLeftGap, tickCount ) - x;
  if( mCellCursor != nullptr )
    drawCell( x, y + mLyricTextHeight.mOffset, w, mLyricTextHeight.mHeight, mCellCursor->isMatch( cccLyric ) && mCellCursor->lineIndex() == mLineIndex );
  mReferenceList.append( CsReference( x, y + mLyricTextHeight.mOffset, w, mLyricTextHeight.mHeight, cccLyric, mLineIndex, QString{}, 0 ) );
  }




void CsPainter::drawCell(int x, int y, int width, int height, bool isCurrent)
  {
  if( isCurrent ) {
    //Draw current cell background
    mPainter->setPen( mSettings.mColorCellCurrent );
    mPainter->setBrush( mSettings.mColorCellCurrent.lighter(140) );
    mPainter->drawRoundedRect( x, y, width, height, 3, 3 );
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
