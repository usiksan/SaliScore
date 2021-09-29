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
    CsDlgDefRemark( bool editable, CsComposition &comp, QWidget *parent );

    // CsDlgDefinition interface
  protected:
    //!
    //! \brief defList Returns definition list
    //! \return        Definition list
    //!
    virtual CsDefList defList() const override;

    //!
    //! \brief defAppend   Append new definition
    //! \param title       Title (name) of definition
    //! \param description Description of definition
    //!
    virtual void      defAppend(const QString &title, const QString &description) override;

    //!
    //! \brief defRename Rename definition
    //! \param index     Index of renamed definition
    //! \param title     New title (name) of definition
    //!
    virtual void      defRename(int index, const QString &title) override;

    //!
    //! \brief defDelete Deletes definition
    //! \param index     Index of deleted definition
    //!
    virtual void      defDelete(int index) override;

    //!
    //! \brief defToggle Toggle visibility of definition
    //! \param index     Index of toggled definition
    //! \return          New state of visibility
    //!
    virtual bool      defToggle(int index) override;
  };

#endif // CSDLGDEFREMARK_H
