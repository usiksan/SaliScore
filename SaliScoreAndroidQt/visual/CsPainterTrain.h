#ifndef CSPAINTERTRAIN_H
#define CSPAINTERTRAIN_H

#include "CsPainter.h"

class CsPainterTrain : public CsPainter
  {
    CsTrainInterval mFragment;
    bool            mShowFragment;
  public:
    CsPainterTrain( QPainter *painter, const QString &keyViewSettings, const CsComposition &comp, const CsCursorPosition *player, int offsetX, QSize size, int fragmentIndex );


    // CsPainter interface
  public:
    virtual void drawLineBackground(int lineHeight) override;
  };

#endif // CSPAINTERTRAIN_H
