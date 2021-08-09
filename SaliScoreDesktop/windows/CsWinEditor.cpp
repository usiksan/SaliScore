#include "CsWinEditor.h"
#include "CsWinScoreMode.h"
#include "CsPainter.h"

#include <QPaintEvent>
#include <QPainter>
#include <QSettings>
#include <QScrollBar>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>


CsWinEditor::CsWinEditor(CsComposition &comp, CsPlay &play, QWidget *parent) :
  CsWinScoreView( comp, play, parent ),
  mOffsetX(0),
  mOffsetY(0),
  mSizeY(0),
  mCellCursor( comp )
  {

  }



void CsWinEditor::paint()
  {
  QPainter painter(this);
  CsPainter cp( &painter, QStringLiteral(KEY_EDITOR_SETTINGS), mComposition, mPlayer, mOffsetX, size(), &mCellCursor );

  //Закрасить цветом фона
  painter.fillRect( QRect( QPoint(), size() ), cp.backgroundColor() );

  //Draw properties
  int posy = cp.drawTitleAndProperties( -mOffsetY, mComposition );
  for( int i = 0; i < mComposition.lineCount(); i++ )
    posy = cp.drawLine( posy, i, mComposition.line(i) );

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
  }




void CsWinEditor::setupWinScroll(CsWinScoreMode *winScroll)
  {
  CsWinScoreView::setupWinScroll( winScroll );
  winScroll->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
  winScroll->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
  //Tracking scroll bar position
  connect( winScroll->verticalScrollBar(), &QScrollBar::valueChanged, this, [this] (int value) {
    mOffsetY = qBound( 0, value, mSizeY );
    update();
    });
  connect( winScroll->horizontalScrollBar(), &QScrollBar::valueChanged, this, [this] (int value) {
    mOffsetX = qBound( 0, value, 1024 );
    update();
    //originSet( SdPoint(value,originGet().y()) );
    });

  winScroll->horizontalScrollBar()->setRange( 0, 1024 );
  winScroll->horizontalScrollBar()->setSingleStep( 1 );
  winScroll->horizontalScrollBar()->setPageStep( 128 );
  }




void CsWinEditor::upWheelEvent(QWheelEvent *event)
  {
  int delta = event->angleDelta().y() / 12;
  mOffsetY = qBound( 0, mOffsetY - delta, mSizeY );
  mWinScroll->verticalScrollBar()->setValue( mOffsetY );
  update();
  }




void CsWinEditor::upMousePressEvent(QMouseEvent *event)
  {
  }

void CsWinEditor::upMouseReleaseEvent(QMouseEvent *event)
  {
  }

void CsWinEditor::upMouseMoveEvent(QMouseEvent *event)
  {
  }

void CsWinEditor::upKeyPressEvent(QKeyEvent *event)
  {
  switch( event->key() ) {
    case Qt::Key_Up :
      mCellCursor.move( ccoUp );
      break;

    case Qt::Key_Down :
      mCellCursor.move( ccoDown );
      break;

    case Qt::Key_Left :
      mCellCursor.move( ccoLeft );
      break;

    case Qt::Key_Right :
      mCellCursor.move( ccoRight );
      break;
    }
  update();
  }

void CsWinEditor::upKeyReleaseEvent(QKeyEvent *event)
  {
  }
