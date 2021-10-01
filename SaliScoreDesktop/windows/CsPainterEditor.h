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
    CsPainterEditor( QPainter *painter, const QString &keyViewSettings, const CsComposition &comp, const CsPlay &player, int offsetX, QSize size, CsCellCursor *cellCursor, CsCursorEdit *cursorEdit, const QSet<int> selectedLines );

    // CsPainter interface
  protected:
    virtual bool isNotEditProperty(int propertyId, int x, int y) override;
    virtual bool isNotEditRemark(const QString &part, int x, int y) override;
    virtual bool isNotEditChord(const QString &part, int position, int x, int y) override;
    virtual bool isNotEditNote(const QString &part, int position, int x, int scoreY, int noteStart) override;
    virtual bool isNotEditLyric(QVector<CsLyricDisposition> &disposition) override;
    virtual bool isNotEditTranslation(const QString &part, int x, int y) override;

  private:
    void paintEditText( int x, int y );

    // CsPainter interface
  public:
    virtual void drawLineBackground(int lineHeight) override;
  };

#endif // CSPAINTEREDITOR_H
