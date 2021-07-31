#ifndef CSPOSITION_H
#define CSPOSITION_H

#include "CsConfig.h"
#include "CsJsonIO.h"

class CsPosition
  {
    int mPosition; //!< Position in time scale, tick
  public:
    CsPosition( int pos = 0 );
    virtual ~CsPosition() {}

    int          position() const { return mPosition; }

    virtual void jsonWrite( CsJsonWriter &js ) const;

    virtual void jsonRead( CsJsonReader &js );
  };

#endif // CSPOSITION_H
