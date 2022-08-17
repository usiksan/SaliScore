#ifndef CSPAINTEREDITOR_H
#define CSPAINTEREDITOR_H

#include "CsPainter.h"
#include "CsCursorEdit.h"

#include <QSet>

class CsPainterEditor : public CsPainter
  {
    CsCursorEdit *mCursorEdit;
    QSet<int>     mSelectedLines;
  public:
    CsPainterEditor( QPainter *painter, const QString &keyViewSettings, const CsComposition &comp, const CsCursorPosition *player, int offsetX, QSize size, CsCellCursor *cellCursor, CsCursorEdit *cursorEdit, const QSet<int> selectedLines );

    // CsPainter interface
  protected:
    virtual bool isNotEditProperty( const QString &propertyId, int x, int y) override;
    virtual bool isNotEditRemark(const QString &part, int x, int y) override;
    virtual bool isNotEditChord(const QString &part, int position, int x, int y) override;

    //!
    //! \brief isNotEditNote Test if this position not edited
    //! \param part          Part of score line
    //! \param position      Time position inside line
    //! \param x             x position of note in pixels
    //! \param scoreY        y position of score line
    //! \param noteStart     base note of score line (clef)
    //! \return              true if this position not edited
    //!
    virtual bool isNotEditNote(const QString &part, int position, int x, int scoreY, int noteStart) override;
    virtual bool isNotEditLyric(QVector<CsLyricDisposition> &disposition) override;
    virtual bool isNotEditTranslation(const QString &part, int x, int y) override;

    //!
    //! \brief disableDrawActiveNote Disables drawing active note
    //! \return                      true drawing active note disabled
    //!
    virtual bool disableDrawActiveNote() const override { return mCursorEdit != nullptr && mCursorEdit->isMatch( cccNote ); }

  private:
    void paintEditText( int x, int y );

    // CsPainter interface
  public:
    virtual void drawLineBackground(int lineHeight) override;
  };

#endif // CSPAINTEREDITOR_H
