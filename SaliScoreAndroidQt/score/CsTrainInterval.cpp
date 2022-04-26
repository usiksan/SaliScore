/*
Project "SaliScore Score music edit, view and tutorial programm"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Defines interval of score for training
*/
#include "CsTrainInterval.h"

CsTrainInterval::CsTrainInterval()
  {
  //By default we define maximum interval
  mStart.set( 0, 0 );
  mStop.set( 1000000, 1000000 );
  }




//!
//! \brief jsonWrite Writes content into json writer object
//! \param js        JSON writer object
//!
void CsTrainInterval::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonObject<CsCursorPosition>( "Start", mStart );
  js.jsonObject<CsCursorPosition>( "Stop", mStop );
  }




//!
//! \brief jsonRead Reads content from json reader object
//! \param js       JSON reader object
//!
void CsTrainInterval::jsonRead(CsJsonReader &js)
  {
  js.jsonObject<CsCursorPosition>( "Start", mStart );
  js.jsonObject<CsCursorPosition>( "Stop", mStop );
  }
