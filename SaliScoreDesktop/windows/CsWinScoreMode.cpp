#include "CsWinScoreMode.h"

#include <QPaintEvent>

CsWinScoreMode::CsWinScoreMode(CsWinScoreView *view, QWidget *parent) :
  QAbstractScrollArea(parent),
  mView(view)
  {
  setViewport( view );
  }



void CsWinScoreMode::paintEvent(QPaintEvent *event)
  {
  event->accept();
  mView->paint();
  }
