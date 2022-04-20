#ifndef CSWINTRAIN_H
#define CSWINTRAIN_H

#include "config.h"
#include "CsWinScoreView.h"
#include "score/CsReference.h"

#include <QWidget>

class CsWinTrain : public CsWinScoreView
  {
    Q_OBJECT

    CsReferenceList mReferenceList; //!< List of areas on screen with appropriate reference to position in score
    int             mFragment;      //!< Active fragment
  public:
    explicit CsWinTrain( CsComposition &comp, CsCursorPosition *play, QWidget *parent = nullptr);

    virtual void paint() override;

  public slots:
    void cmFragmentTrain();

    void cmFragment0();
    void cmFragment1();
    void cmFragment2();
    void cmFragmentStart();
    void cmFragmentStop();
  signals:


    // CsWinScoreView interface
  public:
    virtual void activate() override;
    virtual void playStart() override;
    virtual void setupWinScroll(CsWinScoreMode *winScroll) override;
    virtual void upMousePressEvent(QMouseEvent *event) override;
    virtual void upWheelEvent(QWheelEvent *event) override;

    //!
    //! \brief compositionChanged Called when composition changed outside
    //!
    virtual void compositionChanged() override;

  private:
    void updateActions() const;
  };

#endif // CSWINTRAIN_H
