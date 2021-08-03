#ifndef CSWINKARAOKE_H
#define CSWINKARAOKE_H

#include "CsConfig.h"
#include "CsWinScoreMode.h"

#include <QWidget>

class CsWinKaraoke : public CsWinScoreMode
  {
    Q_OBJECT

  public:
    explicit CsWinKaraoke( CsComposition &comp, CsPlay &play, QWidget *parent = nullptr);

  signals:

  };

#endif // CSWINKARAOKE_H
