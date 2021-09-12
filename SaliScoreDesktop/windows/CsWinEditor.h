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

    CsCellCursor    mCellCursor;   //!< Current cursor position

    bool            mAutoScroll;   //!< Automatic scroll content on cursor position changed

    CsCursorEdit   *mEditor;       //!< Editor for content

    bool            mShift;        //!< True when Shift key pressed
    bool            mControl;      //!< True when Control key pressed

  public:
    explicit CsWinEditor( CsComposition &comp, CsPlay &play, QWidget *parent = nullptr);


    virtual void paint() override;

    virtual void playStart() override;

  private:
    //!
    //! \brief keyLeft Handle key left pressing
    //!
    void keyLeft();

    //!
    //! \brief keyRight Handle key right pressing
    //!
    void keyRight();

    //!
    //! \brief keyEnd Handle key End pressing
    //!
    void keyEnd();

    //!
    //! \brief keyTakt Append or remove takt from end of line
    //! \param plus    if true then takt appended in other hand - removed
    //!
    void keyTakt( bool plus );

    //!
    //! \brief keyDelete Handle key Delete pressing
    //!
    void keyDelete();

  signals:

    // CsWinScoreView interface
  public:
    virtual void setupWinScroll(CsWinScoreMode *winScroll) override;
    virtual void upWheelEvent(QWheelEvent *event) override;
    virtual void upMousePressEvent(QMouseEvent *event) override;
    virtual void upMouseReleaseEvent(QMouseEvent *event) override;
    virtual void upMouseMoveEvent(QMouseEvent *event) override;
    virtual void upKeyPressEvent(QKeyEvent *event) override;
    virtual void upKeyReleaseEvent(QKeyEvent *event) override;

    //!
    //! \brief compositionChanged Called when composition changed outside
    //!
    virtual void compositionChanged() override;
  };

#endif // CSWINEDITOR_H
