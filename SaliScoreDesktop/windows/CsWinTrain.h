#ifndef CSWINTRAIN_H
#define CSWINTRAIN_H

#include "CsConfig.h"
#include "score/CsComposition.h"

#include <QWidget>

class CsWinTrain : public QWidget
  {
    Q_OBJECT

    CsComposition &mComposition;
  public:
    explicit CsWinTrain( CsComposition &comp, QWidget *parent = nullptr);

  signals:


    // QWidget interface
  protected:
    virtual void paintEvent(QPaintEvent *event) override;
  };

#endif // CSWINTRAIN_H
