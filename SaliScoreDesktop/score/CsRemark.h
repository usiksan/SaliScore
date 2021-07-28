#ifndef CSREMARK_H
#define CSREMARK_H

#include "CsConfig.h"

#include <QString>
#include <QMap>



class CsRemark
  {
    QMap<QString,QString> mStringMap;
  public:
    CsRemark();

    bool isEmpty() const { return mStringMap.isEmpty(); }
  };

#endif // CSREMARK_H
