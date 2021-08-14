#ifndef CSPLAYPART_H
#define CSPLAYPART_H

#include "CsPlayComposition.h"

class CsPlayPart
  {
    QString               mTitle;
    CsPlayCompositionList mCompositionList;
  public:
    CsPlayPart();

    void jsonWrite( SvJsonWriter &writer ) const;

    void jsonRead( SvJsonReaderExtInt &reader );

  };

using CsPlayPartList = QList<CsPlayPart>;

#endif // CSPLAYPART_H
