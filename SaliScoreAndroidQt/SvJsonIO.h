/*
  Project "Improving the visibility of json io"
  Author
    Alexander Sibilev
  Description
    There are two classes here: writer and reader. Signatures of calling read and write functions in
    both classes are completely identical, so calls can be copied to both the write and read functions.
  www
    www.salilab.com
  History
    05.02.2022 v1 Begin version support
    05.03.2022 v2 Append QPoint support
    06.04.2022 v3 Append value of any class support which must be have jsonWrite and jsonRead members
    21.04.2022 v4 Append map of list of any class support
    26.04.2022 v5 Full English remark
    24.05.2022 v6 Replace all const char* on QString which support both using with const char* and QString
    25.05.2022 v7 Append contains member in SvJsonWriter
*/
#ifndef SVJSONIO_H
#define SVJSONIO_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QColor>
#include <QMap>
#include <QPoint>
#include <functional>

#define SV_JSON_VERSION 7

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

    SvJsonWriter( const SvJsonWriter& ) : mObjectRef(mObject) {}


    //!
    //! \brief SvJsonWriter Constructor with internal json object as destignator
    //!
    SvJsonWriter() : mObjectRef(mObject) {}


    //!
    //! \brief object Returns destignator json object
    //! \return       Destignator json object
    //!
    QJsonObject &object() { return mObjectRef; }

    //!
    //! \brief contains Return true if key contains in SvJsonWriter object
    //! \param key      Tested key
    //! \return         true if key contains in SvJsonWriter object
    //!
    bool contains( const QString &key ) const { return mObjectRef.contains( key ); }
  };


//!
//! \brief jsonBool Transfer bool value
//! \param key      Key for value
//! \param b        Bool value
//!
inline void svJsonBool( SvJsonWriter &js, const QString &key, bool b ) { js.object().insert( key, b ); }
inline void svJsonBool( SvJsonWriter &js, const QString &key, bool b, bool ) { js.object().insert( key, b ); }




//!
//! \brief jsonInt Transfer int value
//! \param key     Key for value
//! \param v       Int value
//!
inline void svJsonInt( SvJsonWriter &js, const QString &key, int v ) { js.object().insert( key, v ); }
inline void svJsonInt( SvJsonWriter &js, const QString &key, int v, int ) { js.object().insert( key, v ); }


//!
//! \brief jsonInt Transfer color value
//! \param key     Key for value
//! \param v       Int value
//!
inline void svJsonColor( SvJsonWriter &js, const QString &key, QColor color ) { js.object().insert( key, color.name() ); }
inline void svJsonColor( SvJsonWriter &js, const QString &key, QColor color, QColor ) { js.object().insert( key, color.name() ); }


//!
//! \brief jsonDouble Transfer double value
//! \param key        Key for value
//! \param d          Double value
//!
inline void svJsonDouble( SvJsonWriter &js, const QString &key, double d ) { js.object().insert( key, d ); }
inline void svJsonDouble( SvJsonWriter &js, const QString &key, double d, double ) { js.object().insert( key, d ); }


//!
//! \brief jsonString Transfer string value
//! \param key        Key for value
//! \param s          String to transfer
//!
inline void svJsonString( SvJsonWriter &js, const QString &key, const QString &s ) { js.object().insert( key, s ); }
inline void svJsonString( SvJsonWriter &js, const QString &key, const QString &s, QString ) { js.object().insert( key, s ); }


//!
//! \brief jsonPoint Transfer point value
//! \param key       Key for value
//! \param p         Point to transfer
//!
template <class SvJsonWriterImpl>
void svJsonPoint( SvJsonWriterImpl &js, const QString &key, QPoint p ) {
  SvJsonWriter sjs;
  svJsonInt( sjs, "x", p.rx() );
  svJsonInt( sjs, "y", p.ry() );
  js.object().insert( key, sjs.object() );
  }


//!
//! \brief jsonListInt Transfer list of int values
//! \param key         Key for list
//! \param list        List to transfer
//!
inline void svJsonListInt( SvJsonWriter &js, const QString &key, const QList<int> &list )
  {
  QJsonArray ar;
  for( auto const &item : list )
    ar.append( item );
  js.object().insert( key, ar );
  }


//!
//! \brief jsonListDouble Transfer list of double values
//! \param key            Key for list
//! \param list           List to transfer
//!
inline void svJsonListDouble( SvJsonWriter &js, const QString &key, const QList<double> &list )
  {
  QJsonArray ar;
  for( auto const &item : list )
    ar.append( item );
  js.object().insert( key, ar );
  }


//!
//! \brief jsonListString Transfer list of string values
//! \param key            Key for list
//! \param list           List to transfer
//!
inline void svJsonListString( SvJsonWriter &js, const QString &key, const QStringList &list )
  {
  QJsonArray ar;
  for( auto const &item : list )
    ar.append( item );
  js.object().insert( key, ar );
  }


//!
//! \brief jsonList Template Transfer list of any values
//!                 Value class must contains jsonWrite method, which returns
//!                 json object for value object
//! \param key      Key for list
//! \param list     List to transfer
//!
template<typename SvClass, typename SvJsonWriterImpl>
void svJsonList( SvJsonWriterImpl &js, const QString &key, const QList<SvClass> &list )
  {
  QJsonArray ar;
  for( auto const &item : list ) {
    SvJsonWriterImpl sjs( js );
    item.jsonWrite( sjs );
    ar.append( sjs.object() );
    }
  js.object().insert( key, ar );
  }


//!
//! \brief jsonListPtr Template Transfer list of pointers to any value
//!                    Value class must contains jsonRead method, which builds
//!                    object from json. To build Value object Value class must
//!                    contains static SvClass *build( SvJsonReader &js )
//! \param key         Key for list
//! \param list        List to transfer
//!
template<typename SvClass, typename SvJsonWriterImpl>
void svJsonListPtr( SvJsonWriterImpl &js, const QString &key, const QList<SvClass*> &list )
  {
  QJsonArray ar;
  for( auto const &item : list ) {
    SvJsonWriterImpl sjs( js );
    item->jsonWrite( sjs );
    ar.append( sjs.object() );
    }
  js.object().insert( key, ar );
  }


//!
//! \brief jsonListPtr Template Transfer list of pointers to any value
//!                    Value class must contains jsonRead method, which builds
//!                    object from json. To build Value object Value class must
//!                    contains static SvClass *build( SvJsonReader &js )
//! \param key         Key for list
//! \param list        List to transfer
//! \param prefixFun   Prefix function. It call for each item in list and if it return true then
//!                    item written into json in other case nothing done
//!
template<typename SvClass, typename SvJsonWriterImpl>
void svJsonListPtr( SvJsonWriterImpl &js, const QString &key, const QList<SvClass*> &list, std::function<bool(const SvClass*)> prefixFun )
  {
  QJsonArray ar;
  for( auto const &item : list ) {
    if( prefixFun(item) ) {
      SvJsonWriterImpl sjs( js );
      item->jsonWrite( sjs );
      ar.append( sjs.object() );
      }
    }
  js.object().insert( key, ar );
  }


//!
//! \brief jsonMapString Transfer map of strings
//! \param key           Key for map
//! \param map           Map to transfer
//!
inline void svJsonMapString( SvJsonWriter &js, const QString &key, const QMap<QString,QString> &map )
  {
  QJsonObject obj;
  for( auto i = map.constBegin(); i != map.constEnd(); i++ )
    obj.insert( i.key(), i.value() );
  js.object().insert( key, obj );
  }


//!
//! \brief jsonMapInt Transfer map of int
//! \param key        Key for map
//! \param map        Map to transfer
//!
inline void svJsonMapInt( SvJsonWriter &js, const QString &key, const QMap<QString,int> &map )
  {
  QJsonObject obj;
  for( auto i = map.constBegin(); i != map.constEnd(); i++ )
    obj.insert( i.key(), i.value() );
  js.object().insert( key, obj );
  }


//!
//! \brief jsonMap Template transfer map of any values with QString as key
//!                Value class must contains jsonWrite method, which returns
//!                json object for value object
//! \param key     Key for map
//! \param list    Map to transfer
//!
template<typename SvClass, typename SvJsonWriterImpl>
void svJsonMap( SvJsonWriterImpl &js, const QString &key, const QMap<QString,SvClass> &map )
  {
  QJsonObject obj;
  for( auto i = map.constBegin(); i != map.constEnd(); i++ ) {
    SvJsonWriterImpl sjs( js );
    i.value().jsonWrite( sjs );
    obj.insert( i.key(), sjs.object() );
    }
  js.object().insert( key, obj );
  }



//!
//! \brief jsonMapList Template transfer map of list of any values with QString
//!                    as key of map. Value class must contains jsonWrite method,
//!                    which do writing into SvJsonWriter object
//! \param key         Key for map
//! \param map         Map of list of object to transfer
//!
template<typename SvClass, typename SvJsonWriterImpl>
void svJsonMapList( SvJsonWriterImpl &js, const QString &key, const QMap<QString, QList<SvClass> > &map )
  {
  QJsonObject obj;
  for( auto i = map.constBegin(); i != map.constEnd(); i++ ) {
    QJsonArray ar;
    auto list = i.value();
    for( auto const &item : list ) {
      SvJsonWriterImpl sjs( js );
      item.jsonWrite( sjs );
      ar.append( sjs.object() );
      }
    obj.insert( i.key(), ar );
    }
  js.object().insert( key, obj );
  }



//!
//! \brief jsonMapPtr Template transfer map of any values with QString as key
//!                   Value class must contains jsonRead method, which builds
//!                   object from json
//! \param key        Key for map
//! \param list       Map to transfer
//!
template<typename SvClass, typename SvJsonWriterImpl>
void svJsonMapPtr( SvJsonWriterImpl &js, const QString &key, const QMap<QString,SvClass*> &map )
  {
  QJsonObject obj;
  for( auto i = map.constBegin(); i != map.constEnd(); i++ ) {
    SvJsonWriterImpl sjs( js );
    i.value()->jsonWrite( sjs );
    obj.insert( i.key(), sjs.object() );
    }
  js.object().insert( key, obj );
  }


//!
//! \brief jsonObject Template transfer any value as json object
//!                   Value class must contains jsonWrite method, which returns
//!                   json object for value object
//! \param key        Key for object
//! \param obj        Object to transfer
//!
template<typename SvClass, typename SvJsonWriterImpl>
void svJsonObject( SvJsonWriterImpl &js, const QString &key, const SvClass &obj )
  {
  SvJsonWriterImpl sjs( js );
  obj.jsonWrite( sjs );
  js.object().insert( key, sjs.object() );
  }


//!
//! \brief jsonObjectPtr Template transfer any value as json object
//!                      Value class must contains jsonWrite method, which returns
//!                      json object for value object
//! \param key           Key for object
//! \param objPtr        Object pointer to transfer
//!
template<typename SvClass, typename SvJsonWriterImpl>
void svJsonObjectPtr( SvJsonWriterImpl &js, const QString &key, const SvClass *objPtr )
  {
  SvJsonWriterImpl sjs;
  objPtr->jsonWrite( sjs );
  js.object().insert( key, sjs.object() );
  }


//!
//! \brief jsonValue Template transfer any value as json value
//!                  Value class must contains jsonWrite method which
//!                  writes object value into json object
//! \param key       Key for value
//! \param val       Object value to transfer
//!
template<typename SvClass>
void svJsonValue( SvJsonWriter &js, const QString &key, const SvClass &val )
  {
  val.json( js, key );
  }







//!
//! \brief The SvJsonReader class Unificate json io class, through which json readen
//!
class SvJsonReader
  {
  protected:
    const QJsonObject &mObject;    //!< JSON object reference from which will readed
  public:
    //!
    //! \brief SvJsonReader Constructor for reader
    //! \param obj          Object which json readed
    //!
    SvJsonReader( const QJsonObject &obj ) : mObject(obj) {}

    SvJsonReader( const QJsonObject &obj, const SvJsonReader& ) : mObject(obj) {}

    const QJsonObject &object() const { return mObject; }
  };






//!
//! \brief The SvJsonReaderExt template used when need translate some common param (property) to
//!                            complex reader, for example dataBase version
//!
template<typename SvProperty>
class SvJsonReaderExt : public SvJsonReader
  {
    SvProperty        *mProperty;  //!< Reader property - special object to build full structure
  public:
    //!
    //! \brief SvJsonReader Constructor for reader
    //! \param obj          Object which json readed
    //!
    SvJsonReaderExt( const QJsonObject &obj, SvProperty *prop ) : SvJsonReader(obj), mProperty(prop) {}

    SvJsonReaderExt( const QJsonObject &obj, const SvJsonReaderExt<SvProperty> &src ) : SvJsonReader(obj), mProperty(src.mProperty) {}

    //!
    //! \brief property Returns current property of reader
    //! \return         Property of reader
    //!
    SvProperty *property() { return mProperty; }
  };



//!
//! \brief jsonBool Transfer bool value
//! \param key      Key for value
//! \param b        Bool value
//! \param def      Default value of param when error while reading occur
//!
inline void svJsonBool( const SvJsonReader &js, const QString &key, bool &b ) { b = js.object().value( key ).toBool(); }
inline void svJsonBool( const SvJsonReader &js, const QString &key, bool &b, bool def ) { b = js.object().value( key ).toBool( def ); }




//!
//! \brief jsonInt Transfer int value
//! \param key     Key for value
//! \param v       Int value
//! \param def      Default value of param when error while reading occur
//!
inline void svJsonInt( const SvJsonReader &js, const QString &key, int &v ) { v = js.object().value( key ).toInt(); }
inline void svJsonInt( const SvJsonReader &js, const QString &key, int &v, int def ) { v = js.object().value( key ).toInt( def ); }


//!
//! \brief jsonInt Transfer color value
//! \param key     Key for value
//! \param v       Int value
//! \param def      Default value of param when error while reading occur
//!
inline void svJsonColor( const SvJsonReader &js, const QString &key, QColor &color ) { color = QColor( js.object().value( key ).toString() ); }
inline void svJsonColor( const SvJsonReader &js, const QString &key, QColor &color, QColor def ) { color = QColor( js.object().value( key ).toString( def.name() ) ); }


//!
//! \brief jsonDouble Transfer double value
//! \param key        Key for value
//! \param d          Double value
//! \param def        Default value of param when error while reading occur
//!
inline void svJsonDouble( const SvJsonReader &js, const QString &key, double &d ) { d = js.object().value( key ).toDouble(); }
inline void svJsonDouble( const SvJsonReader &js, const QString &key, double &d, double def ) { d = js.object().value( key ).toDouble( def ); }


//!
//! \brief jsonString Transfer string value
//! \param key        Key for value
//! \param s          String to transfer
//! \param def        Default value of param when error while reading occur
//!
inline void svJsonString( const SvJsonReader &js, const QString &key, QString &s ) { s = js.object().value( key ).toString(); }
inline void svJsonString( const SvJsonReader &js, const QString &key, QString &s, QString def ) { s = js.object().value( key ).toString( def ); }


//!
//! \brief jsonPoint Transfer point value
//! \param key       Key for value
//! \param p         Point to transfer
//!
inline void svJsonPoint( const SvJsonReader &js, const QString &key, QPoint &p ) {
  SvJsonReader sjs( js.object().value( key ).toObject() );
  svJsonInt( sjs, "x", p.rx() );
  svJsonInt( sjs, "y", p.ry() );
  }


//!
//! \brief jsonListInt Transfer list of int values
//! \param key         Key for list
//! \param list        List to transfer
//!
inline void svJsonListInt( const SvJsonReader &js, const QString &key, QList<int> &list )
  {
  list.clear();
  QJsonArray ar = js.object().value( key ).toArray();
  list.reserve( ar.count() );
  for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
    int v = i->toInt();
    list.append( v );
    }
  }



//!
//! \brief jsonListInt Transfer list of double values
//! \param key         Key for list
//! \param list        List to transfer
//!
inline void svJsonListDouble( const SvJsonReader &js, const QString &key, QList<double> &list )
  {
  list.clear();
  QJsonArray ar = js.object().value( key ).toArray();
  list.reserve( ar.count() );
  for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
    double v = i->toDouble();
    list.append( v );
    }
  }



//!
//! \brief jsonListString Transfer list of string values
//! \param key            Key for list
//! \param list           List to transfer
//!
inline void svJsonListString( const SvJsonReader &js, const QString &key, QStringList &list )
  {
  list.clear();
  QJsonArray ar = js.object().value( key ).toArray();
  list.reserve( ar.count() );
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
template<typename SvClass, typename SvJsonReaderImpl>
void svJsonList( const SvJsonReaderImpl &js, const QString &key, QList<SvClass> &list )
  {
  list.clear();
  QJsonArray ar = js.object().value( key ).toArray();
  list.reserve( ar.count() );
  for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
    SvClass item;
    SvJsonReader sjs( i->toObject(), js );
    item.jsonRead( sjs );
    list.append( item );
    }
  }











//!
//! \brief jsonListPtr Template Transfer list of pointers to any value
//!                    Value class must contains jsonRead method, which builds
//!                    object from json. To build Value object Value class must
//!                    contains static build( Sv
//! \param key         Key for list
//! \param list        List to transfer
//!
template<typename SvClass, typename SvJsonReaderImpl>
void svJsonListPtr( const SvJsonReaderImpl &js, const QString &key, QList<SvClass*> &list )
  {
  qDeleteAll(list);
  list.clear();
  QJsonArray ar = js.object().value( key ).toArray();
  list.reserve( ar.count() );
  for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
    SvJsonReaderImpl sjs( i->toObject(), js );
    SvClass *item = SvClass::build( sjs );
    if( item ) {
      item->jsonRead( sjs );
      list.append( item );
      }
    }
  }



//!
//! \brief jsonListPtr Template Transfer list of pointers to any value
//!                    Value class must contains jsonRead method, which builds
//!                    object from json. To build Value object Value class must
//!                    contains static SvClass *build( SvJsonReader &js )
//! \param key         Key for list
//! \param list        List to transfer
//! \param prefixFun   Prefix function. It call for each item in list and if it return true then
//!                    item written into json in other case nothing done
//!
template<typename SvClass, typename SvJsonReaderImpl>
void svJsonListPtr( const SvJsonReaderImpl &js, const QString &key, const QList<SvClass*> &list, std::function<bool(SvClass*)> suffixFun )
  {
  qDeleteAll(list);
  list.clear();
  QJsonArray ar = js.object().value( key ).toArray();
  list.reserve( ar.count() );
  for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
    SvJsonReaderImpl sjs( i->toObject(), js );
    SvClass *item = SvClass::build( sjs );
    if( item ) {
      item->jsonRead( sjs );
      if( suffixFun(item) )
        list.append( item );
      }
    }
  }



//!
//! \brief jsonMapString Transfer map of strings
//! \param key           Key for map
//! \param map           Map to transfer
//!
inline void svJsonMapString( const SvJsonReader &js, const QString &key, QMap<QString,QString> &map )
  {
  map.clear();
  QJsonObject obj = js.object().value( key ).toObject();
  for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
    QString str = i.value().toString();
    map.insert( i.key(), str );
    }
  }



//!
//! \brief jsonMapInt Transfer map of int
//! \param key        Key for map
//! \param map        Map to transfer
//!
inline void svJsonMapInt( const SvJsonReader &js, const QString &key, QMap<QString,int> &map )
  {
  map.clear();
  QJsonObject obj = js.object().value( key ).toObject();
  for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
    int v = i.value().toInt();
    map.insert( i.key(), v );
    }
  }


//!
//! \brief jsonMap Template transfer map of any values with QString as key
//!                Value class must contains jsonRead method, which builds
//!                object from json
//! \param key     Key for map
//! \param list    Map to transfer
//!
template<typename SvClass, typename SvJsonReaderImpl>
void svJsonMap( const SvJsonReaderImpl &js, const QString &key, QMap<QString,SvClass> &map )
  {
  map.clear();
  QJsonObject obj = js.object().value( key ).toObject();
  for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
    SvClass item;
    SvJsonReader sjs( i.value().toObject(), js );
    item.jsonRead( sjs );
    map.insert( i.key(), item );
    }
  }




//!
//! \brief jsonMapList Template transfer map of list of any values with QString
//!                    as key of map. Value class must contains jsonWrite method,
//!                    which do writing into SvJsonWriter object
//! \param key         Key for map
//! \param map         Map of list of object to transfer
//!
template<typename SvClass, typename SvJsonReaderImpl>
void svJsonMapList( const SvJsonReaderImpl &js, const QString &key, const QMap<QString, QList<SvClass> > &map )
  {
  map.clear();
  QJsonObject obj = js.object().value( key ).toObject();
  for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
    QJsonArray ar = i.value().toArray();
    QList<SvClass> list;
    list.reserve( ar.count() );
    for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
      SvClass item;
      SvJsonReader sjs( i->toObject(), js );
      item.jsonRead( sjs );
      list.append( item );
      }
    map.insert( i.key(), list );
    }
  }



//!
//! \brief jsonMapPtr Template transfer map of any values with QString as key
//!                   Value class must contains buildFromJson method, which builds
//!                   object from json and read it
//! \param key        Key for map
//! \param list       Map to transfer
//!
template<typename SvClass, typename SvJsonReaderImpl>
void svJsonMapPtr( const SvJsonReaderImpl &js, const QString &key, QMap<QString,SvClass*> &map )
  {
  qDeleteAll(map);
  map.clear();
  QJsonObject obj = js.object().value( key ).toObject();
  for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
    SvJsonReader sjs( i.value().toObject(), js );
    SvClass *item = SvClass::buildFromJson( sjs );
    if( item ) {
      map.insert( i.key(), item );
      }
    }
  }


//!
//! \brief jsonObject Template transfer any value as json object
//!                   Value class must contains jsonRead method, which builds
//!                   object from json
//! \param key        Key for object
//! \param obj        Object to transfer
//!
template<typename SvClass, typename SvJsonReaderImpl>
void svJsonObject( const QString &key, SvClass &obj )
  {
  SvJsonReader js( js.object().value( key ).toObject() );
  obj.jsonRead( js );
  }


//!
//! \brief jsonObjectPtr Template transfer any value as json object
//!                      Value class must contains jsonRead method, which builds
//!                      object from json
//! \param key           Key for object
//! \param objPtr        Object pointer to transfer
//!
template<typename SvClass, typename SvJsonReaderImpl>
void svJsonObjectPtr( const QString &key, SvClass *objPtr )
  {
  SvJsonReader js( js.object().value( key ).toObject() );
  objPtr->jsonRead( js );
  }


//!
//! \brief jsonValue Template transfer any value as json value
//!                  Value class must contains jsonRead method which
//!                  reads object value from json object
//! \param key       Key for value
//! \param val       Object value to transfer
//!
template<typename SvClass>
inline void svJsonValue( const SvJsonReader &js, const QString &key, SvClass &val )
  {
  val.json( js, key );
  }



#ifdef AAA




//!
//! \brief The SvJsonReader class Unificate json io class, through which json readen
//!
class SvJsonReader
  {
  protected:
    const QJsonObject &mObject;    //!< JSON object reference from which will readed
  public:
    //!
    //! \brief SvJsonReader Constructor for reader
    //! \param obj          Object which json readed
    //!
    SvJsonReader( const QJsonObject &obj ) : mObject(obj) {}

    const QJsonObject &object() const { return mObject; }
  };






//!
//! \brief The SvJsonReaderExt template used when need translate some common param (property) to
//!                            complex reader, for example dataBase version
//!
template<typename SvProperty>
class SvJsonReaderExt : public SvJsonReader
  {
    SvProperty        *mProperty;  //!< Reader property - special object to build full structure
  public:
    //!
    //! \brief SvJsonReader Constructor for reader
    //! \param obj          Object which json readed
    //!
    SvJsonReaderExt( const QJsonObject &obj, SvProperty *prop ) : SvJsonReader(obj), mProperty(prop) {}

    //!
    //! \brief property Returns current property of reader
    //! \return         Property of reader
    //!
    SvProperty *property() { return mProperty; }

    //!
    //! \brief jsonList Template Transfer list of any values
    //!                 Value class must contains jsonRead method, which builds
    //!                 object from json
    //! \param key      Key for list
    //! \param list     List to transfer
    //!
    template<typename SvClass>
    void jsonList( const QString &key, QList<SvClass> &list )
      {
      list.clear();
      QJsonArray ar = mObject.value( key ).toArray();
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        SvClass item;
        SvJsonReaderExt<SvProperty> js( i->toObject(), mProperty );
        item.jsonRead( js );
        list.append( item );
        }
      }


    //!
    //! \brief jsonListPtr Template Transfer list of pointers to any value
    //!                    Value class must contains jsonRead method, which builds
    //!                    object from json. To build Value object Value class must
    //!                    contains static build( Sv
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    template<typename SvClass>
    void jsonListPtr( const QString &key, QList<SvClass*> &list )
      {
      qDeleteAll(list);
      list.clear();
      QJsonArray ar = mObject.value( key ).toArray();
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        SvJsonReaderExt<SvProperty> js( i->toObject(), mProperty );
        SvClass *item = SvClass::build( js );
        if( item ) {
          item->jsonRead( js );
          list.append( item );
          }
        }
      }


    //!
    //! \brief jsonListPtr Template Transfer list of pointers to any value
    //!                    Value class must contains jsonRead method, which builds
    //!                    object from json. To build Value object Value class must
    //!                    contains static SvClass *build( SvJsonReader &js )
    //! \param key         Key for list
    //! \param list        List to transfer
    //! \param prefixFun   Prefix function. It call for each item in list and if it return true then
    //!                    item written into json in other case nothing done
    //!
    template<typename SvClass>
    void jsonListPtr( const QString &key, const QList<SvClass*> &list, std::function<bool(SvClass*)> suffixFun )
      {
      qDeleteAll(list);
      list.clear();
      QJsonArray ar = mObject.value( key ).toArray();
      list.reserve( ar.count() );
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        SvJsonReaderExt<SvProperty> js( i->toObject(), mProperty );
        SvClass *item = SvClass::build( js );
        if( item ) {
          item->jsonRead( js );
          if( suffixFun(item) )
            list.append( item );
          }
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
    void jsonMap( const QString &key, QMap<QString,SvClass> &map )
      {
      map.clear();
      QJsonObject obj = mObject.value( key ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        SvClass item;
        SvJsonReaderExt<SvProperty> js( i.value().toObject(), mProperty );
        item.jsonRead( js );
        map.insert( i.key(), item );
        }
      }




    //!
    //! \brief jsonMapList Template transfer map of list of any values with QString
    //!                    as key of map. Value class must contains jsonWrite method,
    //!                    which do writing into SvJsonWriter object
    //! \param key         Key for map
    //! \param map         Map of list of object to transfer
    //!
    template <typename SvClass>
    void jsonMapList( const QString &key, QMap<QString, QList<SvClass> > &map )
      {
      map.clear();
      QJsonObject obj = mObject.value( key ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        QJsonArray ar = i.value().toArray();
        QList<SvClass> list;
        list.reserve( ar.count() );
        for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
          SvClass item;
          SvJsonReaderExt<SvProperty> js( i->toObject(), mProperty );
          item.jsonRead( js );
          list.append( item );
          }
        map.insert( i.key(), list );
        }
      }


    //!
    //! \brief jsonMapPtr Template transfer map of any values with QString as key
    //!                   Value class must contains jsonRead method, which builds
    //!                   object from json
    //! \param key        Key for map
    //! \param list       Map to transfer
    //!
    template<typename SvClass>
    void jsonMapPtr( const QString &key, QMap<QString,SvClass*> &map )
      {
      qDeleteAll(map);
      map.clear();
      QJsonObject obj = mObject.value( key ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        SvJsonReaderExt<SvProperty> js( i.value().toObject(), mProperty );
        SvClass *item = SvClass::build( js );
        if( item ) {
          item->jsonRead( js );
          map.insert( i.key(), item );
          }
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
    void jsonObject( const QString &key, SvClass &obj )
      {
      SvJsonReaderExt<SvProperty> js( mObject.value( key ).toObject(), mProperty );
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
    void jsonObjectPtr( const QString &key, SvClass *objPtr )
      {
      SvJsonReaderExt<SvProperty> js( mObject.value( key ).toObject(), mProperty );
      objPtr->jsonRead( js );
      }


    //!
    //! \brief jsonValue Template transfer any value as json value
    //!                  Value class must contains jsonRead method which
    //!                  reads object value from json object
    //! \param key       Key for value
    //! \param val       Object value to transfer
    //!
    template<typename SvClass>
    void jsonValue( const QString &key, SvClass &val )
      {
      val.jsonRead( key, *this );
      }
  };
#endif


//!
//! \brief The SvJsonReaderExtInt class Unificate json io class, through which json readed
//!
using SvJsonReaderExtInt = SvJsonReaderExt<int>;


inline QByteArray svJsonObjectToByteArray( const QJsonObject &obj )
  {
  return QJsonDocument(obj).toJson();
  }


inline QJsonObject svJsonObjectFromByteArray( const QByteArray &ar )
  {
  return QJsonDocument::fromJson(ar).object();
  }

#endif // SVJSONIO_H
