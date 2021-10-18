package salilab.saliscore;

import java.util.Collections;
import java.util.List;
import java.util.Set;
import java.util.TreeMap;

public class CsChordKit extends CsJsonWritable {
  private TreeMap<String,CsChordLine> mChordMap; //!< Chord line variations map

  CsChordKit() {
    mChordMap = new TreeMap<>();
  }

  CsChordKit( CsChordKit kit ) {
    mChordMap = new TreeMap<>();
    Set<String> keys = kit.mChordMap.keySet();
    for( String key : keys )
      mChordMap.put( key, (CsChordLine) kit.mChordMap.get(key).copy() );
  }

  //auto &chordMapConst() const { return mChordMap; }

  //public CsChordLine  chordLineGet( String part ) { return mChordMap.get(part); }

  //public void         chordLineSet( String part, CsChordLine line ) { mChordMap.put( part, line ); }

  public CsChordList  chordListGet( String part ) { return mChordMap.get(part).chordListConst(); }

  public void         chordListSet( String part, CsChordList list ) { mChordMap.put( part, new CsChordLine( list ) ); }

  public void         chordRename( String prevPart, String newPart ) {
    mChordMap.put( newPart, mChordMap.get(prevPart) );
    mChordMap.remove( prevPart );
  }

  public void         chordRemove( String part ) { mChordMap.remove( part ); }

  @Override
  public void jsonWrite( SvJsonWriter js ) throws Exception {
    js.jsonMap( "ChordMap", mChordMap, CsChordLine.class );
  }

  @Override
  public void jsonRead( SvJsonReader js ) throws Exception {
    js.jsonMap( "ChordMap", mChordMap, CsChordLine.class );
  }

  @Override
  public CsJsonWritable copy() {
    return new CsChordKit( this );
  }
}
