#include "CsLyricSymbol.h"

//                           0               0               0               0               0
static char alignChars[65] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+-";


CsLyricSymbol::CsLyricSymbol(QChar ch) :
  mChar( ch ),
  mAlign(0)
  {

  }

CsLyricSymbol::CsLyricSymbol(char alignCode) :
  mAlign(0)
  {
  int index;
  for( index = 0; index < 64 && alignChars[index] != alignCode; index++ );
  if( index < 64 )
    mAlign = index * 16;
  else
    mChar = CsLyricAlignPrefix;
  }




CsLyricSymbol::CsLyricSymbol(int align) :
  mAlign(align)
  {

  }




QString CsLyricSymbol::string() const
  {
  if( mAlign != 0 )
    return QString(CsLyricAlignPrefix) + QChar(alignChars[mAlign >> 4]);
  return mChar;
  }

void CsLyricSymbol::stringSet(const QChar ch)
  {
  mChar = ch;
  mAlign = 0;
  }




QString lyricLineToString(const CsLyricLine &line)
  {
  QString str;
  for( auto const &lyricSymbol : line )
    str.append( lyricSymbol.string() );
  return str;
  }




CsLyricLine lyricLineFromString(const QString &str)
  {
  CsLyricLine lyricLine;
  lyricLine.reserve( str.count() );
  for( int i = 0; i < str.count(); i++ )
    if( str.at(i) == CsLyricAlignPrefix && (i + 1) < str.count() ) {
      i++;
      //Append symbol as alignment
      lyricLine.append( CsLyricSymbol( str.at(i).toLatin1() ) );
      }
    else
      //Append simple symbol
      lyricLine.append( CsLyricSymbol( str.at(i) ) );

  return lyricLine;
  }
