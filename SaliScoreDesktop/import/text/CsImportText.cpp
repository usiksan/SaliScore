#include "CsImportText.h"

#include <QObject>

CsImportText::CsImportText() :
  CsImport( {QString(".txt"), QString(".htm"), QString(".html")}, QObject::tr("Simple text aligned") )
  {

  }


