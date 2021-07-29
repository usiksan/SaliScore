#ifndef CSPOSITION_H
#define CSPOSITION_H

#include "CsConfig.h"

class SvJsonWriter;
class SvJsonReader;

class CsPosition
  {
    int mPosition; //!< Position in time scale, tick
  public:
    CsPosition();
    virtual ~CsPosition() {}

    virtual void jsonWrite( SvJsonWriter &js ) const;

    virtual void jsonRead( SvJsonReader &js );
  };

#endif // CSPOSITION_H
