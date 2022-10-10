#ifndef CSSVG_H
#define CSSVG_H

#include <QPainter>
#include <QPainterPath>
#include <QPolygonF>
#include <QList>

class CsSvg
  {
    QPainterPath mPath;
    QPolygonF    mPolygon;
    bool         mPolyStroke;
  public:
    CsSvg( QPainterPath path, QPolygonF poly, bool polyStroke );

    void render( QPainter *painter, QColor color, int x, int y );
  };

using CsSvgPtr = CsSvg*;
using CsSvgPtrList = QList<CsSvgPtr>;

#endif // CSSVG_H
