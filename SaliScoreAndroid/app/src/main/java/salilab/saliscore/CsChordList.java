package salilab.saliscore;

import java.util.Vector;

public class CsChordList extends QList<CsChord> {
  public CsChordList copy() {
    CsChordList dst = new CsChordList();
    copyTo( dst );
    return dst;
  }
}
