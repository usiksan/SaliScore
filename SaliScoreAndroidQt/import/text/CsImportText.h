/*
Project "SaliScore Score music edit, view and tutorial programm"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  CsImportText perform import partiture from guitar chord-style text song ot saliScore format
*/
#ifndef CSIMPORTTEXT_H
#define CSIMPORTTEXT_H

#include "../CsImport.h"

#include <QMap>

class CsImportText : public CsImport
  {
    QMap<QString,int> mChordMap; //!< Map to conversion textual reprentation of chord to inner SaliScore format
  public:
    CsImportText();


    // CsImport interface
  public:
    virtual bool probe(const QByteArray &fileContent) override;
    virtual CsComposition read(const QByteArray &fileContent, bool &ok) override;

  private:
    bool chordParser( const QString name, int &note, int &chord ) const;

    bool chordProbe( const QString &line ) const;
  };

#endif // CSIMPORTTEXT_H
