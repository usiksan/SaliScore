/*
  Project "SaliScore Score music edit, view and tutorial program"

  Author
    Sibilev Alexander S.
  Web
    www.saliLab.com
    www.saliLab.ru

  Description
    SaliScore local song repository. Repository is mirror of files on local storage.
    Files are compositionId associated with composition version.
*/
#ifndef CSSONGLOCALREPO_H
#define CSSONGLOCALREPO_H

#include "CsCompositionInfo.h"

#include <QString>
#include <QStringList>
#include <QMap>


class CsSongLocalRepo
  {
    QStringList      mCompositionsList; //!< Full list of all known compositions
    CsCompositionMap mSongMap;          //!< Map of song of local repo

    CsSongLocalRepo();
  public:
    static CsSongLocalRepo *repo();

    //!
    //! \brief repoInit Init local repository
    //!
    void        repoInit();

    int         songCount() const { return mCompositionsList.count(); }

    //!
    //! \brief songList Returns full file list as list of compositionId
    //! \return         List of compositionId
    //!
    QStringList songList() const { return mCompositionsList; }

    //!
    //! \brief songStore     Signal to local repo that composition is stored. We update repository with
    //!                      compositionId and version of composition
    //! \param compositionId Ident of composition
    //! \param comp          Composition
    //!
    void        songStore(const QString &compositionId, const CsComposition &comp );

    //!
    //! \brief songRemove    Remove composition from storage. We delete file with composition
    //! \param compositionId Ident of deleted composition
    //!
    void        songRemove( const QString &compositionId );

    //!
    //! \brief songVersion   Return file version of composition
    //! \param compositionId Ident of composition which version need to be returned
    //! \return              File version of composition
    //!
    int         songVersion( const QString &compositionId ) const;

    //!
    //! \brief repoHomeDir Local storage for SaliScore
    //! \param subDir      Sub dir name for local storage
    //! \return            Path to sub dir of local storage
    //!
    QString     repoHomeDir( const QString &subDir ) const;

    //!
    //! \brief repoSongPath Full path to composition
    //! \param songId       Ident of composition
    //! \return
    //!
    QString     repoSongPath( const QString &songId ) const;

  private:
    //!
    //! \brief repoPath Returns full path to file of repository
    //! \return         Full path to file of repository
    //!
    QString     repoPath() const;

    //!
    //! \brief repoSave Perform saving of repository to local storage
    //!
    void        repoSave();
  };

#endif // CSSONGLOCALREPO_H
