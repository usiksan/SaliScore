#include "CsCursorEditChord.h"
#include "score/CsComposition.h"

CsCursorEditChord::CsCursorEditChord(int lineIndex, int position, const QString &part, CsComposition &comp) :
  CsCursorEdit(comp)
  {
  mClass     = cccChord;
  mLineIndex = lineIndex;
  mPosition  = position;
  mPartName  = part;

  mChordList = comp.chordListGet( mLineIndex, mPartName );

  mChordIndex = csPositionFind<CsChord>( mPosition, mChordList );

  if( mChordIndex < 0 ) {
    //This is new chord
    mChord = CsChord( mPosition, comp.stepChord(), noteC, chordMaj );

    mChordIndex = csPositionInsert<CsChord>( mChord, mChordList );

    //Update chord list into composition
    mComposition.chordListSet( mLineIndex, mPartName, mChordList );
    }
  else mChord = mChordList.at(mChordIndex);
  }


void CsCursorEditChord::keyPress(int key, QChar ch, CsCursorEditPtr &ptr)
  {
  if( ch == QChar('#') ) {
    switch( mChord.note() ) {
      case noteA :
        mChord.noteSet( noteAd );
        break;

      case noteAd :
        mChord.noteSet( noteA );
        break;

      case noteC :
        mChord.noteSet( noteCd );
        break;

      case noteCd :
        mChord.noteSet( noteC );
        break;

      case noteD :
        mChord.noteSet( noteDd );
        break;

      case noteDd :
        mChord.noteSet( noteD );
        break;

      case noteF :
        mChord.noteSet( noteFd );
        break;

      case noteFd :
        mChord.noteSet( noteF );
        break;

      case noteG :
        mChord.noteSet( noteGd );
        break;

      case noteGd :
        mChord.noteSet( noteG );
        break;

      }
    }
  else {
    switch( key ) {
      case Qt::Key_A :
        mChord.noteSet( noteA );
        mChord.chordSet( chordMaj );
        break;

      case Qt::Key_B :
        mChord.noteSet( noteB );
        mChord.chordSet( chordMaj );
        break;

      case Qt::Key_C :
        mChord.noteSet( noteC );
        mChord.chordSet( chordMaj );
        break;

      case Qt::Key_D :
        mChord.noteSet( noteD );
        mChord.chordSet( chordMaj );
        break;

      case Qt::Key_E :
        mChord.noteSet( noteE );
        mChord.chordSet( chordMaj );
        break;

      case Qt::Key_F :
        mChord.noteSet( noteF );
        mChord.chordSet( chordMaj );
        break;

      case Qt::Key_G :
        mChord.noteSet( noteG );
        mChord.chordSet( chordMaj );
        break;

      case Qt::Key_M :
        if( mChord.chord() == chordMin )
          mChord.chordSet( chordMaj );
        else if( mChord.chord() == chord7th )
          mChord.chordSet( chordMin7 );
        else if( mChord.chord() == chordMin7 )
          mChord.chordSet( chord7th );
        else
          mChord.chordSet( chordMin );
        break;

      case Qt::Key_7 :
        if( mChord.chord() == chordMin7 )
          mChord.chordSet( chordMin );
        else if( mChord.chord() == chordMin )
          mChord.chordSet( chordMin7 );
        else if( mChord.chord() == chord7th )
          mChord.chordSet( chordMaj );
        else
          mChord.chordSet( chord7th );
        break;


      case Qt::Key_Backspace :
        if( mChord.chord() != chordMaj )
          mChord.chordSet( chordMaj );
        else {
          //Remove chord
          mChordList.removeAt( mChordIndex );
          mComposition.chordListSet( mLineIndex, mPartName, mChordList );
          delete ptr;
          ptr = nullptr;
          return;
          }
        break;

      default:
        CsCursorEdit::keyPress( key, ch, ptr );
      }
    }
  }




QString CsCursorEditChord::getStr() const
  {
  return mChord.chordText();
  }




void CsCursorEditChord::cancel()
  {

  }




void CsCursorEditChord::apply()
  {
  mChordList[mChordIndex] = mChord;
  mComposition.chordListSet( mLineIndex, mPartName, mChordList );
  }
