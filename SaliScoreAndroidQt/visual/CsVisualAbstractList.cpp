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

  mFooterBoundY = viewport()->size().height() - footerHeight();

  //Skip top not displayed items
  int index = 0;
  int posY = -mStartY + headerHeight();
  while( index < itemCount() ) {
    int height = itemHeight(index);
    if( posY + height > headerHeight() ) break;
    index++;
    posY += height;
    }

  //Draw visible list content
  mStartIndex = index;
  mItemBounds.clear();
  if( index < itemCount() ) {
    //Max Y to which need draw
    int lastY = viewport()->size().height() - footerHeight();
    while( index < itemCount() && posY < lastY ) {
      posY += itemPaint( index, posY, painter );
      mItemBounds.append( posY );
      index++;
      }

    //Calculate full content lenght
    while( index < itemCount() ) {
      posY += itemHeight(index++);
      }
    }

  //Draw header. It must overdraw top part of view
  headerPaint( painter );

  //Draw footer. It must overdraw bottom part of view
  footerPaint( painter );

  //Correct full list height when needed
  posY += mStartY - headerHeight();
  updateVerticalScrollBar( posY );
  }




int CsVisualAbstractList::itemPaint(int index, int y, QPainter &painter)
  {
  painter.setPen( Qt::black );
  painter.drawText( 10, y, QString("Item %1").arg(index) );
  return itemHeight(index);
  }



void CsVisualAbstractList::updateVerticalScrollBar(int fullHeight)
  {
  if( mFullHeight != fullHeight ) {
    mFullHeight = fullHeight;

    verticalScrollBar()->setRange( 0, mFullHeight );
    verticalScrollBar()->setSingleStep( size().height() / 100 );
    verticalScrollBar()->setPageStep( size().height() * 9 / 10 );
    }
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

    if( event->y() < headerHeight() ) {
      //Pressed in header area
      headerClicked( event->x(), event->y() );
      }
    else if( event->y() > mFooterBoundY ) {
      //Pressed in footer area
      footerClicked( event->x(), event->y() - mFooterBoundY );
      }
    else {
      //Pressed if item list area
      //Find item index
      for( int i = 0; i < mItemBounds.count(); i++ )
        if( mItemBounds.at(i) > event->y() ) {
          itemClicked( event->x(), i + mStartIndex );
          break;
          }
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

