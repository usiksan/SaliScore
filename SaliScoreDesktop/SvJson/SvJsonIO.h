/*
  Проект     "Повышение наглядности ввода-вывода json"
  Автор
    Сибилев А.С.
  Описание
    Здесь два класса: писатель и читатель. Сигнатуры вызова функций чтения и записи в
    обоих классах полностью идентичны, поэтому вызовы могут быть скопированы и в функцию
    записи и чтения
*/
#ifndef SVJSONIO_H
#define SVJSONIO_H

#include <QJsonObject>
#include <QJsonArray>


//!
//! \brief The SvJsonWriter class Unificate json io class, through which json written
//!
class SvJsonWriter
  {
    QJsonObject  mObject;    //!< Working JSON-object if it not passed by caller
    QJsonObject &mObjectRef; //!< Output JSON-object reference
  public:
    //!
    //! \brief SvJsonWriter Constructor with external json object as destignator
    //! \param obj          External destignator json object
    //!
    SvJsonWriter( QJsonObject &obj ) : mObjectRef(obj) {}


    //!
    //! \brief SvJsonWriter Constructor with internal json object as destignator
    //!
    SvJsonWriter() : mObjectRef(mObject) {}


    //!
    //! \brief object Returns destignator json object
    //! \return       Destignator json object
    //!
    QJsonObject object() { return mObjectRef; }


    //!
    //! \brief jsonBool Transfer bool value
    //! \param key      Key for value
    //! \param b        Bool value
    //!
    void jsonBool( const char *key, bool b ) { mObjectRef.insert( QString(key), b ); }


    //!
    //! \brief jsonInt Transfer int value
    //! \param key     Key for value
    //! \param v       Int value
    //!
    void jsonInt( const char *key, int v ) { mObjectRef.insert( QString(key), v ); }


    //!
    //! \brief jsonDouble Transfer double value
    //! \param key        Key for value
    //! \param d          Double value
    //!
    void jsonDouble( const char *key, double d ) { mObjectRef.insert( QString(key), d ); }


    //!
    //! \brief jsonString Transfer string value
    //! \param key        Key for value
    //! \param s          String to transfer
    //!
    void jsonString( const char *key, const QString &s ) { mObjectRef.insert( QString(key), s ); }


    //!
    //! \brief jsonListInt Transfer list of int values
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    void jsonListInt( const char *key, const QList<int> &list )
      {
      QJsonArray ar;
      for( auto const &item : list )
        ar.append( item );
      mObjectRef.insert( QString(key), ar );
      }


    //!
    //! \brief jsonListString Transfer list of string values
    //! \param key            Key for list
    //! \param list           List to transfer
    //!
    void jsonListString( const char *key, const QStringList &list )
      {
      QJsonArray ar;
      for( auto const &item : list )
        ar.append( item );
      mObjectRef.insert( QString(key), ar );
      }


    //!
    //! \brief jsonList Template Transfer list of any values
    //!                 Value class must contains jsonWrite method, which returns
    //!                 json object for value object
    //! \param key      Key for list
    //! \param list     List to transfer
    //!
    template<typename SvClass>
    void jsonList( const char *key, const QList<SvClass> &list )
      {
      QJsonArray ar;
      for( auto const &item : list ) {
        SvJsonWriter js;
        item.jsonWrite( js );
        ar.append( js.object() );
        }
      mObjectRef.insert( QString(key), ar );
      }


    //!
    //! \brief jsonMapString Transfer map of strings
    //! \param key           Key for map
    //! \param map           Map to transfer
    //!
    void jsonMapString( const char *key, const QMap<QString,QString> &map )
      {
      QJsonObject obj;
      for( auto i = map.constBegin(); i != map.constEnd(); i++ )
        obj.insert( i.key(), i.value() );
      mObjectRef.insert( QString(key), obj );
      }


    //!
    //! \brief jsonMap Template transfer map of any values with QString as key
    //!                Value class must contains jsonWrite method, which returns
    //!                json object for value object
    //! \param key     Key for map
    //! \param list    Map to transfer
    //!
    template<typename SvClass>
    void jsonMap( const char *key, const QMap<QString,SvClass> &map )
      {
      QJsonObject obj;
      for( auto i = map.constBegin(); i != map.constEnd(); i++ ) {
        SvJsonWriter js;
        i.value().jsonWrite( js );
        obj.insert( i.key(), js.object() );
        }
      mObjectRef.insert( QString(key), obj );
      }


    //!
    //! \brief jsonObject Template transfer any value as json object
    //!                   Value class must contains jsonWrite method, which returns
    //!                   json object for value object
    //! \param key        Key for object
    //! \param obj        Object to transfer
    //!
    template<typename SvClass>
    void jsonObject( const char *key, const SvClass &obj )
      {
      SvJsonWriter js;
      obj.jsonWrite( js );
      mObjectRef.insert( QString(key), js.object() );
      }


    //!
    //! \brief jsonObjectPtr Template transfer any value as json object
    //!                      Value class must contains jsonWrite method, which returns
    //!                      json object for value object
    //! \param key           Key for object
    //! \param objPtr        Object pointer to transfer
    //!
    template<typename SvClass>
    void jsonObjectPtr( const char *key, const SvClass *objPtr )
      {
      SvJsonWriter js;
      objPtr->jsonWrite( js );
      mObjectRef.insert( QString(key), js.object() );
      }
  };






//!
//! \brief The SvJsonWriter class Unificate json io class, through which json readed
//!
class SvJsonReader
  {
    const QJsonObject &mObject;  //!< JSON object reference from which will readed
    int                mVersion; //!< Version of database
  public:
    //!
    //! \brief SvJsonReader Constructor for reader
    //! \param obj          Object which json readed
    //!
    SvJsonReader( const QJsonObject &obj, int ver = 0 ) : mObject(obj), mVersion(ver) {}

    //!
    //! \brief version Returns version of database
    //! \return        Version of database
    //!
    int  version() const { return mVersion; }

    //!
    //! \brief jsonBool Transfer bool value
    //! \param key      Key for value
    //! \param b        Bool value
    //!
    void jsonBool( const char *title, bool &b ) { b = mObject.value( QString(title) ).toBool(); }


    //!
    //! \brief jsonInt Transfer int value
    //! \param key     Key for value
    //! \param v       Int value
    //!
    void jsonInt( const char *key, int &v ) { v = mObject.value( QString(key) ).toInt(); }


    //!
    //! \brief jsonDouble Transfer double value
    //! \param key        Key for value
    //! \param d          Double value
    //!
    void jsonDouble( const char *key, double &d ) { d = mObject.value( QString(key) ).toDouble(); }


    //!
    //! \brief jsonString Transfer string value
    //! \param key        Key for value
    //! \param s          String to transfer
    //!
    void jsonString( const char *key, QString &s ) { s = mObject.value( QString(key) ).toString(); }


    //!
    //! \brief jsonListInt Transfer list of int values
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    void jsonListInt( const char *key, QList<int> &list )
      {
      list.clear();
      QJsonArray ar = mObject.value( QString(key) ).toArray();
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        int v = i->toInt();
        list.append( v );
        }
      }


    //!
    //! \brief jsonListString Transfer list of string values
    //! \param key            Key for list
    //! \param list           List to transfer
    //!
    void jsonListString( const char *key, QStringList &list )
      {
      list.clear();
      QJsonArray ar = mObject.value( QString(key) ).toArray();
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        QString v = i->toString();
        list.append( v );
        }
      }


    //!
    //! \brief jsonList Template Transfer list of any values
    //!                 Value class must contains jsonRead method, which builds
    //!                 object from json
    //! \param key      Key for list
    //! \param list     List to transfer
    //!
    template<typename SvClass>
    void jsonList( const char *key, QList<SvClass> &list )
      {
      list.clear();
      QJsonArray ar = mObject.value( QString(key) ).toArray();
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        SvClass item;
        SvJsonReader js( i->toObject(), mVersion );
        item.jsonRead( js );
        list.append( item );
        }
      }


    //!
    //! \brief jsonMapString Transfer map of strings
    //! \param key           Key for map
    //! \param map           Map to transfer
    //!
    void jsonMapString( const char *key, QMap<QString,QString> &map )
      {
      map.clear();
      QJsonObject obj = mObject.value( QString(key) ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        QString str = i.value().toString();
        map.insert( i.key(), str );
        }
      }


    //!
    //! \brief jsonMap Template transfer map of any values with QString as key
    //!                Value class must contains jsonRead method, which builds
    //!                object from json
    //! \param key     Key for map
    //! \param list    Map to transfer
    //!
    template<typename SvClass>
    void jsonMap( const char *key, QMap<QString,SvClass> &map )
      {
      map.clear();
      QJsonObject obj = mObject.value( QString(key) ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        SvClass item;
        SvJsonReader js( i.value().toObject(), mVersion );
        item.jsonRead( js );
        map.insert( i.key(), item );
        }
      }


    //!
    //! \brief jsonObject Template transfer any value as json object
    //!                   Value class must contains jsonRead method, which builds
    //!                   object from json
    //! \param key        Key for object
    //! \param obj        Object to transfer
    //!
    template<typename SvClass>
    void jsonObject( const char *key, SvClass &obj )
      {
      SvJsonReader js( mObject.value( QString(key) ).toObject(), mVersion );
      obj.jsonRead( js );
      }


    //!
    //! \brief jsonObjectPtr Template transfer any value as json object
    //!                      Value class must contains jsonRead method, which builds
    //!                      object from json
    //! \param key           Key for object
    //! \param objPtr        Object pointer to transfer
    //!
    template<typename SvClass>
    void jsonObjectPtr( const char *key, SvClass *objPtr )
      {
      SvJsonReader js( mObject.value( QString(key) ).toObject(), mVersion );
      objPtr->jsonRead( js );
      }

  };


#endif // SVJSONIO_H
