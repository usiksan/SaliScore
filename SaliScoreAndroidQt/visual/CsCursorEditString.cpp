#include "CsCursorEditString.h"

#include <QClipboard>
#include <QApplication>

CsCursorEditString::CsCursorEditString(CsComposition &comp) :
  CsCursorEdit(comp)
  {

  }




//!
//! \brief movePos Move cursor (insert) position
//! \param pos     New position of cursor
//! \param sel     If true then text from current position to pos is make selected
//!
void CsCursorEditString::movePos(int pos, bool sel)
  {
  //Check bounds of new position
  //Correct if need
  pos = qBound( 0, pos, mString.length() );
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
void CsCursorEditString::setSel(int start, int stop)
  {
  if( start >= 0 && start <= mString.length() && stop >= start && stop <= mString.length() ) {
    mStartSel = start;
    mCharPos = mStopSel = stop;
    }
  }




void CsCursorEditString::insertText(const QString str, bool sel)
  {
  //If selection present - delete it
  if( isSelectionPresent() )
    delSelected();

  if( !str.isEmpty() ) {
    //Make insertion
    mString.insert( mCharPos, str );

    //End of selection in any chois is equal position after insertion
    mStopSel = mCharPos += str.length();

    //If no selection, then startSel equal stopSel
    if( !sel )
      mStartSel = mStopSel;
    }
  }




void CsCursorEditString::setText(const QString str, bool sel)
  {
  //At first clear old text
  mString.clear();
  mCharPos = mStopSel = mStartSel = 0;

  //Insert new text with selection param
  insertText( str, sel );
  }




//!
//! \brief delSelected Delete selection if present
//!
void CsCursorEditString::delSelected()
  {
  //Check if selection present
  if( isSelectionPresent() ) {
    //Remove selection
    mString.remove( mStartSel, mStopSel - mStartSel );
    mCharPos = mStopSel = mStartSel;
    }
  }




void CsCursorEditString::keyPress(int key, QChar ch, CsCursorEditPtr &ptr)
  {
  switch( key ) {
    case Qt::Key_Shift :
    case Qt::Key_Control :
    case Qt::Key_Escape :
    case Qt::Key_Return :
    case Qt::Key_Enter :
      CsCursorEdit::keyPress( key, ch, ptr );
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
      movePos( mString.length(), mShift );
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
        mString.insert( mCharPos, ch );
        mCharPos++;
        mStartSel = mStopSel = mCharPos;
        }
      break;
    }
  }




QString CsCursorEditString::getStr() const
  {
  return mString;
  }




bool CsCursorEditString::isSelectionPresent() const
  {
  return mStartSel != mStopSel;
  }




int CsCursorEditString::selStart() const
  {
  return mStartSel;
  }



int CsCursorEditString::selStop() const
  {
  return mStopSel;
  }



int CsCursorEditString::charPosition() const
  {
  return mCharPos;
  }





void CsCursorEditString::clipboardCut()
  {
  clipboardCopy();
  delSelected();
  }




void CsCursorEditString::clipboardPaste()
  {
  if( isSelectionPresent() )
    delSelected();

  QClipboard *clip = QApplication::clipboard();
  if( !clip->text().isEmpty() )
    insertText( clip->text(), false );
  }



void CsCursorEditString::clipboardCopy()
  {
  if( isSelectionPresent() ) {
    QClipboard *clip = QApplication::clipboard();
    clip->setText( mString.mid( mStartSel, mStopSel - mStartSel ) );
    }
  }
