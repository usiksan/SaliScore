#ifndef CSWINKARAOKE_H
#define CSWINKARAOKE_H

#include "CsConfig.h"
#include "score/CsComposition.h"

#include <QWidget>

class CsWinKaraoke : public QWidget
  {
    Q_OBJECT


    CsComposition &mComposition;
  public:
    explicit CsWinKaraoke(CsComposition &comp, QWidget *parent = nullptr);

  signals:

  };

#endif // CSWINKARAOKE_H
