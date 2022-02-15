#include "CsRemark.h"
#include "../SvJson/SvJsonIO.h"

CsRemark::CsRemark()
  {

  }

CsRemark::CsRemark(const QString &lang, const QString &rem)
  {
  mRemarkMap.insert( lang, rem );
  }

void CsRemark::remarkRename(const QString &prevLang, const QString &newLang)
  {
  mRemarkMap.insert( newLang, mRemarkMap.value(prevLang) );
  mRemarkMap.remove( prevLang );
  }


void CsRemark::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonMapString( "StringMap", mRemarkMap );
  }

void CsRemark::jsonRead(CsJsonReader &js)
  {
  js.jsonMapString( "StringMap", mRemarkMap );
  }
