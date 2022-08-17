#include "SvVisualAbstractList.h"
#include "SvVisualAbstractListView.h"

#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QScrollBar>

SvVisualAbstractList::SvVisualAbstractList(QWidget *parent) :
  QAbstractScrollArea(parent),
  mStartY(0),
  mStartIndex(0),
  mFullHeight(0),
  mMousePress(false)
  {
  setViewport( new SvVisualAbstractListView( this, this ) );
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




void SvVisualAbstractList::contentPaint(QPainter &painter)
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




void SvVisualAbstractList::contentClicked(int x, int y)
  {
  if( y < headerHeight() ) {
    //Pressed in header area
    headerClicked( x, y );
    }
  else if( y > mFooterBoundY ) {
    //Pressed in footer area
    footerClicked( x, y - mFooterBoundY );
    }
  else {
    //Pressed if item list area
    //Find item index
    for( int i = 0; i < mItemBounds.count(); i++ )
      if( mItemBounds.at(i) > y ) {
        itemClicked( x, i + mStartIndex );
        break;
        }
    }
  }




int SvVisualAbstractList::itemPaint(int index, int y, QPainter &painter)
  {
  painter.setPen( Qt::black );
  painter.drawText( 10, y, QString("Item %1").arg(index) );
  return itemHeight(index);
  }



void SvVisualAbstractList::updateVerticalScrollBar(int fullHeight)
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
void SvVisualAbstractList::updateContent()
  {
  viewport()->update();
  }


void SvVisualAbstractList::paintEvent(QPaintEvent *event)
  {
  event->accept();
  QPainter painter(viewport());
  contentPaint( painter );
  }


void SvVisualAbstractList::mousePressEvent(QMouseEvent *event)
  {
  mMousePress = true;
  mMousePressY = event->y();
  mMouseStartY = mStartY;
  }



void SvVisualAbstractList::mouseReleaseEvent(QMouseEvent *event)
  {
  if( mMousePress ) {
    mMousePress = false;
    if( qAbs( event->y() - mMousePressY ) < 10 ) {
      //This is click
      mStartY = mMouseStartY;
      contentClicked( event->x(), event->y() );
      }
    }
  }


void SvVisualAbstractList::mouseMoveEvent(QMouseEvent *event)
  {
  if( mMousePress ) {
    mStartY = qBound( 0, mMouseStartY - event->y() + mMousePressY, mFullHeight );
    verticalScrollBar()->setValue( mStartY );
    updateContent();
    }
  }



void SvVisualAbstractList::wheelEvent(QWheelEvent *event)
  {
  int delta = event->angleDelta().y() / 12;
  mStartY = qBound( 0, mStartY - delta, mFullHeight );
  verticalScrollBar()->setValue( mStartY );
  updateContent();
  }



void SvVisualAbstractList::keyPressEvent(QKeyEvent *event)
  {
  Q_UNUSED(event)
  }

