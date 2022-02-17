#include "CsVisualRegularList.h"

#include <QIcon>
#include <QPainter>

CsVisualRegularList::CsVisualRegularList(const QList<int> &cellWidth, QWidget *parent) :
  CsVisualAbstractList(parent),
  mCellWidth(cellWidth)
  {

  }


int CsVisualRegularList::itemHeight(int index) const
  {
  Q_UNUSED(index)
  return 48;
  }




int CsVisualRegularList::itemPaint(int index, int y, QPainter &painter)
  {
  int w = viewport()->size().width();
  int strechIndex = -1;
  //Calculate strech cell position and streched width
  for( int i = 0; i < mCellWidth.count(); i++ )
    if( mCellWidth.at(i) == 0 ) {
      strechIndex = i;
      }
    else w -= mCellWidth.at(i);

  //Paint all cell
  mCellBoundX.clear();
  int x = 0;
  int h = itemHeight( index );
  for( int i = 0; i < mCellWidth.count(); i++ ) {
    if( i == strechIndex ) {
      cellPaint( x, y, w, h, i, index, painter );
      x += w;
      }
    else {
      cellPaint( x, y, mCellWidth.at(i), h, i, index, painter );
      x += mCellWidth.at(i);
      }
    mCellBoundX.append(x);
    }

  return h;
  }


void CsVisualRegularList::itemClicked(int x, int itemIndex)
  {
  for( int i = 0; i < mCellBoundX.count(); i++ )
    if( x < mCellBoundX.at(i) ) {
      cellClicked( i, itemIndex );
      return;
      }
  }



int CsVisualRegularList::headerHeight() const
  {
  return 50;
  }



void CsVisualRegularList::headerPaint(QPainter &painter)
  {
  //Fill header
  int w = viewport()->size().width();
  painter.fillRect( 0, 0, w, 50, QColor(Qt::lightGray) );
  //Draw Back icon
  QIcon(QStringLiteral(":/pic/iconBack.png")).paint( &painter, 1, 1, 48, 48 );

  //Draw title
  w -= 50;
  painter.drawText( 50, 1, w, 48, Qt::AlignLeft | Qt::AlignVCenter, mTitle );
  }



void CsVisualRegularList::headerClicked(int x, int y)
  {
  Q_UNUSED(y)
  if( x < 50 )
    emit clickBack();
  }
