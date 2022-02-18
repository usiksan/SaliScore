#include "CsVisualScore.h"

CsVisualScore::CsVisualScore(CsComposition &comp, QWidget *parent) :
  CsVisualAbstractList(parent),
  mStartX(0),
  mAutoScroll(true),
  mComposition(comp)
  {

  }



void CsVisualScore::paintScore(CsPainter &cp)
  {

  //Fill background
  cp.fillBackground();

  //Draw properties
  int posy = cp.drawTitleAndProperties( -mStartY, mComposition );

  //Draw lines of score
  for( int i = 0; i < mComposition.lineCount(); i++ )
    posy = cp.drawLine( posy, i, mComposition.line(i), mAutoScroll );

  //Norm posy to vertical size
  posy += mStartY;
  updateVerticalScrollBar( posy );

  //Roll by player
//  if( mPlayer.isRun() ) {
//    //Vertical autoscroll
//    QLine ln( cp.playerLine() );
//    int top = qMin(ln.y1(),ln.y2());
//    int bot = qMax(ln.y1(),ln.y2());
//    if( top < 0 ) {
//      mStartY = qBound( 0, mStartY + top, mFullHeight );
//      update();
//      }
//    else if( bot > size().height() * 2 / 3 ) {
//      mStartY = qBound( 0, mStartY + bot - size().height() / 3, mFullHeight );
//      update();
//      }
//    }

//  else if( mAutoScroll ) {
//    QRect r( cp.cellCursorRect() );
//    //Vertical autoscroll
//    //qDebug() << "offy" << mStartY << r.top() << r.bottom() << mFullHeight;
//    if( r.top() < 0 )
//      mStartY = qBound( 0, mStartY + r.top(), mFullHeight );
//    else if( r.bottom() > size().height() )
//      mStartY = qBound( 0, mStartY + r.bottom() - size().height() + 10, mFullHeight );

//    //Horizontal autoscroll
//    if( r.left() < 0 )
//      mStartX = qBound( 0, mStartX + r.left(), 1024 );
//    else if( r.right() > size().width() )
//      mStartX = qBound( 0, mStartX + r.right() - size().width(), 1024 );

//    mAutoScroll = false;
//    update();
//    }
  }


void CsVisualScore::compositionChanged()
  {
  mStartY = 0;       //!< Coordinate Y of list viewport begin
  mStartIndex = 0;   //!< Index of first visible item
  }


