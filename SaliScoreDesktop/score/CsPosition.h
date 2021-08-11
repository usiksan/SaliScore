#ifndef CSPOSITION_H
#define CSPOSITION_H

#include "CsConfig.h"
#include "CsJsonIO.h"

#include <QList>

class CsPosition
  {
    int mPosition; //!< Position in time scale, tick
    int mDuration; //!< Duration of this position, ticks
  public:
    CsPosition( int pos, int duration );
    virtual ~CsPosition() {}

    int          position() const { return mPosition; }
    void         positionSet( int pos ) { mPosition = pos; }

    int          duration() const { return mDuration; }

    virtual void jsonWrite( CsJsonWriter &js ) const;

    virtual void jsonRead( CsJsonReader &js );
  };

template <typename Position>
int csPositionFind( int position, const QList<Position> &list )
  {
  for( int i = 0; i < list.count(); i++ )
    if( list.at(i).position() == position )
      return i;
  return -1;
  }


template <typename Position>
int csPositionInsert( const Position &p, QList<Position> &list )
  {
  //Find index at which need to be inserted
  int index;
  for( index = 0; index < list.count(); index++ )
    if( list.at(index).position() > p.position() )
      break;

  if( index >= list.count() )
    //Simple append
    list.append( p );
  else
    list.insert( index, p );

  return index;
  }

#endif // CSPOSITION_H
