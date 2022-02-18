#include "CsWinScoreView.h"
#include "CsWinScoreMode.h"
#include "CsPainter.h"

#include <QPainter>
#include <QSettings>
#include <QScrollBar>
#include <QDebug>

CsWinScoreView::CsWinScoreView(CsComposition &comp, CsPlay &play, QWidget *parent) :
  QWidget(parent),
  mComposition(comp),
  mPlayer(play),
  mWinScroll(nullptr),
  mAutoScroll(false),
  mOffsetX(0),
  mOffsetY(0),
  mSizeY(0)
  {

  }

void CsWinScoreView::compositionChanged()
  {
  mPlayer.reset();
  mPlayer.show(false);
  }




void CsWinScoreView::activate()
  {
  mPlayer.reset();
  setFocus();
  update();
  }




void CsWinScoreView::setupWinScroll(CsWinScoreMode *winScroll)
  {
  mWinScroll = winScroll;
  }

void CsWinScoreView::viewUpdate()
  {
  update();
  }




void CsWinScoreView::paintScore(CsPainter &cp)
  {

  //Fill background
  cp.fillBackground();

  //Draw properties
  int posy = cp.drawTitleAndProperties( -mOffsetY, mComposition );

  //Draw lines of score
  for( int i = 0; i < mComposition.lineCount(); i++ )
    posy = cp.drawLine( posy, i, mComposition.line(i), mAutoScroll );

  //Norm posy to vertical size
  posy += mOffsetY;
  if( posy != mSizeY ) {
    //Height changed - update scrollbars
    mSizeY = posy;
    mWinScroll->verticalScrollBar()->setRange( 0, posy );
    mWinScroll->verticalScrollBar()->setSingleStep( size().height() / 100 );
    mWinScroll->verticalScrollBar()->setPageStep( size().height() * 9 / 10 );
    //verticalScrollBar()->setValue( -mOrigin.y() );
    }

  //Roll by player
  if( mPlayer.isRun() ) {
    //Vertical autoscroll
    QLine ln( cp.playerLine() );
    int top = qMin(ln.y1(),ln.y2());
    int bot = qMax(ln.y1(),ln.y2());
    if( top < 0 ) {
      mOffsetY = qBound( 0, mOffsetY + top, mSizeY );
      update();
      }
    else if( bot > size().height() * 2 / 3 ) {
      mOffsetY = qBound( 0, mOffsetY + bot - size().height() / 3, mSizeY );
      update();
      }
    }

  else if( mAutoScroll ) {
    QRect r( cp.cellCursorRect() );
    //Vertical autoscroll
    //qDebug() << "offy" << mOffsetY << r.top() << r.bottom() << mSizeY;
    if( r.top() < 0 )
      mOffsetY = qBound( 0, mOffsetY + r.top(), mSizeY );
    else if( r.bottom() > size().height() )
      mOffsetY = qBound( 0, mOffsetY + r.bottom() - size().height() + 10, mSizeY );

    //Horizontal autoscroll
    if( r.left() < 0 )
      mOffsetX = qBound( 0, mOffsetX + r.left(), 1024 );
    else if( r.right() > size().width() )
      mOffsetX = qBound( 0, mOffsetX + r.right() - size().width(), 1024 );

    mAutoScroll = false;
    update();
    }
  }
