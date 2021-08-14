#ifndef CSPLAYCOMPOSITION_H
#define CSPLAYCOMPOSITION_H

#include "SvJson/SvJsonIO.h"

#include <QString>
#include <QList>

class CsPlayComposition
  {
    QString mId;
    QString mSinger;
    QString mTitle;
    QString mAuthor;
    int     mVersion;
    bool    mIsPublic;
    bool    mMelodyPresent;
  public:
    CsPlayComposition();

    void jsonWrite( SvJsonWriter &writer ) const;

    void jsonRead( SvJsonReaderExtInt &reader );
  };


using CsPlayCompositionList = QList<CsPlayComposition>;

#endif // CSPLAYCOMPOSITION_H
