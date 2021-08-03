#ifndef CSWINTRAIN_H
#define CSWINTRAIN_H

#include "CsConfig.h"
#include "CsWinScoreMode.h"

#include <QWidget>

class CsWinTrain : public CsWinScoreMode
  {
    Q_OBJECT

  public:
    explicit CsWinTrain( CsComposition &comp, CsPlay &play, QWidget *parent = nullptr);

  signals:


    // QWidget interface
  protected:
    virtual void paintEvent(QPaintEvent *event) override;
  };

#endif // CSWINTRAIN_H
