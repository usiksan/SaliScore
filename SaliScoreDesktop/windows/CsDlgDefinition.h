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

    QTableWidget *mDefTable;
  public:
    CsDlgDefinition( QWidget *parent = nullptr );

    void fill();
  public slots:
    void cmAppend();
    void cmDelete();
    void onCellClicked(int row, int column);
    void onItemChanged(QTableWidgetItem *item);
  protected:
    virtual CsDefList defList() const = 0;

    virtual void      defAppend( const QString &title, const QString &description ) = 0;

    virtual void      defRename( int index, const QString &title ) = 0;

    virtual void      defDelet( int index ) = 0;

    virtual void      defToggle( int index, bool on ) = 0;
  };

#endif // CSDLGDEFINITION_H
