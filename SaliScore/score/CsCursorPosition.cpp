/*
Project "SaliScore Score music edit, view and tutorial program"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Defines cursor position in score
*/
#include "CsCursorPosition.h"

CsCursorPosition::CsCursorPosition() :
  mLinePosition(0),       //!< Position of cursor in tick
  mLineIndex(-1)      //!< Line index of cursor
  {

  }




//!
//! \brief jsonWrite Writes content into json writer object
//! \param js        JSON writer object
//!
void CsCursorPosition::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonInt( "Pos", mLinePosition );
  js.jsonInt( "Line", mLineIndex );
  }




//!
//! \brief jsonRead Reads content from json reader object
//! \param js       JSON reader object
//!
void CsCursorPosition::jsonRead(CsJsonReader &js)
  {
  js.jsonInt( "Pos", mLinePosition );
  js.jsonInt( "Line", mLineIndex );
  }
