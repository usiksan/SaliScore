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

    QJsonObject write() const;

    void        read( const QJsonObject &obj );
  };

using CsLyricList = QList<CsLyric>;

#endif // CSLYRIC_H
