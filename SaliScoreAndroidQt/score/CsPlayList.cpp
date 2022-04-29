#include "CsPlayList.h"
#include "CsComposition.h"
#include "CsSongLocalRepo.h"
#include "SvLib/SvTime2x.h"

#include <QFile>
#include <QJsonDocument>
#include <QTreeWidgetItem>

CsPlayList::CsPlayList() :
  mVersion(1),
  mDirty(false)
  {

  }


CsPlayList *CsPlayList::pl()
  {
  static CsPlayList playList;
  return &playList;
  }



void CsPlayList::dirtyReset()
  {
  mDirty = false;
  mVersion = SvTime2x::current();
  }



QString CsPlayList::partTitle(int i) const
  {
  if( i == 0 ) return QObject::tr("All compositions");
  return mPartList.at(i-1).title();
  }



void CsPlayList::partTitleSet(int i, const QString &tit)
  {
  if( i == 0 ) return;
  mPartList[i - 1].titleSet( tit );
  signalDirty();
  }






bool CsPlayList::partAppend(const QString &partName)
  {
  for( int i = 0; i < mPartList.count(); i++ )
    if( mPartList.at(i).title() == partName ) return false;
  CsPlayPart part;
  part.titleSet( partName );
  mPartList.append( part );
  signalDirty();
  return true;
  }




void CsPlayList::partDelete(int i)
  {
  if( i == 0 ) return;
  mPartList.removeAt(i - 1 );
  signalDirty();
  }








int CsPlayList::partCompositionCount(int partIndex) const
  {
  return partIndex == 0 ? CsSongLocalRepo::repo()->songCount() : mPartList.at(partIndex-1).compositionCount();
  }



QString CsPlayList::partCompositionId(int partIndex, int compositionIndex) const
  {
  return partIndex == 0 ? CsSongLocalRepo::repo()->songList().at(compositionIndex) : mPartList.at(partIndex - 1).compositionId(compositionIndex);
  }





bool CsPlayList::partCompositionAppend(int partIndex, const QString &id)
  {
  if( partIndex == 0 ) return false;
  if( mPartList[partIndex - 1].compositionAppend( id ) ) {
    signalDirty();
    return true;
    }
  return false;
  }



void CsPlayList::partCompositionRemove( int partIndex, int compositionIndex )
  {
  if( partIndex == 0 ) {
    //Remove composition from each part list
    QString id = partCompositionId( partIndex, compositionIndex );
    for( int i = 0; i < mPartList.count(); i++ )
      mPartList[i].compositionRemoveById( id );
    //At now remove from description list
    mCompositionsMap.remove( id );
    //And remove file from local repo
    CsSongLocalRepo::repo()->songRemove( id );
    }
  else {
    mPartList[partIndex - 1].compositionRemove(compositionIndex);
    }
  signalDirty();
  }





void CsPlayList::compositionSet( const CsComposition &comp )
  {
  mCompositionsMap.insert( comp.songId(), CsCompositionInfo(comp) );
  signalDirty();
  }







void CsPlayList::jsonWrite( CsJsonWriter &js ) const
  {
  js.jsonInt( "aversion", mVersion );
  js.jsonList( "partList", mPartList );
  js.jsonMap( "compositionMap", mCompositionsMap );
  }




void CsPlayList::jsonRead(CsJsonReader &js)
  {
  mDirty = false;
  js.jsonInt( "aversion", mVersion );
  js.jsonList( "partList", mPartList );
  js.jsonMap( "compositionMap", mCompositionsMap );
  mChanged.emitSignal();
  }




QByteArray CsPlayList::toByteArray() const
  {
  SvJsonWriter js;
  CsJsonAttr attr;
  attr.jsonWrite( js );
  jsonWrite( js );
  return QJsonDocument( js.object() ).toJson(QJsonDocument::Compact);
  }




bool CsPlayList::fromByteArray(const QByteArray &ar)
  {
  CsJsonAttr attr;
  QJsonObject obj( svJsonObjectFromByteArray( ar ) );
  CsJsonReader js( obj, &attr );
  attr.jsonRead( js );
  if( attr.isPlayList() ) {
    jsonRead( js );
    return true;
    }
  return false;
  }




void CsPlayList::load()
  {
  QFile file( fileName() );
  if( file.exists() ) {
    qDebug() << "playList exist loading...";
    if( file.open( QIODevice::ReadOnly ) ) {
      if( fromByteArray( file.readAll() ) )
        return;
      }
    }
  qDebug() << "playList not found or invalid" << fileName();
  //Create default play list
  mVersion = 1;
  partAppend( QObject::tr("My songs") );
  partAppend( QObject::tr("Examples") );
  }



void CsPlayList::save()
  {
  QFile file( fileName() );
  if( dirty() )
    dirtyReset();
  if( file.open( QIODevice::WriteOnly ) )
    file.write( toByteArray() );
  }




void CsPlayList::signalDirty()
  {
  mDirty = true;
  mChanged.emitSignal();
  }




QString CsPlayList::fileName()
  {
  return CsSongLocalRepo::repo()->repoHomeDir( QString{} ) + QStringLiteral("playList.dat");
  }




