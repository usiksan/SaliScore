#ifndef CSLYRIC_H
#define CSLYRIC_H

#include "CsConfig.h"
#include "CsPosition.h"

#include <QString>
#include <QList>
#include <QJsonObject>

class CsLyric : public CsPosition
  {
    QString mLyric;
  public:
    CsLyric();


    // CsPosition interface
  public:
    virtual void jsonWrite(SvJsonWriter &js) const override;
    virtual void jsonRead(SvJsonReader &js) override;
  };

using CsLyricList = QList<CsLyric>;

#endif // CSLYRIC_H
