#ifndef CSDLGDEFREMARK_H
#define CSDLGDEFREMARK_H

#include "CsConfig.h"
#include "score/CsComposition.h"

#include "CsDlgDefinition.h"

class CsDlgDefRemark : public CsDlgDefinition
  {
    Q_OBJECT

    CsComposition &mComposition;
  public:
    CsDlgDefRemark( CsComposition &comp, QWidget *parent );

    // CsDlgDefinition interface
  protected:
    virtual CsDefList defList() const override;
    virtual void defAppend(const QString &title, const QString &description) override;
    virtual void defRename(int index, const QString &title) override;
    virtual void defDelet(int index) override;
    virtual void defToggle(int index, bool on) override;
  };

#endif // CSDLGDEFREMARK_H
