#include "CsCursorEditNote.h"
#include "score/CsComposition.h"

CsCursorEditNote::CsCursorEditNote(int lineIndex, int position, const QString &part, CsComposition &comp) :
  CsCursorEdit( comp )
  {
  mClass     = cccNote;
  mLineIndex = lineIndex;
  mPosition  = position;
  mPartName  = part;

  //Get note line list
  mNoteList = mComposition.noteListGet( mLineIndex, mPartName );

  mNoteIndex = csPositionFind<CsNote>( mPosition, mNoteList );

  if( mNoteIndex < 0 ) {
    //Insert new note
    mNote = CsNote( mPosition, duraQuarter, noteC );

    mNoteIndex = csPositionInsert<CsNote>( mNote, mNoteList );

    //Update composition
    mComposition.noteListSet( mLineIndex, mPartName, mNoteList );

    mIsEdit = false;
    }
  else {
    mNote = mNoteList.at(mNoteIndex);
    mIsEdit = true;
    }
  }




void CsCursorEditNote::keyPress(int key, QChar ch, CsCursorEditPtr &ptr)
  {
  if( ch == QChar('#') ) {
    switch( mNote.note() ) {
      case noteA :
        mNote.noteSet( noteAd );
        break;

      case noteAd :
        mNote.noteSet( noteA );
        break;

      case noteC :
        mNote.noteSet( noteCd );
        break;

      case noteCd :
        mNote.noteSet( noteC );
        break;

      case noteD :
        mNote.noteSet( noteDd );
        break;

      case noteDd :
        mNote.noteSet( noteD );
        break;

      case noteF :
        mNote.noteSet( noteFd );
        break;

      case noteFd :
        mNote.noteSet( noteF );
        break;

      case noteG :
        mNote.noteSet( noteGd );
        break;

      case noteGd :
        mNote.noteSet( noteG );
        break;

      }
    }
  else {
    switch( key ) {
      case Qt::Key_A :
        mNote.noteSet( noteA );
        break;

      case Qt::Key_B :
        mNote.noteSet( noteB );
        break;

      case Qt::Key_C :
        mNote.noteSet( noteC );
        break;

      case Qt::Key_D :
        mNote.noteSet( noteD );
        break;

      case Qt::Key_E :
        mNote.noteSet( noteE );
        break;

      case Qt::Key_F :
        mNote.noteSet( noteF );
        break;

      case Qt::Key_G :
        mNote.noteSet( noteG );
        break;

      case Qt::Key_1 :
        mNote.durationSet( duraHole );
        break;

      case Qt::Key_2 :
        mNote.durationSet( duraHalf );
        break;

      case Qt::Key_4 :
        mNote.durationSet( duraQuarter );
        break;

      case Qt::Key_8 :
        mNote.durationSet( duraEighth );
        break;

      case Qt::Key_6 :
        mNote.durationSet( duraSixteenth );
        break;

      case Qt::Key_3 :
        mNote.durationSet( duraThirtySecond );
        break;

      case Qt::Key_Plus :
        mNote.durationSet( qBound( duraOneHundredTwentyEighth, mNote.duration() * 2, duraBreve ) );
        break;

      case Qt::Key_Minus :
        mNote.durationSet( qBound( duraOneHundredTwentyEighth, mNote.duration() / 2, duraBreve ) );
        break;

      case Qt::Key_Up :
        if( mControl )
          mNote.noteShift( true );
        else
          CsCursorEdit::keyPress( key, ch, ptr );
        break;

      case Qt::Key_Down :
        if( mControl )
          mNote.noteShift( false );
        else
          CsCursorEdit::keyPress( key, ch, ptr );
        break;

      case Qt::Key_PageUp :
        if( mControl )
          mNote.octaveShift( true );
        else
          CsCursorEdit::keyPress( key, ch, ptr );
        break;

      case Qt::Key_PageDown :
        if( mControl )
          mNote.octaveShift( false );
        else
          CsCursorEdit::keyPress( key, ch, ptr );
        break;

      default:
        CsCursorEdit::keyPress( key, ch, ptr );
      }
    }
  }




int CsCursorEditNote::duration() const
  {
  return mNote.duration();
  }





int CsCursorEditNote::noteWhite() const
  {
  return mNote.white();
  }





bool CsCursorEditNote::noteDies() const
  {
  return mNote.isDies();
  }




void CsCursorEditNote::apply()
  {
  mNoteList[mNoteIndex] = mNote;

  mComposition.noteListSet( mLineIndex, mPartName, mNoteList );
  }
