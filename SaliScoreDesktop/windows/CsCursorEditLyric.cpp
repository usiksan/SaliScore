#include "CsCursorEditLyric.h"
#include "score/CsComposition.h"

CsCursorEditLyric::CsCursorEditLyric( int lineIndex, int position, CsComposition &comp) :
  CsCursorEditString( comp )
  {
  mClass = cccLyric;
  mLineIndex = lineIndex;
  mPosition = position;

  //Get lyric line
  mLyricList = mComposition.lyricGet( mLineIndex );

  //Find lyric with position
  mLyricIndex = csPositionFind<CsLyric>( mPosition, mLyricList );

  //Test if we edit existing lyric or insert new one
  if( mLyricIndex < 0 ) {
    //We need insert new lyric
    CsLyric lyric( mPosition, mComposition.stepLyric(), QString{} );

    mLyricIndex = csPositionInsert<CsLyric>( lyric, mLyricList );

    //Update lyric list into composition
    mComposition.lyricSet( mLineIndex, mLyricList );
    mIsEdit = false;
    }
  else mIsEdit = true;

  setText( mLyricList.at(mLyricIndex).lyric(), false );
  }




void CsCursorEditLyric::keyPress(int key, QChar ch, CsCursorEditPtr &ptr)
  {
  if( key == Qt::Key_Backspace && mCharPos == 0 && mLyricIndex ) {
    //Union lyric with previous lyric
    mIsEdit = true;
    mLyricList.removeAt( mLyricIndex );
    mLyricIndex--;
    //Previous lyric
    QString prevLyric = mLyricList.at(mLyricIndex).lyric();
    mPosition = mLyricList.at(mLyricIndex).position();
    setText( prevLyric + mString, false );
    movePos( prevLyric.length(), false );

    //Update lyric list into composition
    mComposition.lyricSet( mLineIndex, mLyricList );
    }

  else if( ch == QChar('*') ) {
    //Split or move current lyric
    if( mCharPos == 0 ) {
      //Move current lyric
      int newPosition = mPosition + mComposition.stepLyric();
      if( mLyricIndex + 1 < mLyricList.count() && mLyricList.at(mLyricIndex+1).position() <= newPosition ) {
        //Need union with next lyric
        mIsEdit = true;

        //Remove current
        mLyricList.removeAt( mLyricIndex );

        //At now next lyric is current, update string
        setText( mString + mLyricList.at(mLyricIndex).lyric(), false );
        movePos( 0, false );

        mPosition = mLyricList.at(mLyricIndex).position();
        }
      else {
        //Simple move on one step
        mPosition = newPosition;
        mLyricList[mLyricIndex].positionSet( newPosition );
        }
      }
    else {
      mIsEdit = true;
      //Split current string
      mLyricList[mLyricIndex].lyricSet( mString.left(mCharPos) );

      //Update second part
      setText( mString.mid( mCharPos ), false );
      movePos( 0, false );

      int newPosition = mPosition + mComposition.stepLyric();
      if( mLyricIndex + 1 < mLyricList.count() && mLyricList.at(mLyricIndex+1).position() <= newPosition ) {
        //Need union with next lyric

        //At now next lyric is current, update string
        setText( mString + mLyricList.at(mLyricIndex).lyric(), false );
        movePos( 0, false );

        mPosition = mLyricList.at(mLyricIndex).position();
        }
      else {
        //Simple insert new lyric
        CsLyric lyric( newPosition, mComposition.stepLyric(), mString );

        mLyricIndex = csPositionInsert<CsLyric>( lyric, mLyricList );

        mPosition = newPosition;
        }
      }

    //Update lyric list into composition
    mComposition.lyricSet( mLineIndex, mLyricList );
    }

  else CsCursorEditString::keyPress( key, ch, ptr );
  }





int CsCursorEditLyric::duration() const
  {
  return 0;
  }




void CsCursorEditLyric::apply()
  {
  //Update lyric list
  if( mString.isEmpty() )
    //Remove lyric with empty lyric
    mLyricList.removeAt( mLyricIndex );
  else
    mLyricList[mLyricIndex].lyricSet( mString );

  //Update lyric line in composition
  mComposition.lyricSet( mLineIndex, mLyricList );
  }




void CsCursorEditLyric::cancel()
  {
  if( !mIsEdit ) {
    //Remove newly inserted lyric
    mLyricList.removeAt( mLyricIndex );

    //Update lyric line in composition
    mComposition.lyricSet( mLineIndex, mLyricList );
    }
  }

