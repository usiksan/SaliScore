#include "CsWinTrain.h"
#include "CsPainter.h"

#include <QPaintEvent>
#include <QPainter>
#include <QSettings>

CsWinTrain::CsWinTrain(CsComposition &comp, CsState &st, QWidget *parent) :
  QWidget(parent),
  mComposition(comp),
  mState(st)
  {

  }


void CsWinTrain::paintEvent(QPaintEvent *event)
  {
  QPainter painter(this);
  QSettings settings;
  //Закрасить цветом фона
  QColor colorBack = QColor( (QRgb) (settings.value( KEY_TRAIN_COLOR_BACK, QVariant(QColor(Qt::white).rgb()) ).toUInt()) );
  painter.fillRect( QRect( QPoint(), size() ), colorBack );

  CsPainter cp( &painter, mState );
  auto list = mComposition.lineList();
  for( const auto &line : list )
    cp.drawLine( line );
  }
