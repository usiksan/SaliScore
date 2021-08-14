#ifndef CSPLAYLIST_H
#define CSPLAYLIST_H

#include "CsPlayPart.h"

class CsPlayList
  {
    int            mVersion;
    CsPlayPartList mPartList;
  public:
    CsPlayList();

    void jsonWrite( SvJsonWriter &writer ) const;

    void jsonRead( SvJsonReaderExtInt &reader );
  };

#endif // CSPLAYLIST_H
