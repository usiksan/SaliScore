#ifndef CSWINKARAOKE_H
#define CSWINKARAOKE_H

#include "CsConfig.h"
#include "CsWinScoreMode.h"
#include "CsKaraokePage.h"

#include <QWidget>

enum CsAnimationMode {
  camSingleLine
};

class CsWinKaraoke : public CsWinScoreMode
  {
    Q_OBJECT

    CsKaraokePageMap mPageMap;
  public:
    explicit CsWinKaraoke( CsComposition &comp, CsPlay &play, QWidget *parent = nullptr);

    void activate();
  signals:


    // QWidget interface
  protected:
    virtual void paintEvent(QPaintEvent *event) override;

  private:
    void buildSingleLine(CsPainter &cp);
  };

#endif // CSWINKARAOKE_H
