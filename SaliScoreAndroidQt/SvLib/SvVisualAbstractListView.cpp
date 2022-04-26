#include "SvVisualAbstractListView.h"
#include "SvVisualAbstractList.h"
#include <QPainter>

SvVisualAbstractListView::SvVisualAbstractListView(SvVisualAbstractList *visualList, QWidget *parent) :
  QWidget{parent},
  mVisualList(visualList)
  {

  }


void SvVisualAbstractListView::paintEvent(QPaintEvent *event)
  {
  Q_UNUSED(event)
  QPainter painter(this);
  mVisualList->contentPaint(painter);
  }
