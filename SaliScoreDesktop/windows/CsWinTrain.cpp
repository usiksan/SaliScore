#include "CsWinTrain.h"
#include "CsPainter.h"

#include <QPaintEvent>
#include <QPainter>
#include <QSettings>

CsWinTrain::CsWinTrain(CsComposition &comp, CsPlay &play, QWidget *parent) :
  CsWinScoreView( comp, play, parent )
  {

  }

void CsWinTrain::paint()
  {
  QPainter painter(this);
  CsPainter cp( &painter, QStringLiteral(KEY_TRAIN_SETTINGS), mComposition, mPlayer );

  //Закрасить цветом фона
  painter.fillRect( QRect( QPoint(), size() ), cp.backgroundColor() );

  int posy = 35;
  for( int i = 0; i < mComposition.lineCount(); i++ )
    posy = cp.drawLine( posy, i, mComposition.line(i) );
  }


