package salilab.saliscore;

import java.util.ArrayList;

public class QList<SvClass extends CsJsonWritable> extends ArrayList<SvClass> {

  QList() {
    super();
  }

  QList( QList<SvClass> src ) {
    super();
    for( SvClass item : src )
      add( (SvClass)item.copy() );
  }

  SvClass at(int index) { return get(index); }

  void    append( SvClass item ) { add( (SvClass) item.copy() ); }

  void    insert( int index, SvClass item ) { add( index, (SvClass) item.copy() ); }

  //void    remove( int index ) { mList.remove( index ); }

  //void    clear() { mList.clear(); }

  int     count() { return size(); }
}
