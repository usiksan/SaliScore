/*
Project "SaliScore Score music edit, view and tutorial programm"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  CsImport base class to import into SaliScore format from wide range formats
*/
#ifndef CSIMPORT_H
#define CSIMPORT_H

#include "CsConfig.h"
#include "score/CsComposition.h"

#include <QByteArray>
#include <QSet>

class CsImport
  {
    QSet<QString> mExtensions; //!< Set of file extensions of import format
    QString       mFilesTitle; //!< Title for file format
  public:
    CsImport(std::initializer_list<QString> list, const QString &filesTitle );
    virtual ~CsImport() {}

    //!
    //! \brief isMatchExtension Tests if file extension match to any of file extensions for the format
    //! \param ext              Tested file extension
    //! \return                 true if file extension match to any of file extensions for the format
    //!
    bool                  isMatchExtension( const QString ext ) const { return mExtensions.contains(ext); }

    //!
    //! \brief extensions Returns set extensions of file format
    //! \return           Set extensions of file format
    //!
    QSet<QString>         extensions() const { return mExtensions; }

    //!
    //! \brief filesTitle Returns title of format
    //! \return           Title of format
    //!
    QString               filesTitle() const { return mFilesTitle; }

    //!
    //! \brief probe       Tests content to match to format
    //! \param fileContent Content to test
    //! \return            true if content match to format
    //!
    virtual bool          probe( const QByteArray &fileContent ) = 0;

    //!
    //! \brief read        Perform import content to inner SaliScore format
    //! \param fileContent Content to import
    //! \param ok          true if import successfull
    //! \return            Composition in SaliScore innter format
    //!
    virtual CsComposition read( const QByteArray &fileContent, bool &ok ) = 0;
  };


using CsImportPtr = CsImport*;

#endif // CSIMPORT_H
