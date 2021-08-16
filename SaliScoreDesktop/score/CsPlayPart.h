#ifndef CSPLAYPART_H
#define CSPLAYPART_H

#include "CsJsonIO.h"

#include <QString>
#include <QList>


class CsPlayPart
  {
    QString        mTitle;
    QList<QString> mCompositionIdList;
  public:
    CsPlayPart();

    void jsonWrite( CsJsonWriter &writer ) const;

    void jsonRead( CsJsonReader &reader );

  };

using CsPlayPartList = QList<CsPlayPart>;

#endif // CSPLAYPART_H
