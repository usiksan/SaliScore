#include "CsWinTrain.h"
#include "CsWinScoreMode.h"
#include "CsPainter.h"

#include <QPaintEvent>
#include <QPainter>
#include <QSettings>
#include <QScrollBar>
#include <QWheelEvent>

CsWinTrain::CsWinTrain(CsComposition &comp, CsPlay &play, QWidget *parent) :
  CsWinScoreView( comp, play, parent ),
  mOffsetX(0),
  mOffsetY(0),
  mSizeY(0)
  {

  }



void CsWinTrain::paint()
  {
  QPainter painter(this);
  CsPainter cp( &painter, QStringLiteral(KEY_TRAIN_SETTINGS), mComposition, mPlayer, mOffsetX, size() );

  paintScore( cp );

  //Update editor's reference list
  //mReferenceList = cp.referenceList();
  }





void CsWinTrain::setupWinScroll(CsWinScoreMode *winScroll)
  {
  CsWinScoreView::setupWinScroll( winScroll );
  winScroll->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
  winScroll->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
  //Tracking scroll bar position
  connect( winScroll->verticalScrollBar(), &QScrollBar::valueChanged, this, [this] (int value) {
    mOffsetY = qBound( 0, value, mSizeY );
    update();
    });
  connect( winScroll->horizontalScrollBar(), &QScrollBar::valueChanged, this, [this] (int value) {
    mOffsetX = qBound( 0, value, 1024 );
    update();
    //originSet( SdPoint(value,originGet().y()) );
    });

  winScroll->horizontalScrollBar()->setRange( 0, 1024 );
  winScroll->horizontalScrollBar()->setSingleStep( 1 );
  winScroll->horizontalScrollBar()->setPageStep( 128 );
  //horizontalScrollBar()->setValue( mOrigin.x() );
  }


void CsWinTrain::upWheelEvent(QWheelEvent *event)
  {
  int delta = event->angleDelta().y() / 12;
  mOffsetY = qBound( 0, mOffsetY - delta, mSizeY );
  mWinScroll->verticalScrollBar()->setValue( mOffsetY );
  update();
  }




//!
//! \brief compositionChanged Called when composition changed outside
//!
void CsWinTrain::compositionChanged()
  {
  CsWinScoreView::compositionChanged();
  mOffsetX = mOffsetY = mSizeY = 0;
  update();
  }
