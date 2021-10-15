package salilab.saliscore;

import java.util.Vector;

public class CsPosition extends CsJsonWritable {
    protected Integer mPosition; //!< Position in time scale, tick
    protected Integer mDuration; //!< Duration of this position, ticks

    CsPosition( int pos, int duration ) {
      mPosition = pos;
      mDuration = duration;
    }

    public int  position() { return mPosition; }
    public void setPosition( int pos ) { mPosition = pos; }

    public int  duration() { return mDuration; }
    public void durationSet( int d ) { mDuration = d; }

    @Override
    public void jsonWrite( SvJsonWriter js ) throws Exception {
        js.jsonInt( "Duration", mDuration );
        js.jsonInt( "Position", mPosition );
    }

    @Override
    public void jsonRead( SvJsonReader js ) throws Exception {
        js.jsonInt( "Duration", mDuration );
        js.jsonInt( "Position", mPosition );
    }

    @Override
    public CsJsonWritable copy() {
        return new CsPosition( mPosition, mDuration );
    }


    static int csPositionFind(int position, Vector<? extends CsPosition> list )
    {
        for( int i = 0; i < list.size(); i++ )
            if( list.get(i).position() == position )
                return i;
        return -1;
    }

    static <Position extends CsPosition>
    int csPositionInsert( Position p, Vector<Position> list )
    {
        //Find index at which need to be inserted
        int index;
        for( index = 0; index < list.size(); index++ )
            if( list.get(index).position() > p.position() )
                break;

        if( index >= list.size() )
            //Simple append
            list.add( p );
        else
            list.add( index, p );

        return index;
    }
}
