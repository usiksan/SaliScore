package salilab.saliscore;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import android.graphics.Color;

import java.util.Map;
import java.util.Set;
import java.util.Vector;
import java.util.TreeMap;

public class SvJsonWriter {
    private JSONObject mObjectRef;

    //!
    //! \brief SvJsonWriter Constructor with external json object as destignator
    //! \param obj          External destignator json object
    //!
    SvJsonWriter( JSONObject obj ) {
        mObjectRef = obj;
    }


    //!
    //! \brief SvJsonWriter Constructor with internal json object as destignator
    //!
    SvJsonWriter() {
        mObjectRef = new JSONObject();
    }


    //!
    //! \brief object Returns destignator json object
    //! \return       Destignator json object
    //!
    JSONObject object() { return mObjectRef; }


    //!
    //! \brief jsonBool Transfer bool value
    //! \param key      Key for value
    //! \param b        Bool value
    //!
    public void jsonBool(String key, Boolean b) throws Exception { mObjectRef.put( key, b.booleanValue() ); }



    //!
    //! \brief jsonInt Transfer int value
    //! \param key     Key for value
    //! \param v       Int value
    //!
    public void jsonInt( String key, Integer v ) throws Exception { mObjectRef.put( key, v.intValue() ); }



    //!
    //! \brief jsonInt Transfer color value
    //! \param key     Key for value
    //! \param v       Int value
    //!
    public void jsonColor( String key, Integer color ) throws Exception { mObjectRef.put( key, color.intValue() ); }


    //!
    //! \brief jsonDouble Transfer double value
    //! \param key        Key for value
    //! \param d          Double value
    //!
    public void jsonDouble( String key, Double d ) throws Exception { mObjectRef.put( key, d.doubleValue() ); }


    //!
    //! \brief jsonString Transfer string value
    //! \param key        Key for value
    //! \param s          String to transfer
    //!
    public void jsonString( String key, String s ) throws Exception { mObjectRef.put( key, s ); }


    //!
    //! \brief jsonListInt Transfer list of int values
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    public void jsonListInt( String key, Vector<Integer> list ) throws Exception {
        JSONArray ar = new JSONArray();
        for( Integer item : list )
          ar.put( item.intValue() );
        mObjectRef.put( key, ar );
    }


    //!
    //! \brief jsonListString Transfer list of string values
    //! \param key            Key for list
    //! \param list           List to transfer
    //!
    public void jsonListString( String key, Vector<String> list ) throws Exception {
        JSONArray ar = new JSONArray();
        for( String item : list )
            ar.put( item );
        mObjectRef.put( key, ar );
    }




    //!
    //! \brief jsonList Template Transfer list of any values
    //!                 Value class must contains jsonWrite method, which returns
    //!                 json object for value object
    //! \param key      Key for list
    //! \param list     List to transfer
    //!
    public <SvClass extends CsJsonWritable>
    void jsonList(String key, @org.jetbrains.annotations.NotNull Vector<SvClass> list, Class<SvClass> cls ) throws Exception {
        JSONArray ar = new JSONArray();
        for( SvClass item : list ) {
          SvJsonWriter js = new SvJsonWriter();
          item.jsonWrite( js );
          ar.put( js.object() );
        }
        mObjectRef.put( key, ar );
    }




    //!
    //! \brief jsonMapString Transfer map of strings
    //! \param key           Key for map
    //! \param map           Map to transfer
    //!
    public void jsonMapString( String key, TreeMap<String,String> map ) throws Exception
    {
        JSONObject obj = new JSONObject();
        Set<String> keys = map.keySet();
        for( String i : keys )
            obj.put( i, map.get(i) );
        mObjectRef.put( key, obj );
    }


    //!
    //! \brief jsonMapInt Transfer map of int
    //! \param key        Key for map
    //! \param map        Map to transfer
    //!
    public void jsonMapInt( String key, TreeMap<String,Integer> map ) throws Exception
    {
        JSONObject obj = new JSONObject();
        Set<String> keys = map.keySet();
        for( String i : keys )
            obj.put( i, map.get(i) );
        mObjectRef.put( key, obj );
    }


    //!
    //! \brief jsonMap Template transfer map of any values with QString as key
    //!                Value class must contains jsonWrite method, which returns
    //!                json object for value object
    //! \param key     Key for map
    //! \param list    Map to transfer
    //!
    public <SvClass extends CsJsonWritable>
    void jsonMap( String key, Map<String,SvClass> map, Class<SvClass> cls ) throws Exception
    {
        JSONObject obj = new JSONObject();
        Set<String> keys = map.keySet();
        for( String i : keys ) {
            SvJsonWriter js = new SvJsonWriter();
            map.get(i).jsonWrite( js );
            obj.put( i, js.object() );
        }
        mObjectRef.put( key, obj );
    }


    //!
    //! \brief jsonObject Template transfer any value as json object
    //!                   Value class must contains jsonWrite method, which returns
    //!                   json object for value object
    //! \param key        Key for object
    //! \param obj        Object to transfer
    //!
    public <SvClass extends CsJsonWritable>
    void jsonObject( String key, SvClass obj ) throws Exception
    {
        SvJsonWriter js = new SvJsonWriter();
        obj.jsonWrite( js );
        mObjectRef.put( key, js.object() );
    }


}
