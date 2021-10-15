package salilab.saliscore;

import java.util.ArrayList;

public class QList<SvClass extends CsJsonWritable> {
  private class PrivateList<SvClass> {
    ArrayList<SvClass> mData;
    int                mRefCount;

    PrivateList() {
      mData = new ArrayList<>();
      mRefCount = 1;
    }
  }

  private PrivateList<SvClass> mList;

  QList() {
    mList = new PrivateList<>();
  }

  QList( QList<SvClass> src ) {
    mList = src.mList;
    mList.mRefCount++;
  }

  private void deepCopy() {
    if( mList.mRefCount > 1 ) {
      PrivateList<SvClass> newList = new PrivateList<>();
      for( SvClass item : mList.mData )
        newList.mData.add( (SvClass)item.copy() );
      mList.mRefCount--;
      mList = newList;
    }
  }

  SvClass at

}
