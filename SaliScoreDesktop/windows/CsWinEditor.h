#ifndef CSWINEDITOR_H
#define CSWINEDITOR_H

#include "CsConfig.h"
#include "CsWinScoreView.h"

#include <QWidget>


class CsWinEditor : public CsWinScoreView
  {
    Q_OBJECT
  public:
    explicit CsWinEditor( CsComposition &comp, CsPlay &play, QWidget *parent = nullptr);


    virtual void paint() override;
  signals:

  };

#endif // CSWINEDITOR_H
