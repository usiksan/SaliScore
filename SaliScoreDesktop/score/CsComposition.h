#ifndef CSCOMPOSITION_H
#define CSCOMPOSITION_H

#include "CsConfig.h"
#include "CsLine.h"

#include <QJsonObject>

class CsComposition
  {
    QString    mTitle;
    QString    mSinger;
    QString    mComposer;
    QString    mLyricist;
    CsLineList mLineList;
  public:
    CsComposition();

    QJsonObject write() const;

    void        read( const QJsonObject obj );
  };

#endif // CSCOMPOSITION_H
