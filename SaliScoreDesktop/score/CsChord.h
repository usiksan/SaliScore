#ifndef CSCHORD_H
#define CSCHORD_H

#include "CsConfig.h"
#include "CsPosition.h"

#include <QList>

class CsChord : public CsPosition
  {
  public:
    CsChord();
  };

//using CsChordPtr

using CsChordList = QList<CsChord>;

#endif // CSCHORD_H
