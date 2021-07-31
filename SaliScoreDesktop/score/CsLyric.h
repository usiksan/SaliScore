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

    QString lyric() const { return mLyric; }
    void    setLyric( const QString lyr ) { mLyric = lyr; }

    // CsPosition interface
  public:
    virtual void jsonWrite(CsJsonWriter &js) const override;
    virtual void jsonRead(CsJsonReader &js) override;
  };

using CsLyricList = QList<CsLyric>;

#endif // CSLYRIC_H
