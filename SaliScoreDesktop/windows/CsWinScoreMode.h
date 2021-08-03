#ifndef CSWINSCOREMODE_H
#define CSWINSCOREMODE_H

#include "CsConfig.h"
#include "score/CsComposition.h"
#include "play/CsPlay.h"

#include <QWidget>

class CsWinScoreMode : public QWidget
  {
    Q_OBJECT

  protected:
    CsComposition &mComposition;
    CsPlay        &mPlayer;
  public:
    explicit CsWinScoreMode( CsComposition &comp, CsPlay &play, QWidget *parent = nullptr);

  signals:

  };

#endif // CSWINSCOREMODE_H
