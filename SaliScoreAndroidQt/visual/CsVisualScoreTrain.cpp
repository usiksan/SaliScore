#include "CsVisualScoreTrain.h"
#include "CsPainterTrain.h"

CsVisualScoreTrain::CsVisualScoreTrain(CsComposition &comp, QWidget *parent) :
  CsVisualScore( comp, parent )
  {

  }


void CsVisualScoreTrain::paintContent(QPainter &painter)
  {
  CsPainterTrain cp( &painter, QStringLiteral(KEY_TRAIN_SETTINGS), mComposition, mPlayer, mStartX, size(), mFragment );

  paintScore( cp );

  //Update editor's reference list
  mReferenceList = cp.referenceList();
  }
