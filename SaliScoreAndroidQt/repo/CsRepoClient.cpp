#include "CsRepoClient.h"
#include "score/CsComposition.h"

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

#define REPO_FIELD_FIND_PARAM1   QStringLiteral("find1")
#define REPO_FIELD_FIND_PARAM2   QStringLiteral("find2")
#define REPO_FIELD_FIND_PARAM3   QStringLiteral("find3")
#define REPO_FIELD_FIND_PARAM4   QStringLiteral("find4")


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




CsRepoClient::CsRepoClient(QObject *parent) :
  QObject(parent),
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
      case cpqFindSong :
        cmFindSong( obj );
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
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_LISTTIME, CsPlayList::pl()->version() );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PLAYLIST, CsPlayList::pl()->toByteArray() );

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




void CsRepoClient::doFindSong(const QString &strToFind)
  {
  mFindQuery = strToFind;
  if( mQueryType == cpqIdle )
    doFindSong();
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
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_LISTTIME, CsPlayList::pl()->version() );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PLAYLIST, CsPlayList::pl()->toByteArray() );

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
    QString singer = comp.attributeGet( CS_ATTR_SINGER );
    if( singer.isEmpty() ) singer = tr("Not assigned");

    QString title = comp.attributeGet( CS_ATTR_NAME );
    if( title.isEmpty() ) title = tr("Not defined");

    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_AUTHOR, author.toUtf8() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PASSWORD, password.toUtf8() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_COMPOSITIONID, compositionid.toUtf8() );

    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_VERSION,       comp.version().toInt() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_SINGER,        singer.toUtf8() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_TITLE,         title.toUtf8() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_ISPUBLIC,      comp.isPublic() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_MELODYPRESENT, comp.isMelodyPresent() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_SONG,          comp.toByteArray() );

    mQueryType = cpqUploadSong;
    QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("uploadsong.php"))), multiPart );
    multiPart->setParent(reply); // delete the multiPart with the reply
    }
  }





void CsRepoClient::doDeletionSong()
  {
  QStringList activeSongs = CsPlayList::pl()->compositionList();
  QStringList songFiles = CsSongLocalRepo::repo()->songList();
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

  //No need to deletion
  mQueryType = cpqIdle;
  doFindSong();
  }



void CsRepoClient::doFindSong()
  {
  if( !mFindQuery.isEmpty() ) {
    //Split single string into separate substrings
    QStringList paramList = mFindQuery.split( QChar(' ') );
    mFindQuery.clear();

    //Remove from list empty and too short strings
    for( int i = paramList.count() - 1; i > 0; i-- )
      if( paramList.at(i).length() < 2 )
        paramList.removeAt(i);

    //Fill unused substring
    while( paramList.count() < 4 )
      paramList.append( QString("-----") );

    //Build query
    QSettings s;
    QString hostRepo       = s.value( KEY_WEB_REPO ).toString();

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_FIND_PARAM1, paramList.at(0).toLower().toUtf8() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_FIND_PARAM2, paramList.at(1).toLower().toUtf8() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_FIND_PARAM3, paramList.at(2).toLower().toUtf8() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_FIND_PARAM4, paramList.at(3).toLower().toUtf8() );


    mQueryType = cpqFindSong;
    QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("findsong.php"))), multiPart );
    multiPart->setParent(reply); // delete the multiPart with the reply
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
    if( CsPlayList::pl()->version() < listtime )
      //Need update current playlist from repo
      doDownloadPlayList();
    else if( CsPlayList::pl()->version() > listtime )
      //Need update playlist into repo with current
      doUploadPlayList();
    else {
      mSyncList = CsPlayList::pl()->compositionList();
      doSyncSong();
      }
    }
  else {
    //Some error
    qDebug() << "repo cmSyncList error" << reply.value( REPO_FIELD_RESULT ).toInt();
    mQueryType = cpqIdle;
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
      CsPlayList::pl()->fromByteArray( playlist.toUtf8() );
      CsPlayList::pl()->save();
      emit playlistChanged();
      }
    mSyncList = CsPlayList::pl()->compositionList();
    doSyncSong();
    }
  else {
    //Some error
    qDebug() << "repo cmDownloadPlayList error" << reply.value( REPO_FIELD_RESULT ).toInt();
    mQueryType = cpqIdle;
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
    mSyncList = CsPlayList::pl()->compositionList();
    doSyncSong();
    }
  else {
    //Some error
    qDebug() << "repo cmUploadPlayList error" << reply.value( REPO_FIELD_RESULT ).toInt();
    mQueryType = cpqIdle;
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
    int fileVersion =CsSongLocalRepo::repo()->songVersion( compositionid );
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
  else {
    //Some error
    qDebug() << "repo cmSyncSong error" << reply.value( REPO_FIELD_RESULT ).toInt();
    mQueryType = cpqIdle;
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
    qDebug() << "Composition downloaded" << reply.value( REPO_FIELD_COMPOSITIONID ).toString();
    CsComposition comp;
    comp.fromByteArray( reply.value( REPO_FIELD_SONG ).toString().toUtf8(), false );
    if( comp.fileSave() ) {
      //Write completed
      emit songChanged( comp.songId() );

      if( comp.songId() == mNeedSong ) {
        mNeedSong.clear();
        //emit songLoaded( comp );
        }

      //Remove received song from sync list
      mSyncList.removeAll( comp.songId() );
      }

    //Continue sync song
    doSyncSong();
    }
  else {
    //Some error
    qDebug() << "repo cmDownloadSong error" << reply.value( REPO_FIELD_RESULT ).toInt();
    mQueryType = cpqIdle;
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
    //Some error
    qDebug() << "repo cmUploadSong error" << reply.value( REPO_FIELD_RESULT ).toInt();
    mQueryType = cpqIdle;
    }
  }



void CsRepoClient::cmDeleteSong(const QJsonObject &reply)
  {
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 4 - имя уже есть в базе и пароль не совпал
  if( reply.value( REPO_FIELD_RESULT ).toInt() == 0 || reply.value( REPO_FIELD_RESULT ).toInt() == 3 ) {
    qDebug() << "Song deleted";
    QString compositionid = reply.value( REPO_FIELD_COMPOSITIONID ).toString();

    //Remove deleted song from local repo
    CsSongLocalRepo::repo()->songRemove( compositionid );

    //Continue deleting songs
    doDeletionSong();
    }
  else {
    //Some error
    qDebug() << "repo cmDeleteSong error" << reply.value( REPO_FIELD_RESULT ).toInt();
    mQueryType = cpqIdle;
    }
  }




void CsRepoClient::cmFindSong(const QJsonObject &reply)
  {
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 3 - не найдено подходящих песен
  // 4 - имя уже есть в базе и пароль не совпал
  if( reply.value( REPO_FIELD_RESULT ).toInt() == 0 ) {
    //Received list of matched songs
    mFindSongList.clear();
    QJsonObject list = reply.value( REPO_FIELD_PLAYLIST ).toObject();
    //Fill internal list with received songs
    for( auto it = list.constBegin(); it != list.constEnd(); it++ ) {
      CsFindSongItem item;
      item.mCompositionId = it.key();
      QJsonObject obj = it.value().toObject();
      item.mAuthor = obj.value( QStringLiteral("author") ).toString();
      item.mSinger = obj.value( QStringLiteral("singer") ).toString();
      item.mTitle  = obj.value( QStringLiteral("title") ).toString();
      mFindSongList.append( item );
      }
    emit findSongComplete();
    }
  else if( reply.value( REPO_FIELD_RESULT ).toInt() == 3 ) {
    //No songs found
    mFindSongList.clear();
    emit findSongComplete();
    }
  else {
    //Some error
    qDebug() << "repo cmFindSong error" << reply.value( REPO_FIELD_RESULT ).toInt();
    mQueryType = cpqIdle;
    }
  }




