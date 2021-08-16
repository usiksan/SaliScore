#ifndef CSWINSCOREVIEW_H
#define CSWINSCOREVIEW_H

#include "CsConfig.h"
#include "score/CsComposition.h"
#include "play/CsPlay.h"

#include <QWidget>

class CsWinScoreMode;

class CsWinScoreView : public QWidget
  {
    Q_OBJECT

  protected:
    CsComposition  &mComposition;
    CsPlay         &mPlayer;
    CsWinScoreMode *mWinScroll;   //!< Parent window which is QAbstractScrollArea
  public:
    explicit CsWinScoreView( CsComposition &comp, CsPlay &play, QWidget *parent = nullptr);

    //!
    //! \brief compositionChanged Called when composition changed outside
    //!
    virtual void compositionChanged();

    virtual void activate();

    virtual void paint() = 0;

    virtual void setupWinScroll( CsWinScoreMode *winScroll );

    virtual void upMousePressEvent(QMouseEvent *event) { Q_UNUSED(event) }
    virtual void upMouseReleaseEvent(QMouseEvent *event) { Q_UNUSED(event) }
    virtual void upMouseMoveEvent(QMouseEvent *event) { Q_UNUSED(event) }
    virtual void upWheelEvent(QWheelEvent *event) { Q_UNUSED(event) }
    virtual void upKeyPressEvent(QKeyEvent *event) { Q_UNUSED(event) }
    virtual void upKeyReleaseEvent(QKeyEvent *event) { Q_UNUSED(event) }
  signals:

  public slots:
    void viewUpdate();
  };

#endif // CSWINSCOREVIEW_H
