#include "CsReference.h"

CsReference::CsReference(QRect r, int type, int line, const QString &part, int index) :
  mRect(r),
  mType(type),
  mLine(line),
  mPart(part),
  mIndex(index)
  {

  }

CsReference::CsReference(int x, int y, int width, int height, int type) :
  mRect( x, y, width, height ),
  mType( type ),
  mLine(-1),
  mIndex(-1)
  {

  }

CsReference::CsReference(int x, int y, int width, int height, int type, int line, const QString &part, int index) :
  mRect( x, y, width, height ),
  mType( type ),
  mLine(line),
  mPart(part),
  mIndex(index)
  {

  }
