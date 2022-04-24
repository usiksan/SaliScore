#ifndef CSVISUALWINMAIN_H
#define CSVISUALWINMAIN_H

#include "score/CsComposition.h"

#include <QMainWindow>

class CsVisualWinMain : public QMainWindow
  {
    Q_OBJECT

  protected:
    CsComposition mComposition;   //!< Current composition

    //Player part
  public:
    explicit CsVisualWinMain(QWidget *parent = nullptr);

  signals:

  };

#endif // CSVISUALWINMAIN_H
