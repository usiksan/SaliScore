#include "CsWinTrain.h"
#include "CsWinScoreMode.h"
#include "CsWinMain.h"
#include "CsPainterTrain.h"

#include <QPaintEvent>
#include <QPainter>
#include <QSettings>
#include <QScrollBar>
#include <QWheelEvent>

CsWinTrain::CsWinTrain(CsComposition &comp, CsPlay &play, QWidget *parent) :
  CsWinScoreView( comp, play, parent ),
  mFragment(0)
  {

  }



void CsWinTrain::paint()
  {
  QPainter painter(this);
  CsPainterTrain cp( &painter, QStringLiteral(KEY_TRAIN_SETTINGS), mComposition, mPlayer, mOffsetX, size(), mFragment );

  paintScore( cp );

  //Update editor's reference list
  mReferenceList = cp.referenceList();
  }




void CsWinTrain::cmFragmentTrain()
  {

  }




void CsWinTrain::cmFragment0()
  {
  mFragment = 0;
  updateActions();
  update();
  }




void CsWinTrain::cmFragment1()
  {
  mFragment = 1;
  updateActions();
  update();
  }




void CsWinTrain::cmFragment2()
  {
  mFragment = 2;
  updateActions();
  update();
  }





void CsWinTrain::cmFragmentStart()
  {
  if( mFragment ) {
    mComposition.fragmentStartSet( mFragment - 1, mPlayer.lineIndex(), mPlayer.lineTickIndex() );
    update();
    }
  }





void CsWinTrain::cmFragmentStop()
  {
  if( mFragment ) {
    mComposition.fragmentStopSet( mFragment - 1, mPlayer.lineIndex(), mPlayer.lineTickIndex() );
    update();
    }
  }




void CsWinTrain::activate()
  {
  CsWinScoreView::activate();
  updateActions();
  }




void CsWinTrain::playStart()
  {
  if( !mPlayer.isShow() ) {
    //Begin from start of fragment
    mPlayer.setTrainFragment( mFragment );
    }
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





void CsWinTrain::upMousePressEvent(QMouseEvent *event)
  {
  for( int i = mReferenceList.count() - 1; i >= 0; i-- )
    if( mReferenceList.at(i).isHit( event->pos() ) ) {
      const CsReference &ref( mReferenceList.at(i) );

      switch( ref.type() ) {
        case cccChord :
        case cccNote :
          CsWinMain::actionPlayStop->trigger();
          mPlayer.jump( ref.line(), ref.index() );
          mPlayer.show(true);
          break;

        case cccRemark :
        case cccLyric :
        case cccTranslation :
        case cccLyricSymbol :
          CsWinMain::actionPlayStop->trigger();
          mPlayer.jump( ref.line(), 0 );
          mPlayer.show(true);
          break;

        }
      update();
      return;
      }
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
  mFragment = 0;
  updateActions();
  update();
  }



void CsWinTrain::updateActions() const
  {
  mPlayer.setTrainFragment( mFragment );
  CsWinMain::actionFragment0->setChecked( mFragment == 0 );
  CsWinMain::actionFragment1->setChecked( mFragment == 1 );
  CsWinMain::actionFragment2->setChecked( mFragment == 2 );
  CsWinMain::actionFragmentStart->setDisabled( mFragment == 0 );
  CsWinMain::actionFragmentStop->setDisabled( mFragment == 0 );
  }
