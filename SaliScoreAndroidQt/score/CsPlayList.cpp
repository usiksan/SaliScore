#include "CsPlayList.h"
#include "SdLib/SdTime2x.h"

#include <QFile>
#include <QJsonDocument>
#include <QTreeWidgetItem>

CsPlayList::CsPlayList()
  {

  }

void CsPlayList::dirtyReset()
  {
  mDirty = false;
  mVersion = SdTime2x::current();
  }



void CsPlayList::partTitleSet(int i, const QString &tit)
  {
  mPartList[i].titleSet( tit );
  mDirty = true;
  }






bool CsPlayList::partAppend(const QString &partName)
  {
  for( int i = 0; i < mPartList.count(); i++ )
    if( mPartList.at(i).title() == partName ) return false;
  CsPlayPart part;
  part.titleSet( partName );
  mPartList.append( part );
  mDirty = true;
  return true;
  }




void CsPlayList::partDelete(int i)
  {
  mPartList.removeAt(i);
  garbageCollection();
  mDirty = true;
  }






bool CsPlayList::partCompositionAppend(int partIndex, const QString &id)
  {
  if( mPartList[partIndex].compositionAppend( id ) ) {
    mDirty = true;
    return true;
    }
  return false;
  }



void CsPlayList::partCompositionRemove( int partIndex, int compositionIndex )
  {
  mPartList[partIndex].compositionRemove(compositionIndex);
  garbageCollection();
  mDirty = true;
  }





void CsPlayList::compositionSet( const CsComposition &comp )
  {
  mCompositionsMap.insert( comp.songId(), CsCompositionInfo(comp) );
  mDirty = true;
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
  }




QByteArray CsPlayList::toByteArray() const
  {
  SvJsonWriter js;
  CsJsonAttr attr;
  attr.jsonWrite( js );
  jsonWrite( js );
  return QJsonDocument( js.object() ).toJson(QJsonDocument::Compact);
  }




void CsPlayList::fromByteArray(const QByteArray &ar)
  {
  CsJsonAttr attr;
  QJsonObject obj = svJsonObjectFromByteArray( ar );
  CsJsonReader js( obj, &attr );
  attr.jsonRead( js );
  if( attr.isPlayList() )
    jsonRead( js );
  }




void CsPlayList::load()
  {
  QFile file( fileName() );
  if( file.exists() ) {
    qDebug() << "playList exist loading...";
    if( file.open( QIODevice::ReadOnly ) )
      fromByteArray( file.readAll() );
    }
  else {
    qDebug() << "playList not found" << fileName();
    //Create default play list
    mVersion = 1;
    partAppend( QObject::tr("My songs") );
    partAppend( QObject::tr("Examples") );

    CsComposition comp;
    comp.attributeSet( CS_ATTR_SINGER, "Цой" );
    comp.attributeSet( CS_ATTR_NAME, "Кукушка" );
    compositionSet( comp );
    comp.fileSave();
    partCompositionAppend( 0, comp.songId() );
    partCompositionAppend( 1, comp.songId() );
    }
  }



void CsPlayList::save()
  {
  QFile file( fileName() );
  if( dirty() )
    dirtyReset();
  if( file.open( QIODevice::WriteOnly ) )
    file.write( toByteArray() );
  }




void CsPlayList::garbageCollection()
  {
  QStringList deletionList( compositionList() );
  QSet<QString> deletionSet( deletionList.begin(), deletionList.end() );
  //Scan all parts and remove from deletionSet all existing ids
  for( const auto &part : qAsConst( mPartList )  ) {
    for( int i = 0; i < part.compositionCount(); i++ )
      deletionSet.remove( part.compositionId(i) );
    }

  //At this moment in deletionSet remain ids which must be deleted
  //Convert set to list
  deletionList = deletionSet.values();
  if( deletionList.count() ) {
    //For each id from deletion list we delete associated composition
    for( const auto &id : qAsConst(deletionList) )
      mCompositionsMap.remove( id );
    mDirty = true;
    }
  }



QString CsPlayList::fileName()
  {
  return CsComposition::mSongRepo.repoHomeDir( QString{} ) + QStringLiteral("playList.dat");
  }
