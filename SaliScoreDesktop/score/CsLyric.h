#ifndef CSLYRIC_H
#define CSLYRIC_H

#include "CsConfig.h"
#include "CsPosition.h"

#include <QString>
#include <QList>

class CsLyric : public CsPosition
  {
    QString mLyric;
  public:
    CsLyric();
  };

using CsLyricList = QList<CsLyric>;

#endif // CSLYRIC_H
