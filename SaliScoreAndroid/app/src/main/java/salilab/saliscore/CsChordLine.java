package salilab.saliscore;

import org.json.JSONException;

import java.util.Collections;
import java.util.Vector;

public class CsChordLine extends CsJsonWritable {
  private CsChordList mList;

  CsChordLine() {
    mList = new CsChordList();
  }
  CsChordLine( CsChordList list ) {
    mList = list.copy();
  }

  CsChordList chordListConst()  { return (CsChordList) Collections.unmodifiableList( mList ); }

  @Override
  public void jsonWrite( SvJsonWriter js ) throws Exception {
    js.jsonList( "ChordList", mList, CsChord.class );
  }

  @Override
  public void jsonRead( SvJsonReader js ) throws Exception {
    js.jsonList( "ChordList", mList, CsChord.class );
  }

  @Override
  public CsJsonWritable copy() {
    return new CsChordLine( mList );
  }
}
