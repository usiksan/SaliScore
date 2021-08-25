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




CsRepoClient::CsRepoClient(CsPlayList &playList, QObject *parent) :
  QObject(parent),
  mPlayList(playList)
  {

  mNetworkManager = new QNetworkAccessManager(this);

  connect( mNetworkManager, &QNetworkAccessManager::finished, this, &CsRepoClient::finished );
  //connect( &mTimer, &QTimer::timeout, this, &SdObjectNetClient::doSync );
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




void CsRepoClient::finished(QNetworkReply *reply)
  {
  if( reply->error() ) {
    //Error when transfer
    CsRepoQueryType queryType = mQueryType;
    mQueryType = cpqIdle;
    switch( queryType ) {
      case cpqRegister :
        cmRegister( QJsonObject{} );
        emit registerStatus( true, tr("Error when transfer: \"%1\"").arg(reply->errorString()) );
        break;
//      case SdRemoteQueryList :
//      case SdRemoteQueryNone :
//      case SdRemoteQueryUploadObject :
//      case SdRemoteQueryDownloadObject :
//        qDebug() << "finished Error when transfer " << reply->errorString();
//        emit informationAppended( tr("Error when transfer: \"%1\"").arg(reply->errorString()) );
//        break;
      }
    }
  else {
    //Transfer completed successfully
    //Inside reply - JSON with answer
    QJsonObject obj = QJsonDocument::fromJson( reply->readAll() ).object();
    CsRepoQueryType queryType = mQueryType;
    mQueryType = cpqIdle;
    switch( queryType ) {
      case cpqRegister :
        cmRegister( obj );
        break;
//      case SdRemoteQueryList :
//        cmSyncList( obj );
//        break;
//      case SdRemoteQueryDownloadObject :
//        cmDownloadObject( obj );
//        break;
//      case SdRemoteQueryUploadObject :
//        cmUploadObject( obj );
//        break;
//      case SdRemoteQueryNone :
//        break;
      }
    }
  reply->deleteLater();
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
//!
void CsRepoClient::doRegister(const QString repo, const QString authorName, const QString password, const QString email)
  {
  mTimer.stop();

  QString fullRepo;
  if( repo.endsWith(QChar('/')) )
    fullRepo = repo;
  else
    fullRepo = repo + QStringLiteral("/");

  QSettings s;
  s.setValue( KEY_WEB_REPO, fullRepo );
  s.setValue( KEY_AUTHOR, authorName );
  s.setValue( KEY_PASSWORD, password );

  QString mail = email.isEmpty() ? QStringLiteral("noEmail") : email;

  //Prepare block for transmition
  QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_AUTHOR, authorName.toUtf8() );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PASSWORD, password.toUtf8() );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_EMAIL, mail.toUtf8() );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_LISTTIME, QByteArray::number( mPlayList.version() ) );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PLAYLIST, mPlayList.toByteArray() );

  emit registerStatus( false, tr("Register start...") );
  mQueryType = cpqRegister;
  QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + fullRepo + QStringLiteral("register.php"))), multiPart );
  multiPart->setParent(reply); // delete the multiPart with the reply
  }





void CsRepoClient::cmRegister(const QJsonObject &reply)
  {
  bool success = false;
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 3 - не смог вставить новое имя в базу
  // 4 - имя уже есть в базе и пароль не совпал
  if( reply.contains(REPO_FIELD_RESULT) ) {
    int     result = reply.value( REPO_FIELD_RESULT ).toInt();
    switch( result ) {
      case 0 :
        success = true;
        emit registerStatus( true, QString{} );
        break;
      case 1 :
        emit registerStatus( true, tr("Internal data. Check repo address") );
        break;
      case 2 :
        emit registerStatus( true, tr("Failure data base connection. Try later") );
        break;
      case 3 :
        emit registerStatus( true, tr("Can't insert this nickname into data base. Try another nickname") );
        break;
      case 4 :
        emit registerStatus( true, tr("This nickname already present in data base with different password. Try another nickname or restore password.") );
        break;
      default:
        emit registerStatus( true, tr("Unresponsible code %1. Check repo address").arg(result) );
        break;
      }
    }
  else {
    emit registerStatus( true, tr("Unresponsible reply. Check repo address") );
    }

  if( !success ) {
    QSettings s;
    s.remove( QStringLiteral(KEY_WEB_REPO) );
    s.remove( QStringLiteral(KEY_AUTHOR) );
    s.remove( QStringLiteral(KEY_PASSWORD) );
    }
  }


