#ifndef CSWINTRAIN_H
#define CSWINTRAIN_H

#include "CsConfig.h"
#include "CsWinScoreView.h"

#include <QWidget>

class CsWinTrain : public CsWinScoreView
  {
    Q_OBJECT

  public:
    explicit CsWinTrain( CsComposition &comp, CsPlay &play, QWidget *parent = nullptr);

    virtual void paint() override;
  signals:

  };

#endif // CSWINTRAIN_H
