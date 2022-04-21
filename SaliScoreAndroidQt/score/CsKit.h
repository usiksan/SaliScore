#ifndef CSKIT_H
#define CSKIT_H

#include "CsJsonIO.h"

#include <QMap>
#include <QString>

template <class Item>
class CsKit
  {
  protected:
    QMap<QString,Item> mKitMap;
  public:
    CsKit() {}

    bool       isEmpty() const { return mKitMap.isEmpty(); }

    bool       contains( const QString key ) const { return mKitMap.contains(key); }

    const Item get( const QString &name ) const { return mKitMap.value(name); }

    void       set( const QString &name, const Item &t ) { mKitMap.insert( name, t ); }

    void       rename( const QString &oldName, const QString &newName )
      {
      if( oldName != newName ) {
        mKitMap.insert( newName, mKitMap.value(oldName) );
        mKitMap.remove( oldName );
        }
      }

    void       remove( const QString &name ) { mKitMap.remove( name ); }

  };



class CsKitOfString : public CsKit<QString>
  {
  public:
    CsKitOfString() : CsKit<QString>() {}

    void       jsonWrite( const char *key, CsJsonWriter &js ) const { js.jsonMapString( key, mKitMap ); }

    void       jsonRead( const char *key, CsJsonReader &js ) { js.jsonMapString( key, mKitMap ); }

  };



template <typename Item>
class CsKitOfList : public CsKit<QList<Item> >
  {
  public:
    CsKitOfList() : CsKit<QList<Item> >() {}

    void       jsonWrite( const char *key, CsJsonWriter &js ) const { js.jsonMapList( key, this->mKitMap ); }

    void       jsonRead( const char *key, CsJsonReader &js ) { js.jsonMapList( key, this->mKitMap ); }
  };


#endif // CSKIT_H
