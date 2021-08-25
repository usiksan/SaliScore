

#include "CsDlgRegistration.h"
#include "repo/CsRepoClient.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSettings>

CsDlgRegistration::CsDlgRegistration(QWidget *parent) :
  QDialog(parent)
  {
  QSettings s;

  QVBoxLayout *vbox = new QVBoxLayout();
  vbox->addWidget( new QLabel(tr("Please come up with a nickname and password\n"
                                 "for yourself. This is necessary for syncing your devices. \n"
                                 "The email address is needed to be able to restore the password \n"
                                 "if necessary. You don't have to enter your email address. \n\n"
                                 "You can enter an existing nickname and password, in this case, \n"
                                 "your devices will automatically sync.")) );
  QFormLayout *form = new QFormLayout();
  form->addRow( tr("Your nickname:"), mAuthor = new QLineEdit( s.value(KEY_AUTHOR).toString() )   );
  form->addRow( tr("Password:"),      mPassword = new QLineEdit( s.value(KEY_PASSWORD).toString() )  );
  form->addRow( tr("E-Mail:"),        mEmail = new QLineEdit() );
  form->addRow( tr("Status:"),        mStatus = new QLabel( repoClient->isRegistered() ? tr("Registered") : tr("Not registered") ) );

  QPushButton *reg = new QPushButton( tr("Register") );
  form->addRow( tr(""), reg );

  QPushButton *exit = new QPushButton( tr("Exit") );
  form->addRow( exit, mClose = new QPushButton( tr("Close") ) );

  mAuthor->setToolTip( tr("Nickname - letters and digits 32 chars max") );
  mAuthor->setInputMask( QStringLiteral("nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn;") );
  mPassword->setToolTip( tr("Password - any symbols 16 chars max") );
  vbox->addLayout( form );

  connect( reg, &QPushButton::clicked, this, [this] () {
    emit registration( QStringLiteral(CS_DEFAULT_REPO), mAuthor->text().replace( QChar(' '), QChar('_') ), mPassword->text(), mEmail->text() );
    });

  connect( exit, &QPushButton::clicked, this, &CsDlgRegistration::close );
  connect( mClose, &QPushButton::clicked, this, &CsDlgRegistration::accept );

  mClose->setEnabled( repoClient->isRegistered() );

  setLayout( vbox );


  connect( this, &CsDlgRegistration::registration, repoClient, &CsRepoClient::doRegister );
  connect( repoClient, &CsRepoClient::registerStatus, this, &CsDlgRegistration::registrationProcess );
  }



void CsDlgRegistration::registrationProcess(bool finish, const QString res)
  {
  if( finish && res.isEmpty() ) {
    mStatus->setText( tr("Registered") );
    mClose->setEnabled( true );
    return;
    }
  mStatus->setText( res );
  }
