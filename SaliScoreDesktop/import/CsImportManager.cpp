#include "CsImportManager.h"

#include <QObject>
#include <QFile>
#include <QFileInfo>

CsImportManager::CsImportManager()
  {

  }

CsImportManager::~CsImportManager()
  {
  qDeleteAll(mImporterList);
  }

static QString formatExtensions( QSet<QString> extensions )
  {
  QString res(" (" );
  bool notFirst = false;
  for( const auto &str : extensions ) {
    if( notFirst )
      res += QStringLiteral(" ");
    notFirst = true;
    res += QChar('*') + str;
    }
  return res + QStringLiteral(")");
  }




QString CsImportManager::formats() const
  {
  //List of all extensions
  QSet<QString> allExtensions;
  for( auto imp : mImporterList )
    allExtensions.unite( imp->extensions() );
  QString res( QObject::tr("All SaliScore files") + formatExtensions( allExtensions ) );

  //Append concrete import extensions
  for( auto imp : mImporterList ) {
    res += QStringLiteral(";;") + imp->filesTitle() + formatExtensions( imp->extensions() );
    }

  return res;
  }




//!
//! \brief read Do import from data
//! \param data Source data to import
//! \param ok   true if import successfull or false in other hand
//! \return     Imported composition
//!
CsComposition CsImportManager::read(const QByteArray &data, bool &ok)
  {
  //Find applied importer
  for( auto imp : qAsConst(mImporterList) )
    if( imp->probe(data) )
      //Importer found. Perform import
      return imp->read( data, ok );
  ok = false;
  return CsComposition{};
  }




//!
//! \brief readFile Do import from file
//! \param path     Path to source file
//! \param ok       true if import successfull or false in other hand
//! \return         Imported composition
//!
CsComposition CsImportManager::readFile(const QString path, bool &ok)
  {
  //File extension in low register
  QString ext( QChar('.') + QFileInfo(path).suffix().toLower() );
  //File
  QFile file(path);
  if( file.open( QIODevice::ReadOnly ) ) {
    QByteArray content( file.readAll() );
    //Test importer with extension
    for( auto imp : qAsConst(mImporterList) )
      if( imp->isMatchExtension( ext ) ) {
        if( imp->probe(content) )
          return imp->read( content, ok );
        }
    }

  ok = false;
  return CsComposition{};
  }
