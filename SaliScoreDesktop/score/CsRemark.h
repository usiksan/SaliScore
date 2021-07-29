#ifndef CSREMARK_H
#define CSREMARK_H

#include "CsConfig.h"

#include <QString>
#include <QMap>


class SvJsonWriter;
class SvJsonReader;

class CsRemark
  {
    QMap<QString,QString> mStringMap;
  public:
    CsRemark();

    bool isEmpty() const { return mStringMap.isEmpty(); }

    void jsonWrite( SvJsonWriter &js ) const;

    void jsonRead( SvJsonReader &js );
  };

#endif // CSREMARK_H
