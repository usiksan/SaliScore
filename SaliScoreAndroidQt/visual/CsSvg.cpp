#include "CsSvg.h"

CsSvg::CsSvg(QPainterPath path, QPolygonF poly, bool polyStroke) :
  mPath(path),
  mPolygon(poly),
  mPolyStroke(polyStroke)
  {

  }




void CsSvg::render(QPainter *painter, QColor color, int x, int y)
  {
  QTransform t;
  t.translate(x,y);
  if( !mPath.isEmpty() )
    painter->drawPath( t.map(mPath) );

  painter->setPen( Qt::transparent );
  painter->setBrush( color );
  painter->drawPolygon( t.map(mPolygon) );
  painter->setPen( color );
  }
