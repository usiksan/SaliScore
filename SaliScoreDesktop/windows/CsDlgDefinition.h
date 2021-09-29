#ifndef CSDLGDEFINITION_H
#define CSDLGDEFINITION_H

#include "CsConfig.h"
#include "score/CsDefinition.h"

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>

class CsDlgDefinition : public QDialog
  {
    Q_OBJECT

    QTableWidget *mDefTable;    //!< Visual table with parts list
    bool          mEditEnable;  //!< Internal flag to prevent recursive update visual table when edit it
    bool          mEditable;    //!< When true - dialog works in editable mode, i.e. enable to append, edit and delete parts
                                //!  when false - append, edit and delete are disabled
  public:
    CsDlgDefinition( bool editable, QWidget *parent = nullptr );

    void fill();
  public slots:
    void cmAppend();
    void cmDelete();
    void onCellClicked(int row, int column);
    void onItemChanged(QTableWidgetItem *item);
  protected:
    //!
    //! \brief defList Returns definition list
    //! \return        Definition list
    //!
    virtual CsDefList defList() const = 0;

    //!
    //! \brief defAppend   Append new definition
    //! \param title       Title (name) of definition
    //! \param description Description of definition
    //!
    virtual void      defAppend( const QString &title, const QString &description ) = 0;

    //!
    //! \brief defRename Rename definition
    //! \param index     Index of renamed definition
    //! \param title     New title (name) of definition
    //!
    virtual void      defRename( int index, const QString &title ) = 0;

    //!
    //! \brief defDelete Deletes definition
    //! \param index     Index of deleted definition
    //!
    virtual void      defDelete( int index ) = 0;

    //!
    //! \brief defToggle Toggle visibility of definition
    //! \param index     Index of toggled definition
    //! \return          New state of visibility
    //!
    virtual bool      defToggle( int index ) = 0;

  private:
    bool isNamePresent( const QString &name );

    void fillRow( int row, const CsDefinition &def );
  };

#endif // CSDLGDEFINITION_H
