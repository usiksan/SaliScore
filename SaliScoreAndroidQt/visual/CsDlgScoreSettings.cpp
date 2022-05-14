#include "CsDlgScoreSettings.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>

CsDlgScoreSettings::CsDlgScoreSettings(CsComposition &comp, QWidget *parent) :
  QDialog(parent),
  mComposition(comp)
  {
  setWindowTitle( tr("Score settings") );
  auto vbox = new QVBoxLayout();
  auto form = new QFormLayout();

  form->addRow( tr("Offset line start, in parts"), mLineOffset = new QLineEdit( QString::number( comp.lineStartOffset() / comp.tickPerPart() ) ) );
  form->addRow( tr("Numerator (count of part in takt)"), mNumerator = new QLineEdit( QString::number( comp.partPerTakt() ) ) );
  form->addRow( tr("Denominator (duration of part)"), mDenominator = new QLineEdit( QString::number( comp.denominator() ) ) );

  vbox->addLayout( form );
  auto dbox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
  vbox->addWidget( dbox );

  setLayout( vbox );
  connect( dbox, &QDialogButtonBox::accepted, this, &CsDlgScoreSettings::accept );
  connect( dbox, &QDialogButtonBox::rejected, this, &CsDlgScoreSettings::reject );
  }



void CsDlgScoreSettings::accept()
  {
  mComposition.denominatorSet( mDenominator->text().toInt() );
  mComposition.partPerTaktSet( qBound( 1, mNumerator->text().toInt(), 32 ) );
  mComposition.lineStartOffsetSet( qBound( 0, mLineOffset->text().toInt(), 32 ) * mComposition.tickPerPart() );

  QDialog::accept();
  }
