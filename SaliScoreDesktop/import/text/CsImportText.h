#ifndef CSIMPORTTEXT_H
#define CSIMPORTTEXT_H

#include "../CsImport.h"

class CsImportText : public CsImport
  {
  public:
    CsImportText();


    // CsImport interface
  public:
    virtual bool probe(const QByteArray &fileContent) override;
    virtual CsComposition read(const QByteArray &fileContent, bool &ok) override;
  };

#endif // CSIMPORTTEXT_H
