#include "CsVisualAbstractList.h"
#include "CsVisualAbstractListView.h"

#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QScrollBar>

CsVisualAbstractList::CsVisualAbstractList(QWidget *parent) :
  QAbstractScrollArea(parent),
  mStartY(0),
  mStartIndex(0),
  mFullHeight(0),
  mMousePress(false)
  {
  setViewport( new CsVisualAbstractListView( this, this ) );
  setMouseTracking(true);

  //Setup scrollbars
  setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
  //Tracking scroll bar position
  connect( verticalScrollBar(), &QScrollBar::valueChanged, this, [this] (int value) {
    mStartY = qBound( 0, value, mFullHeight );
    viewport()->update();
    });

  }




void CsVisualAbstractList::paintContent(QPainter &painter)
  {
  painter.fillRect( QRect( QPoint(), viewport()->size() ), QColor(Qt::white) );

  //Skip top not displayed items
  int index = 0;
  int posY = -mStartY;
  while( index < count() ) {
    int height = itemHeight(index);
    if( posY + height > 0 ) break;
    index++;
    posY += height;
    }

  //Draw visible list content
  mStartIndex = index;
  mItemBounds.clear();
  if( index < count() ) {
    //Max Y to which need draw
    int lastY = viewport()->size().height();
    while( index < count() && posY < lastY ) {
      posY += paintItem( index, posY, painter );
      mItemBounds.append( posY );
      index++;
      }

    //Calculate full content lenght
    while( index < count() ) {
      posY += itemHeight(index++);
      }
    }

  //Correct full list height when needed
  posY += mStartY;
  if( mFullHeight != posY ) {
    mFullHeight = posY;

    verticalScrollBar()->setRange( 0, mFullHeight );
    verticalScrollBar()->setSingleStep( size().height() / 100 );
    verticalScrollBar()->setPageStep( size().height() * 9 / 10 );
    }
  }




int CsVisualAbstractList::paintItem(int index, int y, QPainter &painter)
  {
  painter.setPen( Qt::black );
  painter.drawText( 10, y, QString("Item %1").arg(index) );
  return itemHeight(index);
  }



//!
//! \brief updateContent Call update function on viewport
//!
void CsVisualAbstractList::updateContent()
  {
  viewport()->update();
  }


void CsVisualAbstractList::paintEvent(QPaintEvent *event)
  {
  event->accept();
  QPainter painter(viewport());
  paintContent( painter );
  }


void CsVisualAbstractList::mousePressEvent(QMouseEvent *event)
  {
  mMousePress = true;
  mMousePressY = event->y();
  mMouseStartY = mStartY;
  }

void CsVisualAbstractList::mouseReleaseEvent(QMouseEvent *event)
  {
  mMousePress = false;
  if( qAbs( event->y() - mMousePressY ) < 10 ) {
    //This is click
    mStartY = mMouseStartY;

    //Find item index
    for( int i = 0; i < mItemBounds.count(); i++ )
      if( mItemBounds.at(i) > event->y() ) {
        clicked( event->x(), i + mStartIndex );
        break;
        }
    }
  }


void CsVisualAbstractList::mouseMoveEvent(QMouseEvent *event)
  {
  if( mMousePress ) {
    mStartY = qBound( 0, mMouseStartY - event->y() + mMousePressY, mFullHeight );
    verticalScrollBar()->setValue( mStartY );
    updateContent();
    }
  }



void CsVisualAbstractList::wheelEvent(QWheelEvent *event)
  {
  int delta = event->angleDelta().y() / 12;
  mStartY = qBound( 0, mStartY - delta, mFullHeight );
  verticalScrollBar()->setValue( mStartY );
  updateContent();
  }



void CsVisualAbstractList::keyPressEvent(QKeyEvent *event)
  {

  }

