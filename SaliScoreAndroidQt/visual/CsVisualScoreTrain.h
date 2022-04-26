#ifndef CSVISUALSCORETRAIN_H
#define CSVISUALSCORETRAIN_H

#include "CsVisualScore.h"
#include "score/CsReference.h"

class CsVisualScoreTrain : public CsVisualScore
  {
    Q_OBJECT

    CsReferenceList mReferenceList; //!< List of areas on screen with appropriate reference to position in score
    int             mFragment;      //!< Active fragment
  public:
    CsVisualScoreTrain( CsComposition &comp, CsVisualWinMain *player, QWidget *parent = nullptr );

    // CsVisualAbstractList interface
  public:
    virtual void contentPaint(QPainter &painter) override;
  };

#endif // CSVISUALSCORETRAIN_H
