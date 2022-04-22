#include "CsVisualScoreKaraoke.h"
#include "CsPainterTrain.h"

CsVisualScoreKaraoke::CsVisualScoreKaraoke(CsComposition &comp, QWidget *parent) :
  CsVisualScore( comp, parent )
  {

  }


void CsVisualScoreKaraoke::contentPaint(QPainter &painter)
  {
  CsPainterTrain cp( &painter, QStringLiteral(KEY_TRAIN_SETTINGS), mComposition, nullptr, mStartX, size(), 0 );

  paintScore( cp );
  }
