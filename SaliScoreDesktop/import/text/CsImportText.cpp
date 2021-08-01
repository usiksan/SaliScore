#include "CsImportText.h"

#include <QObject>

CsImportText::CsImportText() :
  CsImport( {QString(".txt"), QString(".htm"), QString(".html")}, QObject::tr("Simple text aligned") )
  {

  }




bool CsImportText::probe(const QByteArray &fileContent)
  {
  return true;
  }



CsComposition CsImportText::read(const QByteArray &fileContent, bool &ok)
  {
  //Convert to string
  QString fileString = QString::fromUtf8( fileContent );

  //Separate to line list
  QStringList lineList = fileString.split( QChar('\n') );

  //Scan all lines and classify them to remark, chord or lyric
  //Lyric lets nearly after chord
  bool wasChord = false;



  CsComposition comp;

  }
