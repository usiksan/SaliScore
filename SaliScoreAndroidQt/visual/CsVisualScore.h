#ifndef CSVISUALSCORE_H
#define CSVISUALSCORE_H

#include "CsVisualAbstractList.h"
#include "CsPainter.h"

class CsComposition;

class CsVisualScore : public CsVisualAbstractList
  {
    Q_OBJECT

  protected:
    int             mStartX;      //!< Offset of horizontal begin view port in score (in pixels)
    bool            mAutoScroll;  //!< Automatic scroll content on cursor position changed
    CsComposition  &mComposition;  //!< Current viewed composition
  public:
    CsVisualScore( CsComposition &comp, QWidget *parent = nullptr );

    void paintScore( CsPainter &cp );

    virtual void compositionChanged();

  };

#endif // CSVISUALSCORE_H
