#ifndef CSWINSCOREMODE_H
#define CSWINSCOREMODE_H

#include "config.h"
#include "score/CsComposition.h"
#include "CsWinScoreView.h"

#include <QWidget>
#include <QAbstractScrollArea>

class CsWinScoreMode : public QAbstractScrollArea
  {
    Q_OBJECT

    CsWinScoreView *mView;
  public:
    explicit CsWinScoreMode( CsWinScoreView *view, QWidget *parent = nullptr);

    CsWinScoreView *view() { return mView; }
  signals:


    // QWidget interface
  protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override { mView->upMousePressEvent(event); }
    virtual void mouseReleaseEvent(QMouseEvent *event) override { mView->upMouseReleaseEvent(event); }
    virtual void mouseMoveEvent(QMouseEvent *event) override { mView->upMouseMoveEvent(event); }
    virtual void wheelEvent(QWheelEvent *event) override { mView->upWheelEvent(event); }
    virtual void keyPressEvent(QKeyEvent *event) override { mView->upKeyPressEvent(event); }
    virtual void keyReleaseEvent(QKeyEvent *event) override { mView->upKeyReleaseEvent(event); }
  };

#endif // CSWINSCOREMODE_H
