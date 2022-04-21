#include "CsCellCursor.h"
#include "score/CsComposition.h"


QMap<QString,QString> CsCellCursor::mMoveAttrLeft;
QMap<QString,QString> CsCellCursor::mMoveAttrRight;
QMap<QString,QString> CsCellCursor::mMoveAttrTop;
QMap<QString,QString> CsCellCursor::mMoveAttrBot;

CsCellCursor::CsCellCursor(CsComposition &comp) :
  mComposition(comp)
  {
  moveTop();

  if( mMoveAttrLeft.isEmpty() ) {
    //Build moving map for attributes
    mMoveAttrLeft.insert( CS_ATTR_NAME, CS_ATTR_NAME );

    mMoveAttrLeft.insert( CS_ATTR_SINGER, CS_ATTR_SINGER );
    mMoveAttrLeft.insert( CS_ATTR_COMPOSER, CS_ATTR_COMPOSER );
    mMoveAttrLeft.insert( CS_ATTR_LYRICIST, CS_ATTR_LYRICIST );
    mMoveAttrLeft.insert( CS_ATTR_AUTHOR, CS_ATTR_AUTHOR );

    mMoveAttrLeft.insert( CS_ATTR_VOICE, CS_ATTR_SINGER );
    mMoveAttrLeft.insert( CS_ATTR_VOICE_DUAL, CS_ATTR_COMPOSER );
    mMoveAttrLeft.insert( CS_ATTR_VOICE_LEFT, CS_ATTR_LYRICIST );
    mMoveAttrLeft.insert( CS_ATTR_STYLE, CS_ATTR_AUTHOR );
    mMoveAttrLeft.insert( CS_ATTR_TEMPO, CS_ATTR_TEMPO );



    }
  }


void CsCellCursor::moveTop()
  {
  mClass = cccAttribute;
  mPartName = CS_ATTR_NAME;
  mLinePosition = 0;
  mLineIndex = -1;
  }




void CsCellCursor::updatePosition()
  {
  if( mClass >= cccRemark ) {
    if( mLineIndex >= mComposition.lineCount() ) {
      //This try move down composition
      //move to last position of composition
      mPartName.clear();
      moveUp();
      }
    else {
      if( mClass == cccRemark ) {
        if( !mComposition.line(mLineIndex).isRemark() ) {
          mClass = cccChord;
          mPartName.clear();
          mPartName = mComposition.chordNextVisible( mPartName );
          }
        }
      else {
        if( mComposition.line(mLineIndex).isRemark() ) {
          mClass = cccRemark;
          mPartName.clear();
          mPartName = mComposition.remarkNextVisible( mPartName );
          }
        }
      }
    }
  }





void CsCellCursor::move(CsCellCursorOperation oper, bool doSelect, int n)
  {
  Q_UNUSED(doSelect)
  while( n-- > 0 ) {
    switch( oper ) {
      case ccoNoMove :
      default:
        //Nothing doing
        break;

      case ccoStart :
        mClass = cccTitle;
        break;

      case ccoStartLine :
        mLinePosition = 0;
        break;

      case ccoLeft :
        switch( mClass ) {
          case cccRemark :
          case cccTranslation :
            break;
          case cccChord :
            setPosition( mLinePosition - mComposition.stepChord(), mComposition.stepChord() );
            break;
          case cccNote :
            setPosition( mLinePosition - mComposition.stepNote(), mComposition.stepNote() );
            break;
          case cccLyric :
            setPosition( mLinePosition - mComposition.stepLyric(), mComposition.stepLyric() );
            break;
          default:
            mClass = qBound<int>( cccTitle, mClass - 1, cccRemark );
            break;
          }

        break;

      case ccoRight :
        switch( mClass ) {
          case cccRemark :
          case cccTranslation :
            break;
          case cccChord :
            setPosition( mLinePosition + mComposition.stepChord(), mComposition.stepChord() );
            break;
          case cccNote :
            setPosition( mLinePosition + mComposition.stepNote(), mComposition.stepNote() );
            break;
          case cccLyric :
            setPosition( mLinePosition + mComposition.stepLyric(), mComposition.stepLyric() );
            break;
          default:
            mClass = qBound<int>( cccTitle, mClass + 1, cccRemark );
            break;
          }

        break;

      case ccoUp :
        moveUp();
        break;

      case ccoDown :
        moveDown();
        break;
      }
    }
  }




void CsCellCursor::jump(int aclass, int aposition, int aline, const QString &apart)
  {
  mClass     = aclass;
  mLinePosition  = aposition;
  mLineIndex = aline;
  mPartName  = apart;
  }




void CsCellCursor::setPosition(int pos, int step)
  {
  mLinePosition = qBound( 0, (pos / step) * step, mComposition.lineTickCount(mLineIndex) - step );
  }




void CsCellCursor::movePrevPart()
  {
  switch( mClass ) {
    case cccRemark :
      mPartName = mComposition.remarkPrevVisible( mPartName );
      if( mPartName.isEmpty() ) {
        mLineIndex = qBound( -1, mLineIndex - 1, mComposition.lineCount() - 1 );
        if( mLineIndex >= 0 )
          mClass = mComposition.line(mLineIndex).isRemark() ? cccRemark : cccTranslation;
        }
      break;

    case cccChord :
      mPartName = mComposition.chordPrevVisible( mPartName );
      if( mPartName.isEmpty() ) {
        mLineIndex = qBound( -1, mLineIndex - 1, mComposition.lineCount() - 1 );
        if( mLineIndex >= 0 )
          mClass = mComposition.line(mLineIndex).isRemark() ? cccRemark : cccTranslation;
        }
      else normPosition( mComposition.stepChord() );
      break;

    case cccNote :
      mPartName = mComposition.notePrevVisible( mPartName );
      if( mPartName.isEmpty() )
        mClass = cccChord;
      else normPosition( mComposition.stepNote() );
      break;

    case cccLyric :
      mClass = cccNote;
      break;

    case cccTranslation :
      mPartName = mComposition.translationPrevVisible( mPartName );
      if( mPartName.isEmpty() ) {
        mClass = cccLyric;
        normPosition( mComposition.stepLyric() );
        }
      break;
    }
  }




void CsCellCursor::moveNextPart()
  {
  switch( mClass ) {
    case cccRemark :
      mPartName = mComposition.remarkNextVisible( mPartName );
      if( mPartName.isEmpty() ) {
        mLineIndex++;
        if( mLineIndex < mComposition.lineCount() )
          mClass = mComposition.line(mLineIndex).isRemark() ? cccRemark : cccChord;
        }
      break;

    case cccChord :
      mPartName = mComposition.chordNextVisible( mPartName );
      if( mPartName.isEmpty() )
        mClass = cccNote;
      else normPosition( mComposition.stepChord() );
      break;

    case cccNote :
      mPartName = mComposition.noteNextVisible( mPartName );
      if( mPartName.isEmpty() ) {
        mClass = cccLyric;
        normPosition( mComposition.stepLyric() );
        }
      else normPosition( mComposition.stepNote() );
      break;

    case cccLyric :
      mClass = cccTranslation;
      break;

    case cccTranslation :
      mPartName = mComposition.translationNextVisible( mPartName );
      if( mPartName.isEmpty() ) {
        mLineIndex++;
        if( mLineIndex < mComposition.lineCount() )
          mClass = mComposition.line(mLineIndex).isRemark() ? cccRemark : cccChord;
        }
      break;
    }
  }



void CsCellCursor::moveUp()
  {
  if( mClass >= cccRemark ) {
    do
      //Test previous part
      movePrevPart();
    while( mLineIndex >= 0 && mPartName.isEmpty() && mClass != cccLyric );
    if( mLineIndex < 0 )
      mClass = cccAuthor;
    }
  else
    mClass = qBound<int>( cccTitle, mClass - 2, cccRemark );
  }




void CsCellCursor::moveDown()
  {
  if( mClass >= cccRemark ) {
    do
      moveNextPart();
    while( mLineIndex < mComposition.lineCount() && mPartName.isEmpty() && mClass != cccLyric );
    if( mLineIndex >= mComposition.lineCount() ) {
      //This try move down composition
      //move to last position of composition
      moveUp();
      }
    }
  else {
    mClass = qBound<int>( cccTitle, mClass + 2, cccRemark );
    if( mClass == cccRemark ) {
      if( mComposition.lineCount() == 0 )
        //No lines
        mClass = cccAuthor;
      else {
        mLineIndex = 0;
        mClass = mComposition.line(mLineIndex).isRemark() ? cccRemark : cccChord;
        mPartName.clear();
        moveDown();
        }
      }
    }
  }



void CsCellCursor::normPosition(int step)
  {
  //Bound position
  mLinePosition = qBound( 0, (mLinePosition / step) * step, mComposition.lineTickCount(mLineIndex) - step );
  }

