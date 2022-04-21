#include "CsVisualAbstractListView.h"
#include "CsVisualAbstractList.h"
#include <QPainter>

CsVisualAbstractListView::CsVisualAbstractListView(CsVisualAbstractList *visualList, QWidget *parent) :
  QWidget{parent},
  mVisualList(visualList)
  {

  }


void CsVisualAbstractListView::paintEvent(QPaintEvent *event)
  {
  Q_UNUSED(event)
  QPainter painter(this);
  mVisualList->contentPaint(painter);
  }
