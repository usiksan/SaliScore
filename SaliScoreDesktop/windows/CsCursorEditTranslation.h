#ifndef CSCURSOREDITTRANSLATION_H
#define CSCURSOREDITTRANSLATION_H

#include "CsCursorEditString.h"

class CsCursorEditTranslation : public CsCursorEditString
  {
  public:
    CsCursorEditTranslation( int lineIndex, const QString &part, CsComposition &comp );

    // CsCursorEdit interface
  public:
    virtual void apply() override;
  };

#endif // CSCURSOREDITTRANSLATION_H
