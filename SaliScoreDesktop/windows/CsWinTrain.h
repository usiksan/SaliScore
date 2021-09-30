#ifndef CSWINTRAIN_H
#define CSWINTRAIN_H

#include "CsConfig.h"
#include "CsWinScoreView.h"
#include "score/CsReference.h"

#include <QWidget>

class CsWinTrain : public CsWinScoreView
  {
    Q_OBJECT

    CsReferenceList mReferenceList; //!< List of areas on screen with appropriate reference to position in score
  public:
    explicit CsWinTrain( CsComposition &comp, CsPlay &play, QWidget *parent = nullptr);

    virtual void paint() override;
  signals:


    // CsWinScoreView interface
  public:
    virtual void playStart() override;
    virtual void setupWinScroll(CsWinScoreMode *winScroll) override;
    virtual void upMousePressEvent(QMouseEvent *event) override;
    virtual void upWheelEvent(QWheelEvent *event) override;

    //!
    //! \brief compositionChanged Called when composition changed outside
    //!
    virtual void compositionChanged() override;

  };

#endif // CSWINTRAIN_H
