#ifndef CSWINTRAIN_H
#define CSWINTRAIN_H

#include "CsConfig.h"
#include "CsWinScoreView.h"

#include <QWidget>

class CsWinTrain : public CsWinScoreView
  {
    Q_OBJECT

    int mOffsetX;
    int mOffsetY;
    int mSizeY;
  public:
    explicit CsWinTrain( CsComposition &comp, CsPlay &play, QWidget *parent = nullptr);

    virtual void paint() override;
  signals:


    // CsWinScoreView interface
  public:
    virtual void setupWinScroll(CsWinScoreMode *winScroll) override;

    // CsWinScoreView interface
  public:
    virtual void upWheelEvent(QWheelEvent *event) override;
  };

#endif // CSWINTRAIN_H
