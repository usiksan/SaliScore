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

    void jsonWrite( SvJsonWriter &js ) const;

    void jsonRead( SvJsonReader &js );
  };

#endif // CSCHORDKIT_H
