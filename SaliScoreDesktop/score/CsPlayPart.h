#ifndef CSPLAYPART_H
#define CSPLAYPART_H

#include "SvJson/SvJsonIO.h"

#include <QString>
#include <QList>


class CsPlayPart
  {
    QString        mTitle;
    QList<QString> mCompositionList;
  public:
    CsPlayPart();

    void jsonWrite( SvJsonWriter &writer ) const;

    void jsonRead( SvJsonReaderExtInt &reader );

  };

using CsPlayPartList = QList<CsPlayPart>;

#endif // CSPLAYPART_H
