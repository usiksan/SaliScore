#include "CsDlgDefinition.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

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


  }

void CsDlgDefinition::fill()
  {

  }

void CsDlgDefinition::cmAppend()
  {

  }


void CsDlgDefinition::cmDelete()
  {

  }
