/*
Project "SaliScore Score music edit, view and tutorial programm"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Defines cursor position in score
*/
package salilab.saliscore;

public class CsCursorPosition extends CsJsonWritable {
  protected Integer mPosition;  //!< Position of cursor in tick
  protected Integer mLineIndex; //!< Line index of cursor

  CsCursorPosition() {
    mPosition = 0;
    mLineIndex = -1;
  }

  CsCursorPosition( CsCursorPosition cur ) {
    mPosition = cur.mPosition;
    mLineIndex = cur.mLineIndex;
  }

  //!
  //! \brief position Returns position of cursor in tick
  //! \return         Position of cursor in tick
  //!
  int      position() { return mPosition; }

  //!
  //! \brief lineIndex Returns line index of cursor
  //! \return          Line index of cursor
  //!
  int      lineIndex() { return mLineIndex; }

  void     set( int line, int pos ) { mLineIndex = line; mPosition = pos; }

  //!
  //! \brief jsonWrite Writes content into json writer object
  //! \param js        JSON writer object
  //!
  @Override
  void     jsonWrite( SvJsonWriter js ) throws Exception {
    js.jsonInt( "Pos", mPosition );
    js.jsonInt( "Line", mLineIndex );
  }

  //!
  //! \brief jsonRead Reads content from json reader object
  //! \param js       JSON reader object
  //!
  @Override
  void     jsonRead( SvJsonReader js ) throws Exception {
    js.jsonInt( "Pos", mPosition );
    js.jsonInt( "Line", mLineIndex );
  }

  @Override
  CsJsonWritable copy() {
    return new CsCursorPosition( this );
  }
}
