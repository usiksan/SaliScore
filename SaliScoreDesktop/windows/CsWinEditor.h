#ifndef CSWINEDITOR_H
#define CSWINEDITOR_H

#include "CsConfig.h"
#include "CsWinScoreMode.h"

#include <QWidget>
#include <QAbstractScrollArea>


class CsWinEditor : public CsWinScoreMode
  {
    Q_OBJECT
  public:
    explicit CsWinEditor( CsComposition &comp, CsPlay &play, QWidget *parent = nullptr);


  signals:

  };

#endif // CSWINEDITOR_H
