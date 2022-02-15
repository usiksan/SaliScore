#include "CsVisualAbstractList.h"
#include "CsVisualAbstractListView.h"

#include <QPainter>
#include <QPaintEvent>

CsVisualAbstractList::CsVisualAbstractList(QWidget *parent) :
  QAbstractScrollArea(parent),
  mStartY(0),
  mStartIndex(0)
  {
  setViewport( new CsVisualAbstractListView( this, this ) );
  setMouseTracking(true);
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

  mStartIndex = index;
  mItemBounds.clear();
  if( index < count() ) {
    int lastY = viewport()->size().height();
    while( index < count() && posY < lastY ) {
      posY += paintItem( index, posY, painter );
      mItemBounds.append( posY );
      index++;
      }
    }
  }




int CsVisualAbstractList::paintItem(int index, int y, QPainter &painter)
  {
  painter.setPen( Qt::black );
  painter.drawText( 10, y, QString("Item %1").arg(index) );
  return itemHeight(index);
  }


void CsVisualAbstractList::paintEvent(QPaintEvent *event)
  {
  event->accept();
  QPainter painter(viewport());
  paintContent( painter );
  }
