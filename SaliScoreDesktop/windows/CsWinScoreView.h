#ifndef CSWINSCOREVIEW_H
#define CSWINSCOREVIEW_H

#include "CsConfig.h"
#include "score/CsComposition.h"
#include "play/CsPlay.h"

#include <QWidget>

class CsWinScoreView : public QWidget
  {
    Q_OBJECT

  protected:
    CsComposition &mComposition;
    CsPlay        &mPlayer;
  public:
    explicit CsWinScoreView( CsComposition &comp, CsPlay &play, QWidget *parent = nullptr);

    virtual void activate() { update(); }

    virtual void paint() = 0;
  signals:

  };

#endif // CSWINSCOREVIEW_H
