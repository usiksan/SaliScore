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
#ifndef CSCURSORPOSITION_H
#define CSCURSORPOSITION_H

#include "config.h"
#include "CsJsonIO.h"

class CsCursorPosition
  {
  protected:
    int     mPosition;  //!< Position of cursor in tick
    int     mLineIndex; //!< Line index of cursor
  public:
    CsCursorPosition();

    //!
    //! \brief position Returns position of cursor in tick
    //! \return         Position of cursor in tick
    //!
    int      position() const { return mPosition; }

    //!
    //! \brief lineIndex Returns line index of cursor
    //! \return          Line index of cursor
    //!
    int      lineIndex() const { return mLineIndex; }

    void     set( int line, int pos ) { mLineIndex = line; mPosition = pos; }

    //!
    //! \brief jsonWrite Writes content into json writer object
    //! \param js        JSON writer object
    //!
    void     jsonWrite( CsJsonWriter &js ) const;

    //!
    //! \brief jsonRead Reads content from json reader object
    //! \param js       JSON reader object
    //!
    void     jsonRead( CsJsonReader &js );
  };

#endif // CSCURSORPOSITION_H
