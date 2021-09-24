#include "CsPainterEditor.h"

CsPainterEditor::CsPainterEditor(QPainter *painter, const QString &keyViewSettings, const CsComposition &comp, const CsPlay &player, int offsetX, QSize size, CsCellCursor *cellCursor, CsCursorEdit *cursorEdit) :
  CsPainter( painter, keyViewSettings, comp, player, offsetX, size, cellCursor ),
  mCursorEdit(cursorEdit)
  {

  }


bool CsPainterEditor::isNotEditProperty(int propertyId, int x, int y)
  {
  if( mCursorEdit == nullptr || !mCursorEdit->isMatch(propertyId) )
    return true;

  paintEditText( x, y );
  return false;
  }




bool CsPainterEditor::isNotEditRemark(const QString &part, int x, int y)
  {
  if( mCursorEdit == nullptr || !mCursorEdit->isMatch( cccRemark, mLineIndex, part ) )
    return true;

  paintEditText( x, y );
  return false;
  }




bool CsPainterEditor::isNotEditChord(const QString &part, int position, int x, int y)
  {
  if( mCursorEdit == nullptr || !mCursorEdit->isMatch( cccChord, position, mLineIndex, part ) )
    return true;

  mPainter->drawText( x, y, mCursorEdit->getStr() );

  QRect rpos = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, mCursorEdit->getStr() );

  //Cursor position
  mPainter->drawLine( x + rpos.width(), y, x + rpos.width(), y - rpos.height() );

  return false;
  }




bool CsPainterEditor::isNotEditNote(const QString &part, int position, int x, int scoreY, int noteStart)
  {
  if( mCursorEdit == nullptr || !mCursorEdit->isMatch( cccNote, position, mLineIndex, part ) )
    return true;

  QRect over = drawNoteSingle( x, scoreY, noteStart, mCursorEdit->noteWhite(), mCursorEdit->duration(), mCursorEdit->noteDies() );

  //Cursor position
  //mPainter->setBrush( Qt::transparent );
  int cx = qMax( over.left(), over.right() );
  int cy = over.top();
  mPainter->drawLine( cx, cy, cx, cy - over.height() );

  return false;
  }



bool CsPainterEditor::isNotEditLyric( QVector<CsLyricDisposition> &disposition )
  {
  if( mCursorEdit == nullptr || !(mCursorEdit->isMatch( cccLyric ) && mCursorEdit->lineIndex() == mLineIndex) )
    return true;

  const CsLyricLine &line = mCursorEdit->getLyric();
  buildDisposition( disposition, line );

  //Get over rect of text
  //QRect over = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, QString("H") );

  //Paint each lyric symbol
  mPainter->setPen( mSettings.mColorLyric );
  for( int i = 0; i < line.count(); i++ ) {
    int x = disposition[i].mPosX;
    if( line.at(i).isAlign() ) {
      //Draw align line
      mPainter->drawLine( x, mCurY, x, mCurY - mLyricTextHeight.mHeight );
      }
    else
      mPainter->drawText( x, mCurY, line.at(i).string() );
    }

  //Cursor position
  int x = visualX( mLeftGap, 0 );
  if( line.count() ) {
    int cursorIndex = mCursorEdit->charPosition();
    if( cursorIndex < line.count() )
      x = disposition[cursorIndex].mPosX;
    else
      x = disposition[line.count() - 1].after();
    }
  mPainter->drawLine( x, mCurY + 2, x, mCurY - mLyricTextHeight.mHeight - 2 );

  if( mCursorEdit->isSelectionPresent() ) {
    //Fill selection rect
    int sStart = mCursorEdit->selStart();
    sStart = sStart < line.count() ? disposition[sStart].mPosX : disposition[line.count() - 1].after();

    int sStop = mCursorEdit->selStop();
    sStop = sStop < line.count() ? disposition[sStop].mPosX : disposition[line.count() - 1].after();

    QRect res( QPoint( qMin(sStart,sStop), mCurY - mLyricTextHeight.mHeight ), QPoint( qMax(sStart,sStop), mCurY )  );

    mPainter->setPen( QPen( Qt::DotLine ) );
    mPainter->setBrush( Qt::transparent );
    mPainter->drawRect( res );
    }

  return false;
  }


bool CsPainterEditor::isNotEditTranslation(const QString &part, int x, int y)
  {
  if( mCursorEdit == nullptr || !mCursorEdit->isMatch( cccRemark, mLineIndex, part ) )
    return true;

  paintEditText( x, y );
  return false;
  }



void CsPainterEditor::paintEditText(int x, int y)
  {
  QString str( mCursorEdit->getStr() );
  //Get over rect of text
  QRect over, rpos;
  if( str.isEmpty() )
    over = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, QString("H") );
  else
    over = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, str );

  rpos = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, str.left( mCursorEdit->charPosition() ) );

  if( !str.isEmpty() )
    mPainter->drawText( x, y, str );

  //Cursor position
  mPainter->drawLine( x + rpos.width(), y, x + rpos.width(), y - over.height() );

  if( mCursorEdit->isSelectionPresent() ) {
    //Fill selection rect
    QRect rStart = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, str.left(mCursorEdit->selStart()) );
    QRect rStop  = mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, str.left(mCursorEdit->selStop()) );
    QRect res;
    QPoint topLeft( rStart.topRight() );
    topLeft.rx() += x;
    topLeft.ry() += y;
    res.setTopLeft( topLeft );
    QPoint bottomRight( rStop.bottomRight() );
    bottomRight.rx() += x;
    bottomRight.ry() += y;
    res.setBottomRight( bottomRight );

    mPainter->setPen( QPen( Qt::DotLine ) );
    mPainter->setBrush( Qt::transparent );
    mPainter->drawRect( res );
    }

  }



