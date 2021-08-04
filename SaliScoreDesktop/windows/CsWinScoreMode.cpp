#include "CsWinScoreMode.h"

#include <QPaintEvent>
#include <QWheelEvent>

CsWinScoreMode::CsWinScoreMode(CsWinScoreView *view, QWidget *parent) :
  QAbstractScrollArea(parent),
  mView(view)
  {
  setViewport( view );
  setMouseTracking(true);
  view->setupWinScroll( this );
  }



void CsWinScoreMode::paintEvent(QPaintEvent *event)
  {
  event->accept();
  mView->paint();
  }



