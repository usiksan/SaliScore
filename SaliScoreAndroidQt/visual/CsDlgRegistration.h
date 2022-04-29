/*
Project "SaliScore Score music edit, view and tutorial program"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for user registration and re-registration
*/
#ifndef CSDLGREGISTRATION_H
#define CSDLGREGISTRATION_H

#include "CsConfig.h"

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class CsDlgRegistration : public QDialog
  {
    Q_OBJECT

    QLineEdit   *mAuthor;   //!< Field for author nik
    QLineEdit   *mPassword; //!< Field for author password
    QLineEdit   *mEmail;    //!< Field for e-mail

    QLabel      *mStatus;   //!< Registration status

    QPushButton *mClose;    //!< Close button
  public:
    CsDlgRegistration( QWidget *parent );

  signals:
    void registration( const QString repo, const QString author, const QString password, const QString email );

  public slots:
    void registrationProcess(bool finish, const QString res );
  };

#endif // CSDLGREGISTRATION_H
