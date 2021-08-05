#ifndef CSPOSITION_H
#define CSPOSITION_H

#include "CsConfig.h"
#include "CsJsonIO.h"

class CsPosition
  {
    int mPosition; //!< Position in time scale, tick
    int mDuration; //!< Duration of this position, ticks
  public:
    CsPosition( int pos, int duration );
    virtual ~CsPosition() {}

    int          position() const { return mPosition; }

    int          duration() const { return mDuration; }

    virtual void jsonWrite( CsJsonWriter &js ) const;

    virtual void jsonRead( CsJsonReader &js );
  };

#endif // CSPOSITION_H
