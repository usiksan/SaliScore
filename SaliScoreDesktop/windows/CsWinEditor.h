#ifndef CSWINEDITOR_H
#define CSWINEDITOR_H

#include "CsConfig.h"
#include "CsWinScoreView.h"
#include "CsCellCursor.h"
#include "CsCursorEdit.h"
#include "score/CsReference.h"

#include <QWidget>
#include <QSet>


class CsWinEditor : public CsWinScoreView
  {
    Q_OBJECT

    CsReferenceList mReferenceList; //!< List of areas on screen with appropriate reference to position in score

    CsCellCursor    mCellCursor;    //!< Current cursor position
    CsCursorEdit   *mEditor;        //!< Editor for content
    QSet<int>       mSelectedLines; //!< Set of selected lines

    bool            mShift;         //!< True when Shift key pressed
    bool            mControl;       //!< True when Control key pressed

    CsLineList      mLineClipboard; //!< Local clipboard for selected lines

  public:
    explicit CsWinEditor( CsComposition &comp, CsPlay &play, QWidget *parent = nullptr);


    virtual void paint() override;


  public slots:
    //Menu Edit
    void cmEditUndo();
    void cmEditRedo();
    void cmEditCut();
    void cmEditCopy();
    void cmEditPaste();
    void cmEditDelete();

  private:
    //!
    //! \brief editCopy Copies selected lines to local clipboard
    //!
    void editCopy();

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

    //!
    //! \brief unselectAll Remove all selections
    //!
    void unselectAll();

  signals:

    // CsWinScoreView interface
  public:
    virtual void playStart() override;
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
