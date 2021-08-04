/*
Project "SaliScore Score music edit, view and tutorial programm"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  CsImportSaliScore reads SaliScore file format
*/
#ifndef CSIMPORTSALISCORE_H
#define CSIMPORTSALISCORE_H

#include "../CsImport.h"

class CsImportSaliScore : public CsImport
  {
  public:
    CsImportSaliScore();

    // CsImport interface
  public:

    //!
    //! \brief probe       Tests content to match to format
    //! \param fileContent Content to test
    //! \return            true if content match to format
    //!
    virtual bool          probe(const QByteArray &fileContent) override;

    //!
    //! \brief read        Perform import content to inner SaliScore format
    //! \param fileContent Content to import
    //! \param ok          true if import successfull
    //! \return            Composition in SaliScore innter format
    //!
    virtual CsComposition read(const QByteArray &fileContent, bool &ok) override;
  };

#endif // CSIMPORTSALISCORE_H
