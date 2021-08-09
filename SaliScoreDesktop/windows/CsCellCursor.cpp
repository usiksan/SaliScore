#include "CsCellCursor.h"
#include "score/CsComposition.h"

CsCellCursor::CsCellCursor(CsComposition &comp) :
  mComposition(comp),
  mCellClass(cccTitle),
  mPosition(0),
  mLineIndex(-1)
  {
  }

bool CsCellCursor::isCurrent(int cellClass, int lineIndex, const QString &partName) const
  {
  return cellClass == mCellClass && lineIndex == mLineIndex && partName == mPartName;
  }




bool CsCellCursor::isCurrent(int cellClass, int position, int lineIndex, const QString &partName) const
  {
  return cellClass == mCellClass && position == mPosition && lineIndex == mLineIndex && partName == mPartName;
  }




bool CsCellCursor::isCurrent(int cellClass, int position, int lineIndex) const
  {
  return cellClass == mCellClass && position == mPosition && lineIndex == mLineIndex;
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
        mCellClass = cccTitle;
        break;

      case ccoStartLine :
        mPosition = 0;
        break;

      case ccoLeft :
        switch( mCellClass ) {
          case cccRemark :
          case cccTranslation :
            break;
          case cccChord :
            setPosition( mPosition - mComposition.stepChord(), mComposition.stepChord() );
            break;
          case cccNote :
            setPosition( mPosition - mComposition.stepNote(), mComposition.stepNote() );
            break;
          case cccLyric :
            setPosition( mPosition - mComposition.stepLyric(), mComposition.stepLyric() );
            break;
          default:
            mCellClass = qBound<int>( cccTitle, mCellClass - 1, cccRemark );
            break;
          }

        break;

      case ccoRight :
        switch( mCellClass ) {
          case cccRemark :
          case cccTranslation :
            break;
          case cccChord :
            setPosition( mPosition + mComposition.stepChord(), mComposition.stepChord() );
            break;
          case cccNote :
            setPosition( mPosition + mComposition.stepNote(), mComposition.stepNote() );
            break;
          case cccLyric :
            setPosition( mPosition + mComposition.stepLyric(), mComposition.stepLyric() );
            break;
          default:
            mCellClass = qBound<int>( cccTitle, mCellClass + 1, cccRemark );
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




void CsCellCursor::setPosition(int pos, int step)
  {
  mPosition = qBound( 0, (pos / step) * step, mComposition.lineTickCount(mLineIndex) - step );
  }




void CsCellCursor::movePrevPart()
  {
  switch( mCellClass ) {
    case cccRemark :
      mPartName = mComposition.remarkPrevVisible( mPartName );
      if( mPartName.isEmpty() ) {
        mLineIndex--;
        if( mLineIndex >= 0 )
          mCellClass = mComposition.line(mLineIndex).isRemark() ? cccRemark : cccTranslation;
        }
      break;

    case cccChord :
      mPartName = mComposition.chordPrevVisible( mPartName );
      if( mPartName.isEmpty() ) {
        mLineIndex--;
        if( mLineIndex >= 0 )
          mCellClass = mComposition.line(mLineIndex).isRemark() ? cccRemark : cccTranslation;
        }
      break;

    case cccNote :
      mPartName = mComposition.notePrevVisible( mPartName );
      if( mPartName.isEmpty() )
        mCellClass = cccChord;
      break;

    case cccLyric :
      mCellClass = cccNote;
      break;

    case cccTranslation :
      mPartName = mComposition.translationPrevVisible( mPartName );
      if( mPartName.isEmpty() )
        mCellClass = cccLyric;
      break;
    }
  }




void CsCellCursor::moveNextPart()
  {
  switch( mCellClass ) {
    case cccRemark :
      mPartName = mComposition.remarkNextVisible( mPartName );
      if( mPartName.isEmpty() ) {
        mLineIndex++;
        if( mLineIndex < mComposition.lineCount() )
          mCellClass = mComposition.line(mLineIndex).isRemark() ? cccRemark : cccChord;
        }
      break;

    case cccChord :
      mPartName = mComposition.chordPrevVisible( mPartName );
      if( mPartName.isEmpty() )
        mCellClass = cccNote;
      break;

    case cccNote :
      mPartName = mComposition.notePrevVisible( mPartName );
      if( mPartName.isEmpty() )
        mCellClass = cccChord;
      break;

    case cccLyric :
      mCellClass = cccTranslation;
      break;

    case cccTranslation :
      mPartName = mComposition.translationPrevVisible( mPartName );
      if( mPartName.isEmpty() ) {
        mLineIndex++;
        if( mLineIndex < mComposition.lineCount() )
          mCellClass = mComposition.line(mLineIndex).isRemark() ? cccRemark : cccChord;
        }
      break;
    }
  }



void CsCellCursor::moveUp()
  {
  if( mCellClass >= cccRemark ) {
    do
      //Test previous part
      movePrevPart();
    while( mLineIndex >= 0 && mPartName.isEmpty() && mCellClass != cccLyric );
    if( mLineIndex < 0 )
      mCellClass = cccAuthor;
    }
  else
    mCellClass = qBound<int>( cccTitle, mCellClass - 2, cccRemark );
  }




void CsCellCursor::moveDown()
  {
  if( mCellClass >= cccRemark ) {
    do
      moveNextPart();
    while( mLineIndex < mComposition.lineCount() && mPartName.isEmpty() && mCellClass != cccLyric );
    if( mLineIndex >= mComposition.lineCount() ) {
      //This try move down composition
      //move to last position of composition
      moveUp();
      }
    }
  else {
    mCellClass = qBound<int>( cccTitle, mCellClass + 2, cccRemark );
    if( mCellClass == cccRemark ) {
      if( mComposition.lineCount() == 0 )
        //No lines
        mCellClass = cccAuthor;
      else {
        mLineIndex = 0;
        mCellClass = mComposition.line(mLineIndex).isRemark() ? cccRemark : cccChord;
        mPartName.clear();
        moveDown();
        }
      }
    }
  }

