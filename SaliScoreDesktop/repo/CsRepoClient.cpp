#include "CsRepoClient.h"

#include <QThread>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpPart>
#include <QHttpMultiPart>
#include <QHostAddress>
#include <QSettings>
#include <QJsonDocument>
#include <QFile>


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

#define REPO_FIELD_LASTTIME      QStringLiteral("lasttime")


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


static void sdHttpMultiPartAppendField( QHttpMultiPart *multiPart, const QString fieldName, int val )
  {
  sdHttpMultiPartAppendField( multiPart, fieldName, QByteArray::number(val) );
  }




CsRepoClient::CsRepoClient(CsPlayList &playList, QObject *parent) :
  QObject(parent),
  mPlayList(playList)
  {

  mNetworkManager = new QNetworkAccessManager(this);

  mTimer.setInterval( 5000 );
  mTimer.start();

  connect( mNetworkManager, &QNetworkAccessManager::finished, this, &CsRepoClient::finished );
  connect( &mTimer, &QTimer::timeout, this, &CsRepoClient::doSync );
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
      default:
        qDebug() << "finished Error when transfer " << reply->errorString();
//        emit informationAppended( tr("Error when transfer: \"%1\"").arg(reply->errorString()) );
        break;
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
      case cpqList :
        cmList( obj );
        break;
      case cpqDownloadList :
        cmDownloadPlayList( obj );
        break;
      case cpqUploadList :
        cmUploadPlayList( obj );
        break;
      case cpqDownloadSong :
        cmDownloadSong( obj );
        break;
      case cpqUploadSong :
        cmUploadSong( obj );
        break;
      case cpqIdle :
        break;
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
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_LISTTIME, mPlayList.version() );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PLAYLIST, mPlayList.toByteArray() );

  emit registerStatus( false, tr("Register start...") );
  mQueryType = cpqRegister;
  QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + fullRepo + QStringLiteral("register.php"))), multiPart );
  multiPart->setParent(reply); // delete the multiPart with the reply
  }




//!
//! \brief doSync Performs syncronization with remote repository
//!
void CsRepoClient::doSync()
  {
  if( mQueryType == cpqIdle && isRegistered() ) {
    QSettings s;
    QString author         = s.value( KEY_AUTHOR ).toString();
    QString password       = s.value( KEY_PASSWORD ).toString();
    int     remoteSyncTime = s.value( KEY_REMOTE_SYNC ).toInt();
    QString hostRepo       = s.value( KEY_WEB_REPO ).toString();
    if( remoteSyncTime == 0 ) remoteSyncTime = 1;
    qDebug() << "doSync remoteSyncIndex" << hostRepo << remoteSyncTime;
      //infoAppend( tr("Do sync...") );
      //Prepare block for transmition
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_AUTHOR, author.toUtf8() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PASSWORD, password.toUtf8() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_LASTTIME, remoteSyncTime );

    mQueryType = cpqList;
    QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("list.php"))), multiPart );
    multiPart->setParent(reply); // delete the multiPart with the reply
    }
  }



void CsRepoClient::doDownloadPlayList()
  {
  qDebug() << "Download play list";
  QSettings s;
  QString author         = s.value( KEY_AUTHOR ).toString();
  QString password       = s.value( KEY_PASSWORD ).toString();
  QString hostRepo       = s.value( KEY_WEB_REPO ).toString();

  QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_AUTHOR, author.toUtf8() );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PASSWORD, password.toUtf8() );

  mQueryType = cpqDownloadList;
  QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("downloadlist.php"))), multiPart );
  multiPart->setParent(reply); // delete the multiPart with the reply
  }




void CsRepoClient::doUploadPlayList()
  {
  qDebug() << "Upload play list";
  QSettings s;
  QString author         = s.value( KEY_AUTHOR ).toString();
  QString password       = s.value( KEY_PASSWORD ).toString();
  QString hostRepo       = s.value( KEY_WEB_REPO ).toString();

  QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_AUTHOR, author.toUtf8() );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PASSWORD, password.toUtf8() );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_LISTTIME, mPlayList.version() );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PLAYLIST, mPlayList.toByteArray() );

  mQueryType = cpqUploadList;
  QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("uploadlist.php"))), multiPart );
  multiPart->setParent(reply); // delete the multiPart with the reply
  }




void CsRepoClient::doDownloadSong()
  {
  while( mUpdateList.count() ) {
    QString compositionid = earlyCompositionId();
    QJsonObject obj = mUpdateList.value( compositionid ).toObject();
    int remoteVersion = obj.value( QStringLiteral("version") ).toInt();
    if( obj.value( QStringLiteral("author") ).toString() == author() ) {
      //Song of this author
      if( remoteVersion > mPlayList.composition(compositionid).versionFromFile() ) {
        //Need download local song
        doDownloadSong( compositionid );
        return;
        }
      }
    else if( obj.value( QStringLiteral("ispublic") ).toBool() ) {
      //Public song of other author

      //Update song in full song list

      if( mPlayList.contains(compositionid) && remoteVersion > mPlayList.composition(compositionid).versionFromFile() ) {
        //Need download global song
        doDownloadSong( compositionid );
        return;
        }
      }
    mUpdateList.remove( compositionid );

    //Update removed song version
    QSettings s;
    s.setValue( KEY_REMOTE_SYNC, remoteVersion );
    }

  doUploadSong();
  }




void CsRepoClient::doDownloadSong(const QString compositionid)
  {
  QSettings s;
  QString author         = s.value( KEY_AUTHOR ).toString();
  QString password       = s.value( KEY_PASSWORD ).toString();
  QString hostRepo       = s.value( KEY_WEB_REPO ).toString();

  QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_AUTHOR, author.toUtf8() );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PASSWORD, password.toUtf8() );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_COMPOSITIONID, compositionid.toUtf8() );

  mQueryType = cpqDownloadSong;
  QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("downloadsong.php"))), multiPart );
  multiPart->setParent(reply); // delete the multiPart with the reply
  }




void CsRepoClient::doUploadSong()
  {
  QSettings s;
  QString author         = s.value( KEY_AUTHOR ).toString();
  QString password       = s.value( KEY_PASSWORD ).toString();
  QString hostRepo       = s.value( KEY_WEB_REPO ).toString();
  int     localSync      = s.value( KEY_LOCAL_SYNC ).toInt();

  QString uploadCompositionId = mPlayList.compositionUpload( localSync, author );
  if( !uploadCompositionId.isEmpty() ) {
    qDebug() << "Upload song" << uploadCompositionId;
    CsCompositionSettings s = mPlayList.composition( uploadCompositionId );
    QFile file( s.path() );
    if( file.open( QIODevice::ReadOnly ) ) {
      QByteArray composition = file.readAll();
      QJsonObject obj = QJsonDocument::fromJson( composition ).object();
      //Check if it composition
      int baseVersion = obj.value(QStringLiteral(CS_BASE_VERSION_KEY)).toInt();
      if( obj.value( QStringLiteral(CS_BASE_TYPE_KEY) ).toString() == QStringLiteral(CS_BASE_TYPE) ) {
        CsJsonReader js( obj, &baseVersion );
        CsComposition comp;
        comp.jsonRead( js );

        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

        sdHttpMultiPartAppendField( multiPart, REPO_FIELD_AUTHOR, author.toUtf8() );
        sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PASSWORD, password.toUtf8() );
        sdHttpMultiPartAppendField( multiPart, REPO_FIELD_COMPOSITIONID, uploadCompositionId.toUtf8() );

        sdHttpMultiPartAppendField( multiPart, REPO_FIELD_VERSION,  comp.version() );
        sdHttpMultiPartAppendField( multiPart, REPO_FIELD_SINGER,   comp.singer().toUtf8() );
        sdHttpMultiPartAppendField( multiPart, REPO_FIELD_TITLE,    comp.title().toUtf8() );
        sdHttpMultiPartAppendField( multiPart, REPO_FIELD_ISPUBLIC, comp.header().isPublic() );
        sdHttpMultiPartAppendField( multiPart, REPO_FIELD_MELODYPRESENT, comp.header().isMelodyPresent() );
        sdHttpMultiPartAppendField( multiPart, REPO_FIELD_SONG,     composition );

        mQueryType = cpqUploadSong;
        QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("upload.php"))), multiPart );
        multiPart->setParent(reply); // delete the multiPart with the reply
        }
      }
    }
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
  mQueryType = cpqIdle;
  }




void CsRepoClient::cmList(const QJsonObject &reply)
  {
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 4 - имя уже есть в базе и пароль не совпал
  if( reply.value( QStringLiteral("result") ).toInt() == 0 ) {
    qDebug() << "list received";
    int listtime = reply.value( QStringLiteral("listtime") ).toString().toInt();
    //Retrive object list from reply
    mUpdateList = reply.value( QStringLiteral("list") ).toObject();
    if( mPlayList.version() < listtime )
      //Need update current playlist from repo
      doDownloadPlayList();
    else if( mPlayList.version() > listtime )
      //Need update playlist into repo with current
      doUploadPlayList();
    else
      doDownloadSong();
    }
  }




void CsRepoClient::cmDownloadPlayList(const QJsonObject &reply)
  {
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 4 - имя уже есть в базе и пароль не совпал
  if( reply.value( QStringLiteral("result") ).toInt() == 0 ) {
    //Retrive object list from reply
    QString playlist = reply.value( QStringLiteral("playlist") ).toString();
    qDebug() << "Play list downloaded";
    if( !playlist.isEmpty() ) {
      mPlayList.fromByteArray( playlist.toUtf8() );
      mPlayList.save();
      emit playlistChanged();
      }
    doDownloadSong();
    }
  }




void CsRepoClient::cmUploadPlayList(const QJsonObject &reply)
  {
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 4 - имя уже есть в базе и пароль не совпал
  if( reply.value( QStringLiteral("result") ).toInt() == 0 ) {
    qDebug() << "Play list uploaded";
    doDownloadSong();
    }
  else
    mUpdateList = QJsonObject{};
  }



void CsRepoClient::cmDownloadSong(const QJsonObject &reply)
  {
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 4 - имя уже есть в базе и пароль не совпал
  if( reply.value( QStringLiteral("result") ).toInt() == 0 ) {
    //Retrive object list from reply
    CsComposition comp;
    comp.fromByteArray( reply.value( QStringLiteral("song") ).toString().toLatin1() );
    QFile file( comp.header().path() );
    if( file.open(QIODevice::WriteOnly) ) {
      //Write contents to file
      file.write( comp.toByteArray() );

      //Write completed
      emit songChanged( comp.header().songId() );

      //Remove received song from waiting list
      mUpdateList.remove( comp.header().songId() );

      //Update received version
      QSettings s;
      s.setValue( KEY_REMOTE_SYNC, comp.header().version() );
      }

    doDownloadSong();
    }
  }




void CsRepoClient::cmUploadSong(const QJsonObject &reply)
  {
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 4 - имя уже есть в базе и пароль не совпал
  if( reply.value( QStringLiteral("result") ).toInt() == 0 ) {
    qDebug() << "Song uploaded";
    int localTime = reply.value( QStringLiteral("version") ).toString().toInt();
    QSettings s;
    s.setValue( KEY_LOCAL_SYNC, localTime );
    doUploadSong();
    }
  else {
    qDebug() << "Song upload error" << reply.value( QStringLiteral("result") ).toInt();
    }
  }




QString CsRepoClient::earlyCompositionId() const
  {
  int version = 0;
  QString key;
  for( auto it = mUpdateList.constBegin(); it != mUpdateList.constEnd(); it++ ) {
    int v = it.value().toObject().value(QStringLiteral("version")).toInt();
    if( key.isEmpty() || v < version ) {
      version = v;
      key = it.key();
      }
    }
  return key;
  }


