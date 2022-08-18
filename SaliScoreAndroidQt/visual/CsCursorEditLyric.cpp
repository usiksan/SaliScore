#include "CsCursorEditLyric.h"
#include "score/CsComposition.h"

#include <QClipboard>
#include <QApplication>


CsCursorEditLyric::CsCursorEditLyric(int lineIndex, CsComposition &comp, int charIndex) :
  CsCursorEdit( comp )
  {
  mClass = cccLyric;
  mLineIndex = lineIndex;

  //Get lyric line
  mLyricLine = mComposition.lyricGet( mLineIndex );

  mStartSel = mStopSel = mCharPos = mLyricLine.count();

  if( 0 <= charIndex && charIndex < mLyricLine.count() )
    mStartSel = mStopSel = mCharPos = charIndex;
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
  if( mCharPos && mLyricLine.at(mCharPos-1).isAlign() ) {
    mLyricLine[mCharPos-1].alignSet( mLyricLine[mCharPos-1].align() + align );
    qDebug() << "align" << mLyricLine[mCharPos-1].align();
    }
  else if( mCharPos < mLyricLine.count() && mLyricLine.at(mCharPos).isAlign() ) {
    mLyricLine[mCharPos].alignSet( mLyricLine[mCharPos].align() + align );
    qDebug() << "align" << mLyricLine[mCharPos].align();
    }
  else if( align > 0 ) {
    //Find previous align
    int prevAlign = 0;
    for( int i = mCharPos - 1; i >= 0; i-- )
      if( mLyricLine.at(i).isAlign() ) {
        prevAlign = mLyricLine.at(i).align();
        break;
        }
    //Correct position of align to insert
    align = (prevAlign + align) / align * align;
    mLyricLine.insert( mCharPos, CsLyricSymbol(align) );
    qDebug() << "align" << mLyricLine[mCharPos].align();
    mCharPos++;
    }
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
      case Qt::Key_Tab : // 1/4
      case Qt::Key_4 : //1/4
        insertAlign( 64 );
        return;
      case Qt::Key_Right :
      case Qt::Key_8 : //1/8
        insertAlign( 32 );
        return;
      case Qt::Key_6 : //1/16
        insertAlign( 16 );
        return;
      case Qt::Key_Left :
        insertAlign( -32 );
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








