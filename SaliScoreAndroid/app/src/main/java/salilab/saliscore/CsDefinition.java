package salilab.saliscore;

public class CsDefinition extends CsJsonWritable {
  public String  mName;
  public String  mDescription;
  public Boolean mVisible;

  CsDefinition() {
    mName = new String();
    mDescription = new String();
    mVisible = true;
  }

  CsDefinition( CsDefinition df ) {
    mName = df.mName;
    mDescription = df.mDescription;
    mVisible = df.mVisible.booleanValue();
  }

  CsDefinition( String nm, String descr ) {
    mName = nm;
    mDescription = descr;
    mVisible = true;
  }

  boolean visibleToggle() { return mVisible = !mVisible; }

  @Override
  void jsonWrite( SvJsonWriter js ) throws Exception {
    js.jsonString( "Name", mName );
    js.jsonString( "Description", mDescription );
    js.jsonBool( "Visible", mVisible );
  }

  @Override
  void jsonRead( SvJsonReader js ) throws Exception {
    js.jsonString( "Name", mName );
    js.jsonString( "Description", mDescription );
    js.jsonBool( "Visible", mVisible );
  }

  @Override
  CsJsonWritable copy() {
    return new CsDefinition( this );
  }
}
