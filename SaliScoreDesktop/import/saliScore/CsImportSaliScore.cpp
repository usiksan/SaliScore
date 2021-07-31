#include "CsImportSaliScore.h"
#include "../../SvJson/SvJsonIO.h"

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

CsImportSaliScore::CsImportSaliScore() :
  CsImport( {QString(CS_BASE_EXTENSION)}, QObject::tr("SaliScore files") )
  {

  }


bool CsImportSaliScore::probe(const QByteArray &fileContent)
  {
  //Try read json
  QJsonObject obj = QJsonDocument::fromJson( fileContent ).object();
  return obj.value(QStringLiteral(CS_BASE_TYPE_KEY)).toString() == QStringLiteral(CS_BASE_TYPE);
      //obj.value(QStringLiteral(CS_BASE_VERSION_KEY)).toInt() == CS_BASE_VERSION;
  }



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
