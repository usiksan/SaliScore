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
#include "SvLib/SvJsonIO.h"

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
  CsJsonAttr attr(true);

  QJsonObject obj( svJsonObjectFromByteArray(fileContent) );

  SvJsonReader js( obj );

  //Check object type and scan version
  attr.jsonRead( js );
  return attr.isComposition();
  }




//!
//! \brief read        Perform import content to inner SaliScore format
//! \param fileContent Content to import
//! \param ok          true if import successfull
//! \return            Composition in SaliScore innter format
//!
CsComposition CsImportSaliScore::read(const QByteArray &fileContent, bool &ok)
  {
  CsComposition comp;
  ok = comp.fromByteArray( fileContent, true );
  return comp;
  }
