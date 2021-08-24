#ifndef CSREPOCLIENT_H
#define CSREPOCLIENT_H

#include "CsConfig.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QJsonArray>

class CsRepoClient : public QObject
  {
    Q_OBJECT

    QNetworkAccessManager *mNetworkManager;  //!< Network manager through witch we connect to global repository
    QTimer                 mTimer;           //!< Timer for periodic sync with global repository
//    SdRemoteQueryType      mQueryType;       //!< Type of remote operation
//    QList<int>             mObjectIndexList; //!< Object index list of newest objects from remote repository
//    QStringList            mInfoList;       //!< List for information items. When any event happens then information item appends
  public:
    explicit CsRepoClient(QObject *parent = nullptr);

    bool    isRegistered() const;

    QString author() const;

  signals:

    //Signal on connection ok and fail
    void connectionStatus( QString msg, bool ok );

    //Signal on registration status
    void registerStatus( const QString msg, const QString email );

  public slots:
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
    void doRegister(const QString repo, const QString authorName, const QString password, const QString email, int listtime, QByteArray playlist );
  };

#endif // CSREPOCLIENT_H
