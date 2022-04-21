#ifndef CSCURSOREDITPROPERTY_H
#define CSCURSOREDITPROPERTY_H

#include "CsCursorEditString.h"

class CsCursorEditProperty : public CsCursorEditString
  {
  public:
    CsCursorEditProperty( const QString &partName, CsComposition &comp );

    // CsCursorEdit interface
  public:
    virtual void apply() override;
  };

#endif // CSCURSOREDITPROPERTY_H
