#include "CsVisualScoreTrain.h"
#include "CsVisualWinMain.h"
#include "CsPainterTrain.h"

CsVisualScoreTrain::CsVisualScoreTrain(CsComposition &comp, CsVisualWinMain *player, QWidget *parent) :
  CsVisualScore( comp, player, parent ),
  mFragment(0)
  {

  }


void CsVisualScoreTrain::contentPaint(QPainter &painter)
  {
  CsCursorPosition play( mPlayer->playLinePosition(), mPlayer->playLineIndex() );
  CsPainterTrain cp( &painter, QStringLiteral(KEY_TRAIN_SETTINGS), mComposition, mPlayer->playIsRun() ? &play : nullptr, mStartX, size(), mFragment );

  paintScore( cp );

  //Update editor's reference list
  mReferenceList = cp.referenceList();
  }
