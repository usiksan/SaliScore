#ifndef CSWINTRAIN_H
#define CSWINTRAIN_H

#include "CsConfig.h"
#include "score/CsComposition.h"
#include "score/CsState.h"

#include <QWidget>

class CsWinTrain : public QWidget
  {
    Q_OBJECT

    CsComposition &mComposition;
    CsState       &mState;
  public:
    explicit CsWinTrain( CsComposition &comp, CsState &st, QWidget *parent = nullptr);

  signals:


    // QWidget interface
  protected:
    virtual void paintEvent(QPaintEvent *event) override;
  };

#endif // CSWINTRAIN_H
