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
package salilab.saliscore;

public class CsTrainInterval extends CsJsonWritable {
  CsCursorPosition mStart; //!< Start position of interval
  CsCursorPosition mStop;  //!< Stop position of interval

  CsTrainInterval() {
    mStart = new CsCursorPosition();
    mStop  = new CsCursorPosition();
  }

  CsTrainInterval( CsTrainInterval tr ) {
    mStart = (CsCursorPosition) tr.mStart.copy();
    mStop  = (CsCursorPosition) tr.mStop.copy();
  }

  //!
  //! \brief jsonWrite Writes content into json writer object
  //! \param js        JSON writer object
  //!
  @Override
  void     jsonWrite( SvJsonWriter js ) throws Exception {
    js.jsonObject( "Start", mStart );
    js.jsonObject( "Stop", mStop );
  }

  //!
  //! \brief jsonRead Reads content from json reader object
  //! \param js       JSON reader object
  //!
  @Override
  void     jsonRead( SvJsonReader js ) throws Exception {
    js.jsonObject( "Start", mStart );
    js.jsonObject( "Stop", mStop );
  }

  @Override
  CsJsonWritable copy() {
    return new CsTrainInterval( this );
  }
}
