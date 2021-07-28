#ifndef CSIMPORT_H
#define CSIMPORT_H

#include "CsConfig.h"
#include "score/CsComposition.h"

#include <QByteArray>
#include <QSet>

class CsImport
  {
    QSet<QString> mExtensions;
    QString       mFilesTitle;
  public:
    CsImport(std::initializer_list<QString> list, const QString &filesTitle );

    bool                  isMatchExtension( const QString ext ) const { return mExtensions.contains(ext); }

    QSet<QString>         extensions() const { return mExtensions; }

    QString               filesTitle() const { return mFilesTitle; }

    virtual bool          probe( const QByteArray &fileContent ) = 0;

    virtual CsComposition read( const QByteArray &fileContent, bool &ok ) = 0;
  };


using CsImportPtr = CsImport*;

#endif // CSIMPORT_H
