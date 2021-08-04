#ifndef CSWINSCOREMODE_H
#define CSWINSCOREMODE_H

#include "CsConfig.h"
#include "score/CsComposition.h"
#include "play/CsPlay.h"
#include "CsWinScoreView.h"

#include <QWidget>
#include <QAbstractScrollArea>

class CsWinScoreMode : public QAbstractScrollArea
  {
    Q_OBJECT

    CsWinScoreView *mView;
  public:
    explicit CsWinScoreMode( CsWinScoreView *view, QWidget *parent = nullptr);

    CsWinScoreView *view() { return mView; }
  signals:


    // QWidget interface
  protected:
    virtual void paintEvent(QPaintEvent *event) override;
  };

#endif // CSWINSCOREMODE_H
