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

    void  remarkAppend( const QString &lang );

    void  remarkRemove( const QString &lang );

    void  lineInsert( int index, const CsLine &line );

    void  lineAppend( const CsLine &line ) { lineInsert( -1, line ); }

    auto &lineList() const { return mLineList; }

    void  jsonWrite( CsJsonWriter &js ) const;

    void  jsonRead( CsJsonReader &js );
  };

#endif // CSCOMPOSITION_H
