#ifndef CSPAINTEREDITOR_H
#define CSPAINTEREDITOR_H

#include "CsPainter.h"
#include "CsCursorEdit.h"

class CsPainterEditor : public CsPainter
  {
    CsCursorEdit *mCursorEdit;
  public:
    CsPainterEditor( QPainter *painter, const QString &keyViewSettings, const CsComposition &comp, const CsPlay &player, int offsetX, QSize size, CsCellCursor *cellCursor, CsCursorEdit *cursorEdit );

    // CsPainter interface
  protected:
    virtual bool isNotEditProperty(int propertyId, int x, int y) override;

  private:
    void paintEditText( int x, int y );
  };

#endif // CSPAINTEREDITOR_H
