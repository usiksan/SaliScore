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
#ifndef CSCURSORPOSITION_H
#define CSCURSORPOSITION_H

#include "CsConfig.h"
#include "SvLib/SvJsonIO.h"

class CsCursorPosition
  {
  protected:
    int     mLinePosition;  //!< Position of cursor in tick from begin of line
    int     mLineIndex;     //!< Line index of cursor
  public:
    CsCursorPosition();
    CsCursorPosition( int position, int index ) : mLinePosition(position), mLineIndex(index) {}

    //!
    //! \brief linePosition Returns position of cursor in tick from begin of line
    //! \return             Position of cursor in tick from begin of line
    //!
    int      linePosition() const { return mLinePosition; }

    //!
    //! \brief lineIndex Returns line index of cursor
    //! \return          Line index of cursor
    //!
    int      lineIndex() const { return mLineIndex; }

    //!
    //! \brief set          Set new cursor position
    //! \param lineIndex    New line index of cursor
    //! \param linePosition New line position of cursor
    //!
    void     set( int lineIndex, int linePosition ) { mLineIndex = lineIndex; mLinePosition = linePosition; }


    //!
    //! \brief isHit    Returns true if current play position is hit into given interval
    //! \param position Start position of interval
    //! \param duration Duration of interval
    //! \return         true if current play position is hit into given interval
    bool     isHit( int position, int duration ) const { return position <= mLinePosition && mLinePosition < (position + duration); }

    bool     isLess( int lineIndex, int linePosition ) const { return mLineIndex < lineIndex || (mLineIndex == lineIndex && mLinePosition < linePosition); }

    //!
    //! \brief jsonWrite Writes content into json writer object
    //! \param js        JSON writer object
    //!
    void     json( SvJsonWriter &js ) const;

    //!
    //! \brief jsonRead Reads content from json reader object
    //! \param js       JSON reader object
    //!
    void     json( SvJsonReader &js );
  };

#endif // CSCURSORPOSITION_H
