#ifndef CSVISUALSCOREKARAOKE_H
#define CSVISUALSCOREKARAOKE_H

#include "CsVisualScore.h"

class CsVisualScoreKaraoke : public CsVisualScore
  {
    Q_OBJECT
  public:
    CsVisualScoreKaraoke( CsComposition &comp, QWidget *parent = nullptr );

    // CsVisualAbstractList interface
  public:
    virtual void contentPaint(QPainter &painter) override;
  };

#endif // CSVISUALSCOREKARAOKE_H
