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
  mPainter->drawLine( x + rpos.width(), y, x + rpos.width(), y + over.height() );

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
