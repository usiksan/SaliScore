#ifndef CSIMPORTMANAGER_H
#define CSIMPORTMANAGER_H

#include "CsConfig.h"
#include "CsImport.h"

#include <QList>

class CsImportManager
  {
    QList<CsImportPtr> mImporterList; //!< List of importer
  public:
    CsImportManager();
    ~CsImportManager();

    //!
    //! \brief registerImport Registers new importer to the importer list
    //! \param importPtr      Importer to register
    //!
    void          registerImport( CsImportPtr importPtr ) { mImporterList.append( importPtr ); }

    //!
    //! \brief formats Return formats string to use in load dialog
    //! \return        Formats string to use in load dialog
    //!
    QString       formats() const;

    //!
    //! \brief read Do import from data
    //! \param data Source data to import
    //! \param ok   true if import successfull or false in other hand
    //! \return     Imported composition
    //!
    CsComposition read( const QByteArray &data, bool &ok );

    //!
    //! \brief readFile Do import from file
    //! \param path     Path to source file
    //! \param ok       true if import successfull or false in other hand
    //! \return         Imported composition
    //!
    CsComposition readFile( const QString path, bool &ok );
  };

#endif // CSIMPORTMANAGER_H
