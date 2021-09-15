#include "CsCursorEditLyric.h"
#include "score/CsComposition.h"

#include <QClipboard>
#include <QApplication>


CsCursorEditLyric::CsCursorEditLyric(int lineIndex, CsComposition &comp) :
  CsCursorEdit( comp )
  {
  mClass = cccLyric;
  mLineIndex = lineIndex;

  //Get lyric line
  mLyricLine = mComposition.lyricGet( mLineIndex );

  mStartSel = mStopSel = mCharPos = mLyricLine.count();
  }





//!
//! \brief movePos Move cursor (insert) position
//! \param pos     New position of cursor
//! \param sel     If true then text from current position to pos is make selected
//!
void CsCursorEditLyric::movePos(int pos, bool sel)
  {
  //Check bounds of new position
  //Correct if need
  pos = qBound( 0, pos, mLyricLine.count() );
  if( sel ) {
    //Need selection
    if( isSelectionPresent() ) {
      //Selection already presents
      if( mCharPos == mStartSel ) {
        mCharPos = pos;
        if( mCharPos > mStopSel ) {
          mStartSel = mStopSel;
          mStopSel = mCharPos;
          }
        else mStartSel = mCharPos;
        }
      else {
        mCharPos = pos;
        if( mCharPos < mStartSel ) {
          mStopSel = mStartSel;
          mStartSel = mCharPos;
          }
        else mStopSel = mCharPos;
        }
      }
    else {
      mCharPos = pos;
      if( mCharPos > mStartSel ) mStopSel = mCharPos;
      else mStartSel = mCharPos;
      }
    }
  else {
    mStartSel = mStopSel = mCharPos = pos;
    }
  }




//!
//! \brief setSel Define selection
//! \param start  Start position of selection
//! \param stop   Stop position of selection
//!
void CsCursorEditLyric::setSel(int start, int stop)
  {
  if( start >= 0 && start <= mLyricLine.count() && stop >= start && stop <= mLyricLine.count() ) {
    mStartSel = start;
    mCharPos = mStopSel = stop;
    }
  }





//!
//! \brief insertText Insert text in current position with selection param
//! \param str        Text to insert
//! \param sel        If true then inserted text is make selected
//!
void CsCursorEditLyric::insertText(const QString str, bool sel)
  {
  //If selection present - delete it
  if( isSelectionPresent() )
    delSelected();

  if( !str.isEmpty() ) {
    //Make insertion
    for( auto ch : str )
      mLyricLine.insert( mCharPos++, CsLyricSymbol( ch ) );

    //End of selection in any chois is equal position after insertion
    mStopSel = mCharPos;

    //If no selection, then startSel equal stopSel
    if( !sel )
      mStartSel = mStopSel;
    }
  }




//!
//! \brief setText Clear old text and insert new one with selection param
//! \param str     Text to set
//! \param sel     If true then inserted text is make selected
//!
void CsCursorEditLyric::setText(const QString str, bool sel)
  {
  mLyricLine.clear();

  mCharPos = mStopSel = mStartSel = 0;

  //Insert new text with selection param
  insertText( str, sel );
  }





//!
//! \brief delSelected Delete selection if present
//!
void CsCursorEditLyric::delSelected()
  {
  //Check if selection present
  if( isSelectionPresent() ) {
    //Remove selection
    for( int i = 0; i < (mStopSel - mStartSel); i++ )
      mLyricLine.removeAt( mStartSel );
    mCharPos = mStopSel = mStartSel;
    }
  }




void CsCursorEditLyric::insertAlign(int align)
  {
  if( mCharPos && mLyricLine.at(mCharPos-1).isAlign() )
    mLyricLine[mCharPos-1].alignSet( mLyricLine[mCharPos-1].align() + align );
  else if( mCharPos < mLyricLine.count() && mLyricLine.at(mCharPos).isAlign() )
    mLyricLine[mCharPos].alignSet( mLyricLine[mCharPos].align() + align );
  else if( align > 0 )
    mLyricLine.insert( mCharPos, CsLyricSymbol(align) );
  }




void CsCursorEditLyric::keyPress(int key, QChar ch, CsCursorEditPtr &ptr)
  {
  if( mControl ) {
    switch( key ) {
      case Qt::Key_0 :
        insertAlign( 512 );
        return;
      case Qt::Key_1 : //1/1
        insertAlign( 256 );
        return;
      case Qt::Key_2 : //1/2
        insertAlign( 128 );
        return;
      case Qt::Key_4 : //1/4
        insertAlign( 64 );
        return;
      case Qt::Key_8 : //1/8
        insertAlign( 32 );
        return;
      case Qt::Key_Right :
      case Qt::Key_6 : //1/16
        insertAlign( 16 );
        return;
      case Qt::Key_Left :
        insertAlign( -16 );
        return;
      }
    }

  switch( key ) {
    case Qt::Key_Shift :
    case Qt::Key_Control :
    case Qt::Key_Escape :
    case Qt::Key_Return :
    case Qt::Key_Enter :
      CsCursorEdit::keyPress( key, ch, ptr );
      break;
    case Qt::Key_Tab : // 1/4
      insertAlign( 64 );
      break;
    case Qt::Key_Left :
      movePos( mCharPos - 1, mShift );
      break;
    case Qt::Key_Right :
      movePos( mCharPos + 1, mShift );
      break;
    case Qt::Key_Home :
      movePos( 0, mShift );
      break;
    case Qt::Key_End :
      movePos( mLyricLine.count(), mShift );
      break;
    case Qt::Key_Backspace :
      if( isSelectionPresent() )
        delSelected();
      movePos( mCharPos - 1, true );
      delSelected();
      break;
    case Qt::Key_Delete :
      if( mShift )
        clipboardCut();
      else {
        if( isSelectionPresent() )
          delSelected();
        movePos( mCharPos + 1, true );
        delSelected();
        }
      break;
    case Qt::Key_Insert :
      if( mShift ) clipboardPaste();
      else if( mControl ) clipboardCopy();
      break;
    default :
      if( ch.isPrint() ) {
        //Insert character
        if( isSelectionPresent() )
          delSelected();
        mLyricLine.insert( mCharPos, CsLyricSymbol(ch) );
        mCharPos++;
        mStartSel = mStopSel = mCharPos;
        }
      break;
    }

  /*
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
  */
  }





int CsCursorEditLyric::duration() const
  {
  return 0;
  }




void CsCursorEditLyric::apply()
  {
  //Update lyric line in composition
  mComposition.lyricSet( mLineIndex, mLyricLine );
  }




bool CsCursorEditLyric::isSelectionPresent() const
  {
  return mStartSel != mStopSel;
  }




void CsCursorEditLyric::clipboardCut()
  {
  clipboardCopy();
  delSelected();
  }



void CsCursorEditLyric::clipboardPaste()
  {
  if( isSelectionPresent() )
    delSelected();

  QClipboard *clip = QApplication::clipboard();
  if( !clip->text().isEmpty() ) {
    CsLyricLine line = lyricLineFromString( clip->text() );
    for( int i = 0; i < line.count(); i++ )
      mLyricLine.insert( mCharPos++, line.at(i) );
    mStartSel = mStopSel = mCharPos;
    }
  }




void CsCursorEditLyric::clipboardCopy()
  {
  if( isSelectionPresent() ) {
    QClipboard *clip = QApplication::clipboard();
    CsLyricLine line;
    line.reserve( mStopSel - mStartSel );
    for( int i = mStartSel; i < mStopSel; i++ )
      line.append( mLyricLine.at(i) );
    clip->setText( lyricLineToString( line )  );
    }
  }








