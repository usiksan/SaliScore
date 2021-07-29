#include "CsComposition.h"

#include "../SvJson/SvJsonIO.h"

CsComposition::CsComposition()
  {

  }

QJsonObject CsComposition::write() const
  {
  QJsonObject obj;
  SvJsonWriter js( obj );

  return obj;
  }
