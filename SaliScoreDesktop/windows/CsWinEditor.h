#ifndef CSWINEDITOR_H
#define CSWINEDITOR_H

#include "CsConfig.h"
#include "CsWinScoreView.h"
#include "CsCellCursor.h"
#include "CsCursorEdit.h"
#include "score/CsReference.h"

#include <QWidget>


class CsWinEditor : public CsWinScoreView
  {
    Q_OBJECT

    CsReferenceList mReferenceList;
    int             mOffsetX;
    int             mOffsetY;
    int             mSizeY;

    CsCellCursor    mCellCursor;

    bool            mAutoScroll;   //!< Automatic scroll content on cursor position changed

    CsCursorEdit   *mEditor;       //!< Editor for content

    bool            mShift;
    bool            mControl;

  public:
    explicit CsWinEditor( CsComposition &comp, CsPlay &play, QWidget *parent = nullptr);


    virtual void paint() override;
  signals:

    // CsWinScoreView interface
  public:
    virtual void setupWinScroll(CsWinScoreMode *winScroll) override;

    // CsWinScoreView interface
  public:
    virtual void upWheelEvent(QWheelEvent *event) override;
    virtual void upMousePressEvent(QMouseEvent *event) override;
    virtual void upMouseReleaseEvent(QMouseEvent *event) override;
    virtual void upMouseMoveEvent(QMouseEvent *event) override;
    virtual void upKeyPressEvent(QKeyEvent *event) override;
    virtual void upKeyReleaseEvent(QKeyEvent *event) override;
  };

#endif // CSWINEDITOR_H
