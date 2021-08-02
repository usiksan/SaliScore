#include "CsDlgDefinition.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIcon>

CsDlgDefinition::CsDlgDefinition(QWidget *parent) :
  QDialog(parent)
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
    }

  connect( mDefTable,  &QTableWidget::cellClicked, this, &CsDlgDefinition::onCellClicked );
  connect( mDefTable,  &QTableWidget::itemChanged, this, &CsDlgDefinition::onItemChanged );
  }



void CsDlgDefinition::cmAppend()
  {

  }


void CsDlgDefinition::cmDelete()
  {

  }

void CsDlgDefinition::onCellClicked(int row, int column)
  {

  }

void CsDlgDefinition::onItemChanged(QTableWidgetItem *item)
  {

  }


