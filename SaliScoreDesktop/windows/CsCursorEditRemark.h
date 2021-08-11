#ifndef CSCURSOREDITREMARK_H
#define CSCURSOREDITREMARK_H

#include "CsCursorEditString.h"

class CsCursorEditRemark : public CsCursorEditString
  {
  public:
    CsCursorEditRemark( int lineIndex, const QString &part, CsComposition &comp );

    // CsCursorEdit interface
  public:
    virtual void apply() override;
  };

#endif // CSCURSOREDITREMARK_H
