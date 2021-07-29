#ifndef SVJSONIO_H
#define SVJSONIO_H

#include <QJsonObject>
#include <QJsonArray>

class SvJsonWriter
  {
    QJsonObject &mObject;
  public:
    SvJsonWriter( QJsonObject &obj ) : mObject(obj) {}

    void jsonBool( const char *key, bool b ) { mObject.insert( QString(key), b ); }

    void jsonInt( const char *key, int v ) { mObject.insert( QString(key), v ); }

    void jsonString( const char *key, const QString &s ) { mObject.insert( QString(key), s ); }

    template<typename SvClass>
    void jsonList( const char *key, const QList<SvClass> &list )
      {
      QJsonArray ar;
      for( auto const &item : list )
        ar.append( item.write() );
      mObject.insert( QString(key), ar );
      }

    template<typename SvClass>
    void jsonMap( const char *key, const QMap<QString,SvClass> &map )
      {
      QJsonObject obj;
      for( auto i = map.constBegin(); i != map.constEnd(); i++ )
        obj.insert( i.key(), i.value().write() );
      mObject.insert( QString(key), obj );
      }
  };



class SvJsonReader
  {
    const QJsonObject &mObject;
  public:
    SvJsonReader( const QJsonObject &obj ) : mObject(obj) {}

    void jsonBool( const char *title, bool &b ) { b = mObject.value( QString(title) ).toBool(); }

    void jsonInt( const char *key, int &v ) { v = mObject.value( QString(key) ).toInt(); }

    void jsonString( const char *key, QString &s ) { s = mObject.value( QString(key) ).toString(); }

    template<typename SvClass>
    void jsonList( const char *key, QList<SvClass> &list )
      {
      list.clear();
      QJsonArray ar = mObject.value( QString(key) ).toArray();
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        SvClass item;
        item.read( i->toObject() );
        list.append( item );
        }
      }

    template<typename SvClass>
    void jsonMap( const char *key, QMap<QString,SvClass> &map )
      {
      map.clear();
      QJsonObject obj = mObject.value( QString(key) ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        SvClass item;
        item.read( i.value().toObject() );
        map.insert( i.key(), item );
        }
      }
  };


#endif // SVJSONIO_H
