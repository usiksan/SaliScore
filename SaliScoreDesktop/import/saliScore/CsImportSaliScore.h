#ifndef CSIMPORTSALISCORE_H
#define CSIMPORTSALISCORE_H

#include "../CsImport.h"

class CsImportSaliScore : public CsImport
  {
  public:
    CsImportSaliScore();

    // CsImport interface
  public:
    virtual bool          probe(const QByteArray &fileContent) override;
    virtual CsComposition read(const QByteArray &fileContent, bool &ok) override;
  };

#endif // CSIMPORTSALISCORE_H
