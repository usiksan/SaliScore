/*
Project "SaliScore Score music edit, view and tutorial programm"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  CsLyricSymbol represents single symbol of lyric. Symbol may be one of char or alignment

  Alignment is offset from begin of note. We represent offset with 1/16 step.
*/
#ifndef CSLYRICSYMBOL_H
#define CSLYRICSYMBOL_H

#include <QString>
#include <QList>

#define CsLyricAlignPrefix QChar('%')

//!
//! \brief The CsLyricSymbol class represents single symbol of lyric. Symbol may be one of char or alignment
//!
class CsLyricSymbol
  {
    QString     mChar;  //!< Textual part of symbol
    int         mAlign; //!< Musical alignment of symbol position
  public:
    mutable int mWidth; //!< Width of symbol in pixels
    mutable int mPosX;  //!< Visual position of symbol in pixels

    CsLyricSymbol( QChar ch );

    CsLyricSymbol(char alignCode );

    //!
    //! \brief isChar Returns true if this symbol is char
    //! \return       true if this symbol is char
    //!
    bool    isChar() const { return mAlign == 0; }

    //!
    //! \brief isAlign Returns true if this symbol is alignment
    //! \return        true if this symbol is alignment
    //!
    bool    isAlign() const { return !isChar(); }

    bool    isDelimiter() const { return isChar() && (mChar.at(0).isSpace() || mChar.at(0) == QChar('-')); }

    //!
    //! \brief string Convert to string representation of symbol
    //! \return       String representation of symbol
    //!
    QString string() const;

    void    stringSet( const QChar ch );


    int     align() const { return mAlign; }

    void    alignSet( int al ) { mAlign = al; mChar.clear(); }

  };

using CsLyricLine = QList<CsLyricSymbol>;


#endif // CSLYRICSYMBOL_H
