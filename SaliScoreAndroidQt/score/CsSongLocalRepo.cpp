#include "CsConfig.h"
#include "CsSongLocalRepo.h"
#include "CsComposition.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>


CsSongLocalRepo::CsSongLocalRepo()
  {

  }



CsSongLocalRepo *CsSongLocalRepo::repo()
  {
  static CsSongLocalRepo localRepo;
  return &localRepo;
  }






//!
//! \brief repoInit Init local repository
//!
void CsSongLocalRepo::repoInit()
  {
  QFile file( repoPath() );
  if( file.open(QIODevice::ReadOnly) ) {
    QJsonObject obj( svJsonObjectFromByteArray(file.readAll()) );
    SvJsonReader js( obj );
    js.jsonMap( "repo", mSongMap );
    mCompositionsList = mSongMap.keys();
    qDebug() << "repo" << mCompositionsList;
    }
  }





//!
//! \brief songStore     Signal to local repo that composition is stored. We update repository with
//!                      compositionId and version of composition
//! \param compositionId Ident of composition
//! \param comp          Composition
//!
void CsSongLocalRepo::songStore(const QString &compositionId, const CsComposition &comp )
  {
  //Test if no changes assigned then nothing done
  if( mSongMap.contains(compositionId) && mSongMap.value(compositionId).version() == comp.version() )
    return;

  //Store new values
  CsCompositionInfo info( comp );
  mSongMap.insert( compositionId, info );

  //Save changes to file
  repoSave();
  }




//!
//! \brief songRemove    Remove composition from storage. We delete file with composition
//! \param compositionId Ident of deleted composition
//!
void CsSongLocalRepo::songRemove(const QString &compositionId)
  {
  //Remove song file from disk
  QFile::remove( repoSongPath(compositionId) );

  //Remove song from repo
  mSongMap.remove( compositionId );

  //Save changes to file
  repoSave();
  }





//!
//! \brief songVersion   Return file version of composition
//! \param compositionId Ident of composition which version need to be returned
//! \return              File version of composition
//!
int CsSongLocalRepo::songVersion(const QString &compositionId) const
  {
  return mSongMap.value( compositionId ).version().toInt();
  }





//!
//! \brief repoHomeDir Local storage for SaliScore
//! \param subDir      Sub dir name for local storage
//! \return            Path to sub dir of local storage
//!
QString CsSongLocalRepo::repoHomeDir(const QString &subDir) const
  {
  QString dirPath = QDir::homePath() + QStringLiteral(CS_DATA_DIRECTORY);
  if( !subDir.isEmpty() )
    dirPath.append( subDir );
  QDir dir;
  dir.mkpath( dirPath );
  if( !dirPath.endsWith( QChar('/') ) )
    dirPath.append( QChar('/') );
  return dirPath;
  }






//!
//! \brief repoSongPath Full path to composition
//! \param songId       Ident of composition
//! \return
//!
QString CsSongLocalRepo::repoSongPath(const QString &songId) const
  {
  return repoHomeDir( songId.right(2) ) + songId + QStringLiteral(CS_BASE_EXTENSION);
  }





//!
//! \brief repoPath Returns full path to file of repository
//! \return         Full path to file of repository
//!
QString CsSongLocalRepo::repoPath() const
  {
  return repoHomeDir( QString{} ) + QStringLiteral("localRepo.dat");
  }





//!
//! \brief repoSave Perform saving of repository to local storage
//!
void CsSongLocalRepo::repoSave()
  {
  //Create file and try to open it to write
  QFile file( repoPath() );
  if( file.open(QIODevice::WriteOnly) ) {
    //File opened successfull
    //We create writer and write repository to file
    SvJsonWriter js;
    js.jsonMap( "repo", mSongMap );

    mCompositionsList = mSongMap.keys();
    file.write( svJsonObjectToByteArray( js.object() ) );
    }
  }
