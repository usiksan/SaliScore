#ifndef CSREMARK_H
#define CSREMARK_H

#include "CsConfig.h"
#include "CsJsonIO.h"

#include <QString>
#include <QMap>

class CsPainter;

class CsRemark
  {
    QMap<QString,QString> mRemarkMap;
  public:
    CsRemark();
    CsRemark( const QString &lang, const QString &rem );

    auto &remarkMapConst() const { return mRemarkMap; }

    QString remarkGet( const QString &lang ) const { return mRemarkMap.value(lang); }

    void    remarkSet( const QString &lang, const QString &rem ) { mRemarkMap.insert( lang, rem ); }

    void    remarkRemove( const QString &lang ) { mRemarkMap.remove( lang ); }

    void    remarkRename( const QString &prevLang, const QString &newLang );

    bool isEmpty() const { return mRemarkMap.isEmpty(); }

    void jsonWrite( CsJsonWriter &js ) const;

    void jsonRead( CsJsonReader &js );
  };

#endif // CSREMARK_H
