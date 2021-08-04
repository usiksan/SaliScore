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
#include "CsImportSaliScore.h"
#include "../../SvJson/SvJsonIO.h"

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

CsImportSaliScore::CsImportSaliScore() :
  CsImport( {QString(CS_BASE_EXTENSION)}, QObject::tr("SaliScore files") )
  {

  }




//!
//! \brief probe       Tests content to match to format
//! \param fileContent Content to test
//! \return            true if content match to format
//!
bool CsImportSaliScore::probe(const QByteArray &fileContent)
  {
  //Try read json
  QJsonObject obj = QJsonDocument::fromJson( fileContent ).object();
  return obj.value(QStringLiteral(CS_BASE_TYPE_KEY)).toString() == QStringLiteral(CS_BASE_TYPE);
  }




//!
//! \brief read        Perform import content to inner SaliScore format
//! \param fileContent Content to import
//! \param ok          true if import successfull
//! \return            Composition in SaliScore innter format
//!
CsComposition CsImportSaliScore::read(const QByteArray &fileContent, bool &ok)
  {
  QJsonObject obj = QJsonDocument::fromJson( fileContent ).object();
  CsComposition comp;
  if( obj.value(QStringLiteral(CS_BASE_TYPE_KEY)).toString() == QStringLiteral(CS_BASE_TYPE) ) {
    int version = obj.value(QStringLiteral(CS_BASE_VERSION_KEY)).toInt();
    SvJsonReaderExtInt js( obj, &version );
    comp.jsonRead( js );
    ok = true;
    }
  else ok = false;
  return comp;
  }
