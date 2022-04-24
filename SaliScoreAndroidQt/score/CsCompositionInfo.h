#ifndef CSCOMPOSITIONINFO_H
#define CSCOMPOSITIONINFO_H

#include "CsJsonIO.h"

#include <QString>

class CsComposition;

class CsCompositionInfo
  {
    QString mName;
    QString mAuthor;
    QString mSinger;
    QString mVersion;
  public:
    CsCompositionInfo();

    CsCompositionInfo( const CsComposition &comp );

    QString name() const { return mName; }

    QString author() const { return mAuthor; }

    QString singer() const { return mSinger; }

    QString version() const { return mVersion; }

    void jsonWrite( SvJsonWriter &js ) const;

    void jsonRead( SvJsonReader &js );
  };

using CsCompositionMap = QMap<QString,CsCompositionInfo>;


#endif // CSCOMPOSITIONINFO_H
