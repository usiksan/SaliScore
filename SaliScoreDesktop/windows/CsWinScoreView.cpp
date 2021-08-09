#include "CsWinScoreView.h"
#include "CsWinScoreMode.h"

#include <QDebug>

CsWinScoreView::CsWinScoreView(CsComposition &comp, CsPlay &play, QWidget *parent) :
  QWidget(parent),
  mComposition(comp),
  mPlayer(play),
  mWinScroll(nullptr)
  {

  }




void CsWinScoreView::activate()
  {
  setFocus();
  update();
  }




void CsWinScoreView::setupWinScroll(CsWinScoreMode *winScroll)
  {
  mWinScroll = winScroll;
  }

void CsWinScoreView::viewUpdate()
  {
  update();
  }
