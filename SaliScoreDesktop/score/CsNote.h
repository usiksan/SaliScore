#ifndef CSNOTE_H
#define CSNOTE_H

#include "CsConfig.h"
#include "CsPosition.h"

#include <QList>

class CsNote : public CsPosition
  {
    int mDuration; //!< Note duration, ticks
    int mPitch;    //!< Pitch from begin of one-lined octave
  public:
    CsNote();

    // CsPosition interface
  public:
    virtual void jsonWrite(SvJsonWriter &js) const override;
    virtual void jsonRead(SvJsonReader &js) override;
  };

using CsNoteList = QList<CsNote>;

#endif // CSNOTE_H
