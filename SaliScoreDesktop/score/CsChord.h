#ifndef CSCHORD_H
#define CSCHORD_H

#include "CsConfig.h"
#include "CsPosition.h"

#include <QList>

class CsChord : public CsPosition
  {
  public:
    CsChord();

    // CsPosition interface
  public:
    virtual void jsonWrite(SvJsonWriter &js) const override;
    virtual void jsonRead(SvJsonReader &js) override;
  };

//using CsChordPtr

using CsChordList = QList<CsChord>;

#endif // CSCHORD_H
