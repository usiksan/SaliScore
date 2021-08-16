#ifndef CSWINKARAOKE_H
#define CSWINKARAOKE_H

#include "CsConfig.h"
#include "CsWinScoreView.h"
#include "CsKaraokePage.h"

#include <QWidget>

enum CsAnimationMode {
  camSingleLine
};

class CsWinKaraoke : public CsWinScoreView
  {
    Q_OBJECT

    CsKaraokePageMap mPageMap;
  public:
    explicit CsWinKaraoke( CsComposition &comp, CsPlay &play, QWidget *parent = nullptr);

    virtual void activate() override;

    virtual void paint() override;

    //!
    //! \brief compositionChanged Called when composition changed outside
    //!
    virtual void compositionChanged() override;
  signals:


  private:
    void buildSingleLine(CsPainter &cp);
  };

#endif // CSWINKARAOKE_H
