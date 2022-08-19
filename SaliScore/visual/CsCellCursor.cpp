#include "CsCellCursor.h"
#include "score/CsComposition.h"


QMap<QString,QString> CsCellCursor::mMoveAttrLeft;
QMap<QString,QString> CsCellCursor::mMoveAttrRight;
QMap<QString,QString> CsCellCursor::mMoveAttrTop;
QMap<QString,QString> CsCellCursor::mMoveAttrBot;

CsCellCursor::CsCellCursor(CsComposition &comp) :
  mComposition(comp),
  mActiveNote( 0, duraQuarter, noteC )
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


    mMoveAttrRight.insert( CS_ATTR_NAME, CS_ATTR_NAME );

    mMoveAttrRight.insert( CS_ATTR_SINGER, CS_ATTR_VOICE );
    mMoveAttrRight.insert( CS_ATTR_COMPOSER, CS_ATTR_VOICE_DUAL );
    mMoveAttrRight.insert( CS_ATTR_LYRICIST, CS_ATTR_VOICE_LEFT );
    mMoveAttrRight.insert( CS_ATTR_AUTHOR, CS_ATTR_STYLE );

    mMoveAttrRight.insert( CS_ATTR_VOICE, CS_ATTR_VOICE );
    mMoveAttrRight.insert( CS_ATTR_VOICE_DUAL, CS_ATTR_VOICE_DUAL );
    mMoveAttrRight.insert( CS_ATTR_VOICE_LEFT, CS_ATTR_VOICE_LEFT );
    mMoveAttrRight.insert( CS_ATTR_STYLE, CS_ATTR_STYLE );
    mMoveAttrRight.insert( CS_ATTR_TEMPO, CS_ATTR_TEMPO );


    mMoveAttrTop.insert( CS_ATTR_NAME, CS_ATTR_NAME );

    mMoveAttrTop.insert( CS_ATTR_SINGER, CS_ATTR_NAME );
    mMoveAttrTop.insert( CS_ATTR_COMPOSER, CS_ATTR_SINGER );
    mMoveAttrTop.insert( CS_ATTR_LYRICIST, CS_ATTR_COMPOSER );
    mMoveAttrTop.insert( CS_ATTR_AUTHOR, CS_ATTR_LYRICIST );

    mMoveAttrTop.insert( CS_ATTR_VOICE, CS_ATTR_NAME );
    mMoveAttrTop.insert( CS_ATTR_VOICE_DUAL, CS_ATTR_VOICE );
    mMoveAttrTop.insert( CS_ATTR_VOICE_LEFT, CS_ATTR_VOICE_DUAL );
    mMoveAttrTop.insert( CS_ATTR_STYLE, CS_ATTR_VOICE_LEFT );
    mMoveAttrTop.insert( CS_ATTR_TEMPO, CS_ATTR_STYLE );


    mMoveAttrBot.insert( CS_ATTR_NAME, CS_ATTR_SINGER );

    mMoveAttrBot.insert( CS_ATTR_SINGER, CS_ATTR_COMPOSER );
    mMoveAttrBot.insert( CS_ATTR_COMPOSER, CS_ATTR_LYRICIST );
    mMoveAttrBot.insert( CS_ATTR_LYRICIST, CS_ATTR_AUTHOR );
    //mMoveAttrBot.insert( CS_ATTR_AUTHOR, CS_ATTR_AUTHOR );

    mMoveAttrBot.insert( CS_ATTR_VOICE, CS_ATTR_VOICE_DUAL );
    mMoveAttrBot.insert( CS_ATTR_VOICE_DUAL, CS_ATTR_VOICE_LEFT );
    mMoveAttrBot.insert( CS_ATTR_VOICE_LEFT, CS_ATTR_STYLE );
    mMoveAttrBot.insert( CS_ATTR_STYLE, CS_ATTR_TEMPO );
    //mMoveAttrBot.insert( CS_ATTR_TEMPO, CS_ATTR_STYLE );



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
          mPartName = mComposition.defNextVisible( mPartName, false );
          }
        }
      else {
        if( mComposition.line(mLineIndex).isRemark() ) {
          mClass = cccRemark;
          mPartName.clear();
          mPartName = mComposition.defNextVisible( mPartName, true );
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
        moveTop();
        break;

      case ccoStartLine :
        mLinePosition = 0;
        break;

      case ccoLeft :
        switch( mClass ) {
          default:
          case cccRemark :
          case cccTranslation :
            break;
          case cccChord :
            setPosition( mLinePosition - CS_STEP_CHORD, CS_STEP_CHORD );
            break;
          case cccNote :
            setPosition( mLinePosition - CS_STEP_NOTE, CS_STEP_NOTE );
            break;
          case cccLyric :
            setPosition( mLinePosition - CS_STEP_LYRIC, CS_STEP_LYRIC );
            break;
          case cccAttribute :
            mPartName = mMoveAttrLeft.value( mPartName, CS_ATTR_NAME );
            break;
          }
        break;

      case ccoRight :
        switch( mClass ) {
          default:
          case cccRemark :
          case cccTranslation :
            break;
          case cccChord :
            setPosition( mLinePosition + CS_STEP_CHORD, CS_STEP_CHORD );
            break;
          case cccNote :
            setPosition( mLinePosition + CS_STEP_NOTE, CS_STEP_NOTE );
            break;
          case cccLyric :
            setPosition( mLinePosition + CS_STEP_LYRIC, CS_STEP_LYRIC );
            break;
          case cccAttribute :
            mPartName = mMoveAttrRight.value( mPartName, CS_ATTR_NAME );
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



void CsCellCursor::setPitchDuration(int pitch, int duration)
  {
  mActiveNote.pitchSet( pitch );
  mActiveNote.durationSet( duration );
  }




void CsCellCursor::setPosition(int pos, int step)
  {
  mLinePosition = qBound( 0, (pos / step) * step, mComposition.lineTickCount(mLineIndex) - step );
  }




void CsCellCursor::movePrevPart()
  {
  mPartName = mComposition.defPrevVisible( mPartName, mClass == cccRemark );
  if( mPartName.isEmpty() ) {
    mLineIndex = qBound( -1, mLineIndex - 1, mComposition.lineCount() - 1 );
    if( mLineIndex >= 0 )
      mClass = mComposition.line(mLineIndex).isRemark() ? cccRemark : cccTranslation;
    }
  }




void CsCellCursor::moveNextPart()
  {
  mPartName = mComposition.defNextVisible( mPartName, mClass == cccRemark );
  if( mPartName.isEmpty() ) {
    mLineIndex++;
    if( mLineIndex < mComposition.lineCount() )
      mClass = mComposition.line(mLineIndex).isRemark() ? cccRemark : cccChord;
    }
  }



void CsCellCursor::moveUp()
  {
  if( mClass >= cccRemark ) {
    do
      //Test previous part
      movePrevPart();
    while( mLineIndex >= 0 && mPartName.isEmpty() && mClass != cccLyric );
    if( mLineIndex < 0 ) {
      mClass = cccAttribute;
      mPartName = CS_ATTR_TEMPO;
      }
    }
  else if( mClass == cccAttribute )
    mPartName = mMoveAttrTop.value( mPartName, CS_ATTR_NAME );
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
  else if( mClass == cccAttribute ) {
    if( mPartName == CS_ATTR_AUTHOR || mPartName == CS_ATTR_TEMPO ) {
      if( mComposition.lineCount() != 0 ) {
        mLineIndex = 0;
        mClass = mComposition.line(mLineIndex).isRemark() ? cccRemark : cccChord;
        mPartName.clear();
        moveDown();
        }
      }
    else
      mPartName = mMoveAttrBot.value( mPartName, CS_ATTR_NAME );
    }
  }



void CsCellCursor::normPosition(int step)
  {
  //Bound position
  if( isLineIndexOk() )
    mLinePosition = qBound( 0, (mLinePosition / step) * step, mComposition.lineTickCount(mLineIndex) - step );
  }



bool CsCellCursor::isLineIndexOk() const
  {
  return 0 <= mLineIndex && mLineIndex < mComposition.lineCount();
  }

