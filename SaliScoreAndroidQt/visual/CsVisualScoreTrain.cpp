#include "CsVisualScoreTrain.h"
#include "CsPainterTrain.h"

CsVisualScoreTrain::CsVisualScoreTrain(CsComposition &comp, QWidget *parent) :
  CsVisualScore( comp, parent )
  {

  }


void CsVisualScoreTrain::contentPaint(QPainter &painter)
  {
  CsPainterTrain cp( &painter, QStringLiteral(KEY_TRAIN_SETTINGS), mComposition, nullptr, mStartX, size(), mFragment );

  paintScore( cp );

  //Update editor's reference list
  mReferenceList = cp.referenceList();
  }
