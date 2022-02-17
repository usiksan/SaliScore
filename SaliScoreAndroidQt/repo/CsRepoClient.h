#ifndef CSREPOCLIENT_H
#define CSREPOCLIENT_H

#include "config.h"
#include "score/CsPlayList.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QJsonArray>

struct CsFindSongItem
  {
    QString mCompositionId;
    QString mSinger;
    QString mTitle;
    QString mAuthor;
  };

class CsRepoClient : public QObject
  {
    Q_OBJECT

    enum CsRepoQueryType {
      cpqIdle,         //!< No active query
      cpqRegister,     //!< Registration
      cpqSyncList,     //!< Get version of repository user's playlist
      cpqDownloadList, //!< Download user's playlist from repository
      cpqUploadList,   //!< Upload current playlist to repository
      cpqSyncSong,     //!< Get version of composition in repository
      cpqDownloadSong, //!< Download composition from repository
      cpqUploadSong,   //!< Upload composition to repository
      cpqDeleteSong,   //!< Delete song from repository
      cpqFindSong      //!< Get finding list from repository
    };

    QNetworkAccessManager *mNetworkManager;  //!< Network manager through witch we connect to global repository
    QTimer                 mTimer;           //!< Timer for periodic sync with global repository
    CsPlayList            &mPlayList;        //!< Users play list
    CsRepoQueryType        mQueryType;       //!< Type of remote operation
    QStringList            mSyncList;        //!< List of composition ids which need to be sync
    QString                mNeedSong;        //!< Song need to be loaded
    QString                mFindQuery;       //!< Do finding
  public:
    QList<CsFindSongItem>  mFindSongList;    //!< Songs which match to pattern

    explicit CsRepoClient( CsPlayList &playList, QObject *parent = nullptr);

    bool    isRegistered() const;

    QString author() const;

  signals:

    //Signal on connection ok and fail
    void connectionStatus( QString msg, bool ok );

    //Signal on registration status
    void registerStatus( bool finish, const QString msg );

    void playlistChanged();

    void songChanged( const QString compositionid );

    void songLoaded( CsComposition composition );

    void findSongComplete();

  public slots:
    //!
    //! \brief finished Called when network reply finished
    //! \param reply    Reply witch being finished
    //!
    void finished( QNetworkReply *reply );

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
    void doRegister(const QString repo, const QString authorName, const QString password, const QString email );

    //!
    //! \brief doSyncPlayList Performs syncronization with remote repository.
    //!                       The first step is get time of user's playlist stored in repository
    //!
    void doSyncPlayList();

    void doFindSong( const QString &strToFind );
  private:


    void doDownloadPlayList();

    void doUploadPlayList();

    void doSyncSong();

    void doDownloadSong( const QString compositionid );

    void doUploadSong( const QString compositionid );

    void doDeletionSong();

    void doFindSong();

    //!
    //! \brief cmRegister Reply received on register query
    //! \param reply      Received reply
    //!
    void    cmRegister( const QJsonObject &reply );

    void    cmSyncList( const QJsonObject &reply );

    void    cmDownloadPlayList( const QJsonObject &reply );

    void    cmUploadPlayList( const QJsonObject &reply );

    void    cmSyncSong( const QJsonObject &reply );

    void    cmDownloadSong( const QJsonObject &reply );

    void    cmUploadSong( const QJsonObject &reply );

    void    cmDeleteSong( const QJsonObject &reply );

    void    cmFindSong( const QJsonObject &reply );
  };


//Main object for remote database communication
extern CsRepoClient *repoClient;

#endif // CSREPOCLIENT_H
