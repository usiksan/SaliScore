#ifndef CSPLAYLIST_H
#define CSPLAYLIST_H

#include "CsPlayPart.h"
#include "Cs.h"

#include <QMap>

class CsPlayList
  {
    int            mVersion;
    CsPlayPartList mPartList;

    QMap<QString,
  public:
    CsPlayList();

    void jsonWrite( SvJsonWriter &writer ) const;

    void jsonRead( SvJsonReaderExtInt &reader );
  };

#endif // CSPLAYLIST_H
