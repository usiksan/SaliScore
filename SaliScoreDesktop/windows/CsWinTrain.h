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

    virtual void upWheelEvent(QWheelEvent *event) override;

    //!
    //! \brief compositionChanged Called when composition changed outside
    //!
    virtual void compositionChanged() override;

  };

#endif // CSWINTRAIN_H
