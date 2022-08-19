/*
Project "SaliScore Score music edit, view and tutorial program"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base for position in score (notes, chords, visual cursors)
*/
#ifndef CSPOSITION_H
#define CSPOSITION_H

#include "CsConfig.h"
#include "SvLib/SvJsonIO.h"

#include <QList>

class CsPosition
  {
    int mLinePosition; //!< Position in time scale from begin of line, tick
    int mDuration;     //!< Duration of this position, ticks
  public:
    CsPosition( int pos, int duration );
    virtual ~CsPosition() {}

    //!
    //! \brief position Returns position of CsPosition in tick from begin line
    //! \return         Position of CsPosition in tick from begin line
    //!
    int          position() const { return mLinePosition; }

    //!
    //! \brief positionSet Sets new position in tick from begin line
    //! \param pos         New position in tick from begin line
    //!
    void         positionSet( int pos ) { mLinePosition = pos; }



    //!
    //! \brief duration Returns duration of CsPosition in tick
    //! \return         Duration of CsPosition in tick
    //!
    int          duration() const { return mDuration; }

    //!
    //! \brief durationSet Sets new duration in tick
    //! \param d           New duration in tick
    //!
    void         durationSet( int d ) { mDuration = d; }


    //!
    //! \brief isBeginInside Check if position of CsPosition is inside given interval
    //! \param tickStart     Begin of interval
    //! \param tickStop      End of interval
    //! \return              true if position of CsPosition is inside given interval
    //!
    bool         isBeginInside( int tickStart, int tickStop ) const { return tickStart <= mLinePosition && mLinePosition < tickStop; }


    //!
    //! \brief json Writes object into json writer
    //! \param js   JSON writer
    //!
    virtual void json( SvJsonWriter &js ) const;

    //!
    //! \brief json Reads object from json reader
    //! \param js   JSON reader
    //!
    virtual void json( SvJsonReader &js );
  };




//!
//! \brief csPositionFind Algorithm to find item in list with exact position
//! \param position       Position to find item
//! \param list           List of items where to find
//! \return               Index of found item or -1 if item not found
//!
template <typename Position>
int csPositionFind( int position, const QList<Position> &list )
  {
  for( int i = 0; i < list.count(); i++ )
    if( list.at(i).position() == position )
      return i;
  return -1;
  }




//!
//! \brief csPositionInsert Insert item in right position in list
//! \param p                Item to insert
//! \param list             List where insert
//! \return                 Index of inserted item
//!
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
