#include "CsVisualScoreKaraoke.h"
#include "CsVisualWinMain.h"
#include "CsPainterTrain.h"

CsVisualScoreKaraoke::CsVisualScoreKaraoke(CsComposition &comp, CsVisualWinMain *player, QWidget *parent) :
  CsVisualScore( comp, player, parent )
  {

  }


void CsVisualScoreKaraoke::contentPaint(QPainter &painter)
  {
  CsCursorPosition play( mPlayer->playLinePosition(), mPlayer->playLineIndex() );
  CsPainterTrain cp( &painter, QStringLiteral(KEY_TRAIN_SETTINGS), mComposition, mPlayer->playIsRun() ? &play : nullptr, mStartX, size(), 0 );

  paintScore( cp );
  }
