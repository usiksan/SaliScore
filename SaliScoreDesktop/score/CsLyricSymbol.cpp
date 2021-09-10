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
