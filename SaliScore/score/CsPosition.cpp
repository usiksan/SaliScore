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
#include "CsPosition.h"



CsPosition::CsPosition(int pos, int duration) :
  mLinePosition(pos),
  mDuration(duration)
  {

  }



//!
//! \brief json Writes object into json writer
//! \param js   JSON writer
//!
void CsPosition::json(SvJsonWriter &js) const
  {
  js.jsonInt( "Duration", mDuration );
  js.jsonInt( "Position", mLinePosition );
  }




//!
//! \brief json Reads object from json reader
//! \param js   JSON reader
//!
void CsPosition::json(SvJsonReader &js)
  {
  js.jsonInt( "Duration", mDuration );
  js.jsonInt( "Position", mLinePosition );
  }



