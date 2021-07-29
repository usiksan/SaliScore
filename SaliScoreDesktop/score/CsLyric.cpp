#include "CsLyric.h"

#include "../SvJson/SvJsonIO.h"

CsLyric::CsLyric()
  {

  }


QJsonObject CsLyric::write() const
  {
  QJsonObject obj;
  SvJsonWriter js( obj );
  js.jsonString( "text", mLyric );

  return obj;
  }



void CsLyric::read(const QJsonObject &obj)
  {
  SvJsonReader js( obj );

  js.jsonString( "text", mLyric );
  }
