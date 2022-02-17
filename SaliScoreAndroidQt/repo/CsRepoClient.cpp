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
  mPlayList(playList),
  mQueryType(cpqIdle)
  {

  mNetworkManager = new QNetworkAccessManager(this);

  mTimer.setInterval( 5000 );
  mTimer.start();

  connect( mNetworkManager, &QNetworkAccessManager::finished, this, &CsRepoClient::finished );
  connect( &mTimer, &QTimer::timeout, this, &CsRepoClient::doSyncPlayList );
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
      case cpqSyncList :
        cmSyncList( obj );
        break;
      case cpqDownloadList :
        cmDownloadPlayList( obj );
        break;
      case cpqUploadList :
        cmUploadPlayList( obj );
        break;
      case cpqSyncSong :
        cmSyncSong( obj );
        break;
      case cpqDownloadSong :
        cmDownloadSong( obj );
        break;
      case cpqUploadSong :
        cmUploadSong( obj );
        break;
      case cpqDeleteSong :
        cmDeleteSong( obj );
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
//! \brief doSyncPlayList Performs syncronization with remote repository.
//!                       The first step is get time of user's playlist stored in repository
//!
void CsRepoClient::doSyncPlayList()
  {
  if( mQueryType == cpqIdle && isRegistered() ) {
    mTimer.setInterval( 60000 );
    QSettings s;
    QString author         = s.value( KEY_AUTHOR ).toString();
    QString password       = s.value( KEY_PASSWORD ).toString();
    QString hostRepo       = s.value( KEY_WEB_REPO ).toString();
    qDebug() << "Do sync: query time of user's playlist";

    //Prepare block for transmition
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_AUTHOR, author.toUtf8() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PASSWORD, password.toUtf8() );

    mQueryType = cpqSyncList;
    QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("synclist.php"))), multiPart );
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




void CsRepoClient::doSyncSong()
  {
  if( !mNeedSong.isEmpty() )
    doDownloadSong( mNeedSong );
  else if( mSyncList.count() ) {
    qDebug() << "Get repository's version of composition" << mSyncList.first();
    QSettings s;
    QString hostRepo = s.value( KEY_WEB_REPO ).toString();

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_COMPOSITIONID, mSyncList.first().toUtf8() );

    mQueryType = cpqSyncSong;
    QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("syncsong.php"))), multiPart );
    multiPart->setParent(reply); // delete the multiPart with the reply
    }
  else
    doDeletionSong();
  }




void CsRepoClient::doDownloadSong(const QString compositionid)
  {
  qDebug() << "Download song" << compositionid;
  QSettings s;
  QString hostRepo       = s.value( KEY_WEB_REPO ).toString();

  QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_COMPOSITIONID, compositionid.toUtf8() );

  mQueryType = cpqDownloadSong;
  QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("downloadsong.php"))), multiPart );
  multiPart->setParent(reply); // delete the multiPart with the reply
  }




void CsRepoClient::doUploadSong(const QString compositionid)
  {
  QSettings s;
  QString author         = s.value( KEY_AUTHOR ).toString();
  QString password       = s.value( KEY_PASSWORD ).toString();
  QString hostRepo       = s.value( KEY_WEB_REPO ).toString();

  qDebug() << "Upload song" << compositionid;
  CsComposition comp;
  if( comp.fileLoad(compositionid) ) {
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    //Exclude potencial empty strings
    QString singer = comp.singer();
    if( singer.isEmpty() ) singer = tr("Not assigned");

    QString title = comp.title();
    if( title.isEmpty() ) title = tr("Not defined");

    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_AUTHOR, author.toUtf8() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PASSWORD, password.toUtf8() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_COMPOSITIONID, compositionid.toUtf8() );

    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_VERSION,       comp.version() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_SINGER,        singer.toUtf8() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_TITLE,         title.toUtf8() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_ISPUBLIC,      comp.header().isPublic() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_MELODYPRESENT, comp.isMelodyPresent() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_SONG,          comp.toByteArray() );

    mQueryType = cpqUploadSong;
    QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("uploadsong.php"))), multiPart );
    multiPart->setParent(reply); // delete the multiPart with the reply
    }
  }





void CsRepoClient::doDeletionSong()
  {
  QStringList activeSongs = mPlayList.compositionList();
  QStringList songFiles = CsComposition::mSongRepo.songList();
  QSet<QString> activeSet( activeSongs.begin(), activeSongs.end() );
  for( const auto &id : qAsConst(songFiles) )
    if( !activeSet.contains(id) ) {
      //File id not contains in active songs and must be deleted
      QSettings s;
      QString author         = s.value( KEY_AUTHOR ).toString();
      QString password       = s.value( KEY_PASSWORD ).toString();
      QString hostRepo       = s.value( KEY_WEB_REPO ).toString();

      QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

      sdHttpMultiPartAppendField( multiPart, REPO_FIELD_AUTHOR, author.toUtf8() );
      sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PASSWORD, password.toUtf8() );
      sdHttpMultiPartAppendField( multiPart, REPO_FIELD_COMPOSITIONID, id.toUtf8() );

      mQueryType = cpqDeleteSong;
      QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("deletesong.php"))), multiPart );
      multiPart->setParent(reply); // delete the multiPart with the reply
      return;
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




void CsRepoClient::cmSyncList(const QJsonObject &reply)
  {
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 4 - имя уже есть в базе и пароль не совпал
  if( reply.value( REPO_FIELD_RESULT ).toInt() == 0 ) {
    qDebug() << "time of playlist received";
    int listtime = reply.value( REPO_FIELD_LISTTIME ).toString().toInt();
    if( mPlayList.version() < listtime )
      //Need update current playlist from repo
      doDownloadPlayList();
    else if( mPlayList.version() > listtime )
      //Need update playlist into repo with current
      doUploadPlayList();
    else {
      mSyncList = mPlayList.compositionList();
      doSyncSong();
      }
    }
  }




void CsRepoClient::cmDownloadPlayList(const QJsonObject &reply)
  {
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 4 - имя уже есть в базе и пароль не совпал
  if( reply.value( REPO_FIELD_RESULT ).toInt() == 0 ) {
    //Retrive object list from reply
    QString playlist = reply.value( REPO_FIELD_PLAYLIST ).toString();
    qDebug() << "Play list downloaded";
    if( !playlist.isEmpty() ) {
      mPlayList.fromByteArray( playlist.toUtf8() );
      mPlayList.save();
      emit playlistChanged();
      }
    mSyncList = mPlayList.compositionList();
    doSyncSong();
    }
  }




void CsRepoClient::cmUploadPlayList(const QJsonObject &reply)
  {
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 4 - имя уже есть в базе и пароль не совпал
  if( reply.value( REPO_FIELD_RESULT ).toInt() == 0 ) {
    qDebug() << "Play list uploaded";
    mSyncList = mPlayList.compositionList();
    doSyncSong();
    }
  }




void CsRepoClient::cmSyncSong(const QJsonObject &reply)
  {
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 4 - имя уже есть в базе и пароль не совпал
  if( reply.value( REPO_FIELD_RESULT ).toInt() == 0 ) {
    qDebug() << "Composition version received";
    QString compositionid = reply.value( REPO_FIELD_COMPOSITIONID ).toString();
    int version = reply.value( REPO_FIELD_VERSION ).toString().toInt();
    int fileVersion = CsComposition::mSongRepo.songVersion( compositionid );
    if( version > fileVersion )
      //In repository is newest version, so download it
      doDownloadSong( compositionid );
    else if( version < fileVersion )
      //Our version is newest, so upload it to repository
      doUploadSong( compositionid );
    else {
      //No need to download or upload
      //Remove this composition from list of sync
      mSyncList.removeAll( compositionid );
      doSyncSong();
      }
    }
  }



void CsRepoClient::cmDownloadSong(const QJsonObject &reply)
  {
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 4 - имя уже есть в базе и пароль не совпал
  if( reply.value( REPO_FIELD_RESULT ).toInt() == 0 ) {
    //Retrive object list from reply
    CsComposition comp;
    comp.fromByteArray( reply.value( REPO_FIELD_SONG ).toString().toUtf8() );
    if( comp.fileSave() ) {
      //Write completed
      emit songChanged( comp.header().songId() );

      if( comp.header().songId() == mNeedSong ) {
        mNeedSong.clear();
        emit songLoaded( comp );
        }

      //Remove received song from sync list
      mSyncList.removeAll( comp.header().songId() );
      }

    //Continue sync song
    doSyncSong();
    }
  }




void CsRepoClient::cmUploadSong(const QJsonObject &reply)
  {
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 4 - имя уже есть в базе и пароль не совпал
  if( reply.value( REPO_FIELD_RESULT ).toInt() == 0 ) {
    qDebug() << "Song uploaded";
    QString compositionid = reply.value( REPO_FIELD_COMPOSITIONID ).toString();

    //Remove uploaded song from sync list
    mSyncList.removeAll( compositionid );

    //Continue sync song
    doSyncSong();
    }
  else {
    qDebug() << "Song upload error" << reply.value( QStringLiteral("result") ).toInt();
    }
  }



void CsRepoClient::cmDeleteSong(const QJsonObject &reply)
  {
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 4 - имя уже есть в базе и пароль не совпал
  if( reply.value( REPO_FIELD_RESULT ).toInt() == 0 ) {
    qDebug() << "Song deleted";
    QString compositionid = reply.value( REPO_FIELD_COMPOSITIONID ).toString();

    //Remove deleted song from local repo
    CsComposition::mSongRepo.songRemove( compositionid );

    //Continue deleting songs
    doDeletionSong();
    }
  else {
    qDebug() << "Song deletion error" << reply.value( QStringLiteral("result") ).toInt();
    }
  }




