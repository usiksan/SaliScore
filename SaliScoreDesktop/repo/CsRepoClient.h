#ifndef CSREPOCLIENT_H
#define CSREPOCLIENT_H

#include "CsConfig.h"
#include "score/CsPlayList.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QJsonArray>

class CsRepoClient : public QObject
  {
    Q_OBJECT

    enum CsRepoQueryType {
      cpqIdle, //!< No active query
      cpqRegister, //!< Registration
      cpqList,
      cpqDownloadList,
      cpqUploadList,
      cpqDownloadSong,
      cpqUploadSong
    };

    QNetworkAccessManager *mNetworkManager;  //!< Network manager through witch we connect to global repository
    QTimer                 mTimer;           //!< Timer for periodic sync with global repository
    CsPlayList            &mPlayList;        //!< Users play list
    CsRepoQueryType        mQueryType;       //!< Type of remote operation
    QJsonObject            mUpdateList;
//    QList<int>             mObjectIndexList; //!< Object index list of newest objects from remote repository
//    QStringList            mInfoList;       //!< List for information items. When any event happens then information item appends
  public:
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
    //! \brief doSync Performs syncronization with remote repository
    //!
    void doSync();

  private:

    void doDownloadPlayList();

    void doUploadPlayList();

    void doDownloadSong();

    void doDownloadSong( const QString compositionid );

    void doUploadSong();

    //!
    //! \brief cmRegister Reply received on register query
    //! \param reply      Received reply
    //!
    void    cmRegister( const QJsonObject &reply );

    void    cmList( const QJsonObject &reply );

    void    cmDownloadPlayList( const QJsonObject &reply );

    void    cmUploadPlayList( const QJsonObject &reply );

    void    cmDownloadSong( const QJsonObject &reply );

    void    cmUploadSong( const QJsonObject &reply );

    QString earlyCompositionId() const;
  };


//Main object for remote database communication
extern CsRepoClient *repoClient;

#endif // CSREPOCLIENT_H
