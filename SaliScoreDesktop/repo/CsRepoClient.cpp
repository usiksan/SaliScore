#include "CsRepoClient.h"

#include <QThread>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpPart>
#include <QHttpMultiPart>
#include <QHostAddress>
#include <QSettings>
#include <QJsonDocument>


#define REPO_FIELD_AUTHOR        QStringLiteral("author")
#define REPO_FIELD_PASSWORD      QStringLiteral("password")
#define REPO_FIELD_EMAIL         QStringLiteral("email")
#define REPO_FIELD_RESULT        QStringLiteral("result")
#define REPO_FIELD_LISTTIME      QStringLiteral("listtime")
#define REPO_FIELD_PLAYLIST      QStringLiteral("playlist")

#define REPO_FIELD_COMPOSITIONID QStringLiteral("compositionid")
#define REPO_FIELD_VERSION       QStringLiteral("version")
#define REPO_FIELD_SINGER        QStringLiteral("singer")
#define REPO_FIELD_TITLE         QStringLiteral("title")
#define REPO_FIELD_ISPUBLIC      QStringLiteral("ispublic")
#define REPO_FIELD_MELODYPRESENT QStringLiteral("melodypresent")
#define REPO_FIELD_SONG          QStringLiteral("song")



//Main object for remote database communication
CsRepoClient *repoClient;




static void sdHttpMultiPartAppendField( QHttpMultiPart *multiPart, const QString fieldName, const QByteArray data )
  {
  QHttpPart textPart;
  textPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
  textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant( QStringLiteral("form-data; name=\"%1\"").arg(fieldName)) );
  textPart.setBody(data);

  multiPart->append( textPart );
  }


CsRepoClient::CsRepoClient(QObject *parent) :
  QObject(parent)
  {

  }

bool CsRepoClient::isRegistered() const
  {
  QSettings s;
  QString  author          = s.value( KEY_AUTHOR ).toString();
  QString  passw           = s.value( KEY_PASSWORD ).toString();
  QString  webRepo        = s.value( KEY_WEB_REPO ).toString();
  return !webRepo.isEmpty() && !author.isEmpty() && !passw.isEmpty();
  }



QString CsRepoClient::author() const
  {
  QSettings s;
  return s.value( KEY_AUTHOR ).toString();
  }





//!
//! \brief doRegister Begin registration process or check registration status
//!                   If this author with this password already registered or
//!                   this author not registered then reply OK in other - duplicate
//!                   author detected and must be enter another author name
//! \param repo       Repository address, repo.salicad.com by default
//! \param authorName Author name for registration
//! \param password   Password for this author name
//! \param email      E-mail for password restore
//! \param listtime   Time of last editing of playList
//! \param playlist   PlayList itself
//!
void CsRepoClient::doRegister(const QString repo, const QString authorName, const QString password, const QString email, int listtime, QByteArray playlist)
  {
  mTimer.stop();

  QString fullRepo;
  if( repo.endsWith(QChar('/')) )
    fullRepo = repo;
  else
    fullRepo = repo + QStringLiteral("/");

  QSettings s;
  s.setValue( KEY_WEB_REPO, fullRepo );

  //Prepare block for transmition
  QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_AUTHOR, authorName.toUtf8() );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PASSWORD, password.toUtf8() );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_EMAIL, email.toUtf8() );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_LISTTIME, QByteArray::number( listtime ) );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PLAYLIST, playlist );

  emit registerStatus( tr("Register start..."), email );
  //mQueryType = SdRemoteQueryRegister;
  QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + fullRepo + QStringLiteral("register.php"))), multiPart );
  multiPart->setParent(reply); // delete the multiPart with the reply
  }
