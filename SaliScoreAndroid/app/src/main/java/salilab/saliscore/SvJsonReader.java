package salilab.saliscore;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import android.graphics.Color;

import java.util.Iterator;
import java.util.Map;
import java.util.Vector;
import java.util.TreeMap;

public class SvJsonReader {
  protected JSONObject mObject;    //!< JSON object reference from which will readed

  //!
  //! \brief SvJsonReader Constructor for reader
  //! \param obj          Object which json readed
  //!
  SvJsonReader( JSONObject obj )  { mObject = obj; }

  JSONObject object() { return mObject; }

  //!
  //! \brief jsonBool Transfer bool value
  //! \param key      Key for value
  //! \param b        Bool value
  //!
  public void jsonBool(String key, Boolean b) throws Exception { b =  mObject.getBoolean( key ); }


  //!
  //! \brief jsonInt Transfer int value
  //! \param key     Key for value
  //! \param v       Int value
  //!
  public void jsonInt( String key, Integer v ) throws Exception { v = mObject.getInt( key ); }


  //!
  //! \brief jsonInt Transfer color value
  //! \param key     Key for value
  //! \param v       Int value
  //!
  public void jsonColor( String key, Integer color ) throws Exception { color =  mObject.getInt( key ); }


  //!
  //! \brief jsonDouble Transfer double value
  //! \param key        Key for value
  //! \param d          Double value
  //!
  public void jsonDouble( String key, Double d ) throws Exception { d = mObject.getDouble( key ); }


  //!
  //! \brief jsonString Transfer string value
  //! \param key        Key for value
  //! \param s          String to transfer
  //!
  public void jsonString( String key, String s ) throws Exception { s = mObject.getString( key ); }


  //!
  //! \brief jsonListInt Transfer list of int values
  //! \param key         Key for list
  //! \param list        List to transfer
  //!
  public void jsonListInt( String key, Vector<Integer> list ) throws Exception {
    list.clear();
    JSONArray ar = mObject.getJSONArray( key );
    list.ensureCapacity( ar.length() );
    for( int i = 0; i < ar.length(); i++ )
      list.add( ar.getInt(i) );
  }


  //!
  //! \brief jsonListString Transfer list of string values
  //! \param key            Key for list
  //! \param list           List to transfer
  //!
  public void jsonListString( String key, Vector<String> list ) throws Exception {
    list.clear();
    JSONArray ar = mObject.getJSONArray( key );
    list.ensureCapacity( ar.length() );
    for( int i = 0; i < ar.length(); i++ )
      list.add( ar.getString(i) );
  }


  //!
  //! \brief jsonList Template Transfer list of any values
  //!                 Value class must contains jsonRead method, which builds
  //!                 object from json
  //! \param key      Key for list
  //! \param list     List to transfer
  //!
  public <SvClass extends CsJsonWritable>
  void jsonList(String key, @org.jetbrains.annotations.NotNull Vector<SvClass> list, Class<SvClass> cls ) throws Exception {
    list.clear();
    JSONArray ar = mObject.getJSONArray( key );
    list.ensureCapacity( ar.length() );
    for( int i = 0; i < ar.length(); i++ ) {
      SvClass item = cls.newInstance();
      SvJsonReader js = new SvJsonReader( ar.getJSONObject(i) );
      item.jsonRead( js );
      list.add( item );
    }
  }




  //!
  //! \brief jsonMapString Transfer map of strings
  //! \param key           Key for map
  //! \param map           Map to transfer
  //!
  public void jsonMapString( String key, TreeMap<String,String> map ) throws Exception
  {
    map.clear();
    JSONObject obj = mObject.getJSONObject( key );
    Iterator<String> keys = obj.keys();
    while( keys.hasNext() ) {
      String k = keys.next();
      map.put( k, obj.getString(k) );
    }
  }



  //!
  //! \brief jsonMapInt Transfer map of int
  //! \param key        Key for map
  //! \param map        Map to transfer
  //!
  public void jsonMapInt( String key, TreeMap<String,Integer> map ) throws Exception
  {
    map.clear();
    JSONObject obj = mObject.getJSONObject( key );
    Iterator<String> keys = obj.keys();
    while( keys.hasNext() ) {
      String k = keys.next();
      map.put( k, obj.getInt(k) );
    }
  }


  //!
  //! \brief jsonMap Template transfer map of any values with QString as key
  //!                Value class must contains jsonRead method, which builds
  //!                object from json
  //! \param key     Key for map
  //! \param list    Map to transfer
  //!
  public <SvClass extends CsJsonWritable>
  void jsonMap( String key, Map<String,SvClass> map, Class<SvClass> cls ) throws Exception
  {
    map.clear();
    JSONObject obj = mObject.getJSONObject( key );
    Iterator<String> keys = obj.keys();
    while( keys.hasNext() ) {
      String k = keys.next();
      SvClass item = cls.newInstance();
      SvJsonReader js = new SvJsonReader( obj.getJSONObject(k) );
      item.jsonRead( js );
      map.put( k, item );
    }
  }



  //!
  //! \brief jsonObject Template transfer any value as json object
  //!                   Value class must contains jsonRead method, which builds
  //!                   object from json
  //! \param key        Key for object
  //! \param obj        Object to transfer
  //!
  public <SvClass extends CsJsonWritable>
  void jsonObject( String key, SvClass obj ) throws Exception
  {
    SvJsonReader js = new SvJsonReader( mObject.getJSONObject( key ) );
    obj.jsonRead( js );
  }
}
