#include "CsDlgDefinition.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QIcon>

CsDlgDefinition::CsDlgDefinition(QWidget *parent) :
  QDialog(parent),
  mEditEnable(true)
  {
  mDefTable = new QTableWidget();
  QPushButton *butAppend = new QPushButton(tr("Append"));
  QPushButton *butDelete = new QPushButton(tr("Delete"));
  QPushButton *butClose  = new QPushButton(tr("Close"));

  QVBoxLayout *vbox = new QVBoxLayout();
  vbox->addWidget( butAppend );
  vbox->addWidget( butDelete );
  vbox->addStretch(5);
  vbox->addWidget( butClose );

  QHBoxLayout *hbox = new QHBoxLayout();
  hbox->addWidget( mDefTable );
  hbox->addLayout( vbox );

  setLayout(hbox);

  connect( butAppend, &QPushButton::clicked, this, &CsDlgDefinition::cmAppend );
  connect( butDelete, &QPushButton::clicked, this, &CsDlgDefinition::cmDelete );
  connect( butClose,  &QPushButton::clicked, this, &CsDlgDefinition::close );

  mDefTable->setColumnCount(3);
  mDefTable->setColumnWidth( 0, 100 );
  mDefTable->setColumnWidth( 1, 50 );
  mDefTable->setColumnWidth( 2, 200 );

  //Set column header
  QStringList header;
  header << tr("Name") << tr("Visible") << tr("Description");// << tr("Color") << tr("Pair name");
  mDefTable->setHorizontalHeaderLabels( header );

  //Hide row header
  mDefTable->verticalHeader()->hide();

  }



void CsDlgDefinition::fill()
  {
  CsDefList list = defList();
  mDefTable->setRowCount( list.count() );
  int row = 0;
  for( const auto &def : list ) {
    //Name of definition
    fillRow( row++, def );
    }

  connect( mDefTable,  &QTableWidget::cellClicked, this, &CsDlgDefinition::onCellClicked );
  connect( mDefTable,  &QTableWidget::itemChanged, this, &CsDlgDefinition::onItemChanged );
  }



void CsDlgDefinition::cmAppend()
  {
  static int index = 0;
  if( mEditEnable ) {
    //Create name
    QString name;
    if( mDefTable->rowCount() ) {
      int lastIndex = mDefTable->rowCount() - 1;
      name = mDefTable->item( lastIndex, 0 )->text();
      while( isNamePresent( name + QStringLiteral("(%1)").arg(++index)) );
      name.append( QStringLiteral("(%1)").arg(index) );
      }
    else {
      //There no definition in table, create with "def" name
      name = tr("default");
      }
    int row = mDefTable->rowCount();
    mDefTable->insertRow( row );
    defAppend( name, QString{} );
    fillRow( row, defList().at(row) );
    if( parentWidget() != nullptr )
      parentWidget()->update();
    }
  }



void CsDlgDefinition::cmDelete()
  {
  if( mEditEnable ) {
    int row = mDefTable->currentRow();
    if( row >= 0 ) {
      defDelete( row );
      mDefTable->removeRow( row );
      if( parentWidget() != nullptr )
        parentWidget()->update();
      }
    }
  }



void CsDlgDefinition::onCellClicked(int row, int column)
  {
  if( column == 1 ) {
    //Visibility changed
    bool vis = defToggle( row );
    mEditEnable = false;
    QTableWidgetItem *item = mDefTable->item( row, 1 );
    item->setIcon( QIcon( vis ? QStringLiteral(":/pic/lampEnable.png") : QStringLiteral(":/pic/lampDisable.png")) );
    mEditEnable = true;
    if( parentWidget() != nullptr )
      parentWidget()->update();
    }
  }



void CsDlgDefinition::onItemChanged(QTableWidgetItem *item)
  {
  if( mEditEnable && item != nullptr ) {
    int column = mDefTable->column(item);
    int row = mDefTable->row(item);
    if( column == 0 ) {
      //Name changed
      //Check if name is available
      if( isNamePresent(item->text()) ) {
        //Name already present, revert to previous name
        QMessageBox::warning( this, tr("Error!"), tr("Name '%1' already exist. Enter another name.") );
        mEditEnable = false;
        item->setText( defList().at(row).mName );
        mEditEnable = true;
        }
      else {
        //Name is correct - change
        defRename( row, item->text() );
        if( parentWidget() != nullptr )
          parentWidget()->update();
        }
      }

    else if( column == 2 ) {
      //Description changed - simple append
      defAppend( mDefTable->item( row, 0 )->text(), item->text() );
      if( parentWidget() != nullptr )
        parentWidget()->update();
      }

    }
  }



bool CsDlgDefinition::isNamePresent(const QString &name)
  {
  CsDefList list = defList();
  for( auto const &def : list )
    if( def.mName == name )
      return true;
  return false;
  }



void CsDlgDefinition::fillRow(int row, const CsDefinition &def)
  {
  mEditEnable = false;

  mDefTable->setRowHeight( row, 25 );

  QTableWidgetItem *item = new QTableWidgetItem(def.mName);
  item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsEditable );
  mDefTable->setItem( row, 0, item );

  //Visibility
  item = new QTableWidgetItem();
  item->setIcon( QIcon( def.mVisible ? QStringLiteral(":/pic/lampEnable.png") : QStringLiteral(":/pic/lampDisable.png")) );
  item->setFlags( Qt::ItemIsEnabled );
  mDefTable->setItem( row, 1, item );

  //Description
  item = new QTableWidgetItem(def.mDescription);
  item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsEditable );
  mDefTable->setItem( row, 2, item );

  mEditEnable = true;
  }


