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
  partItem( i )->setText( 1, tit );
  }






bool CsPlayList::partAppend(const QString &partName)
  {
  for( int i = 0; i < mPartList.count(); i++ )
    if( mPartList.at(i).title() == partName ) return false;
  CsPlayPart part;
  part.titleSet( partName );
  mPartList.append( part );
  return true;
  }




void CsPlayList::partCompositionItemSet(int partIndex, int compositionIndex, QTreeWidgetItem *item)
  {
  mPartList[partIndex].mTreeItemList[compositionIndex] = item;
  }



bool CsPlayList::partCompositionAppend(int partIndex, const QString &id)
  {
  return mPartList[partIndex].compositionAppend( id );
  }





void CsPlayList::compositionSet( const CsComposition &comp )
  {
  mCompositionsMap.insert( comp.header().songId(), CsCompositionSettings(comp) );
  mDirty = true;
  }




void CsPlayList::jsonWrite( CsJsonWriter &js ) const
  {
  js.jsonInt( "aversion", mVersion );
  js.jsonList<CsPlayPart>( "partList", mPartList );
  js.jsonMap<CsCompositionSettings>( "compositionMap", mCompositionsMap );
  }




void CsPlayList::jsonRead(CsJsonReader &js)
  {
  js.jsonInt( "aversion", mVersion );
  js.jsonList<CsPlayPart>( "partList", mPartList );
  js.jsonMap<CsCompositionSettings>( "compositionMap", mCompositionsMap );
  }




QByteArray CsPlayList::toByteArray() const
  {
  SvJsonWriter js;
  jsonWrite( js );
  return QJsonDocument( js.object() ).toJson();
  }




void CsPlayList::fromByteArray(const QByteArray &ar)
  {
  int version = 0;
  QJsonObject obj = QJsonDocument::fromJson( ar ).object();
  SvJsonReaderExtInt js( obj, &version );
  jsonRead( js );
  }




void CsPlayList::load()
  {
  QFile file( CsDescrSong::homeDir( QString{} ) + "playList.dat" );
  if( file.exists() ) {
    if( file.open( QIODevice::ReadOnly ) )
      fromByteArray( file.readAll() );
    }
  else {
    //Create default play list
    mVersion = 1;
    partAppend( QObject::tr("My songs") );
    partAppend( QObject::tr("Examples") );
    }
  }



void CsPlayList::save()
  {
  QFile file( CsDescrSong::homeDir( QString{} ) + "playList.dat" );
  if( dirty() )
    dirtyReset();
  if( file.open( QIODevice::WriteOnly ) )
    file.write( toByteArray() );
  }
