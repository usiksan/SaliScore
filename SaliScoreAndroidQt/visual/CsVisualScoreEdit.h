#ifndef CSVISUALSCOREEDIT_H
#define CSVISUALSCOREEDIT_H

#include "CsVisualScore.h"

class CsCursorEdit;

class CsVisualScoreEdit : public CsVisualScore
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
    CsVisualScoreEdit( CsComposition &comp, CsVisualWinMain *player, QWidget *parent = nullptr );


  signals:
    void actionEditPaste( bool enable );
    void actionEditSelectionPresent( bool enable );
    void scoreSettings();

  public slots:
    //Menu Edit
    void cmEditUndo();
    void cmEditRedo();
    void cmEditCut();
    void cmEditCopy();
    void cmEditPaste();
    void cmEditDelete();
    void cmEditInsertRemark() { editInsertLine( true ); }
    void cmEditInsertNoteLine() { editInsertLine( false ); }
    void cmEditAppendRemark() { editAppendLine( true ); }
    void cmEditAppendNoteLine() { editAppendLine( false ); }
    void cmEditDeleteLine();

    void cmNoteShiftUp();
    void cmNoteShiftDn();
    void cmNoteDurationLess();
    void cmNoteDurationGrow();

    // CsVisualAbstractList interface
  public:
    virtual void contentPaint(QPainter &painter) override;
    virtual void contentClicked(int x, int y) override;

    // QWidget interface
  protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

  private:
    void editInsertLine( bool remark );

    void editAppendLine( bool remark );

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

    void keyToneShift(bool up );

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

    //!
    //! \brief insertActiveNote Inserts active note
    //!
    void insertActiveNote();
  };

#endif // CSVISUALSCOREEDIT_H
