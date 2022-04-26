#ifndef CSVISUALSCORE_H
#define CSVISUALSCORE_H

#include "SvLib/SvVisualAbstractList.h"
#include "CsPainter.h"

class CsComposition;
class CsVisualWinMain;

class CsVisualScore : public SvVisualAbstractList
  {
    Q_OBJECT

  protected:
    int              mStartX;      //!< Offset of horizontal begin view port in score (in pixels)
    bool             mAutoScroll;  //!< Automatic scroll content on cursor position changed
    CsComposition   &mComposition; //!< Current viewed composition
    CsVisualWinMain *mPlayer;      //!< Playback player
  public:
    CsVisualScore( CsComposition &comp, CsVisualWinMain *player, QWidget *parent = nullptr );

    void paintScore( CsPainter &cp );

    virtual void compositionChanged();

    virtual void activate();

  };

#endif // CSVISUALSCORE_H
