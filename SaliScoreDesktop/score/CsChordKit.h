#ifndef CSCHORDKIT_H
#define CSCHORDKIT_H

#include "CsConfig.h"
#include "CsChordLine.h"

#include <QMap>

class CsChordKit
  {
    QMap<QString,CsChordLine> mChordLines;
  public:
    CsChordKit();
  };

#endif // CSCHORDKIT_H
