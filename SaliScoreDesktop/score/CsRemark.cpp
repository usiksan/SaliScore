#include "CsRemark.h"
#include "../SvJson/SvJsonIO.h"
#include "../windows/CsPainter.h"

CsRemark::CsRemark()
  {

  }

CsRemark::CsRemark(const QString &lang, const QString &rem)
  {
  mRemarkMap.insert( lang, rem );
  }


void CsRemark::jsonWrite(CsJsonWriter &js) const
  {
  js.jsonMapString( "StringMap", mRemarkMap );
  }

void CsRemark::jsonRead(CsJsonReader &js)
  {
  js.jsonMapString( "StringMap", mRemarkMap );
  }
