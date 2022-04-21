#include "CsVisualScoreEdit.h"
#include "CsPainterEditor.h"

#include <QPaintEvent>
#include <QPainter>
#include <QSettings>
#include <QScrollBar>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>


CsVisualScoreEdit::CsVisualScoreEdit(CsComposition &comp, QWidget *parent) :
  CsVisualScore( comp, parent ),
  mCellCursor( comp ),
  mEditor(nullptr),
  mShift(false),
  mControl(false)
  {

  }


void CsVisualScoreEdit::contentPaint(QPainter &painter)
  {
  CsPainterEditor cp( &painter, QStringLiteral(KEY_EDITOR_SETTINGS), mComposition, nullptr, mStartX, size(), &mCellCursor, mEditor, mSelectedLines );

  paintScore( cp );

  //Update editor's reference list
  mReferenceList = cp.referenceList();
  }



void CsVisualScoreEdit::contentClicked(int x, int y)
  {
  if( mEditor != nullptr ) {
    //If actived editor, then apply changes and finish editing
    mEditor->apply();
    delete mEditor;
    mEditor = nullptr;
    }

  QPoint pos(x,y);
  for( int i = mReferenceList.count() - 1; i >= 0; i-- )
    if( mReferenceList.at(i).isHit( pos ) ) {
      const CsReference &ref( mReferenceList.at(i) );

      switch( ref.type() ) {
        case cccAttribute :
          mCellCursor.jump( ref.type(), ref.part() );
          break;

        case cccRemark :
        case cccChord :
        case cccNote :
        case cccLyric :
        case cccTranslation :
          mCellCursor.jump( ref.type(), ref.index(), ref.line(), ref.part() );
          break;

        case cccLyricSymbol :
          if( mEditor == nullptr ) {
            mCellCursor.jump( cccLyric, 0, ref.line(), ref.part() );
            mEditor = CsCursorEdit::build( cccLyricSymbol, ref.line(), ref.index(), ref.part(), mComposition );
            mSelectedLines.clear();
            }
          break;

        case cccLineSelect :
          if( mSelectedLines.contains( ref.line() ) )
            mSelectedLines.remove( ref.line() );
          else
            mSelectedLines.insert( ref.line() );
          //TODO enable edition command
//          CsWinMain::actionEditCopy->setEnabled( mSelectedLines.count() != 0 );
//          CsWinMain::actionEditCut->setEnabled( mSelectedLines.count() != 0 );
//          CsWinMain::actionEditDelete->setEnabled( mSelectedLines.count() != 0 );
          break;
        }
      update();
      return;
      }
  }




void CsVisualScoreEdit::keyPressEvent(QKeyEvent *event)
  {
  int key = event->key();
  QChar ch;
  if( !event->text().isEmpty() )
    ch = event->text().at(0);

  if( mEditor != nullptr ) {
    //Handle key press by editor

    //Detect special cases of key: key up and key down
    switch( key ) {
      case Qt::Key_Up :
        mEditor->keyPress( key, ch, mEditor );
        if( mEditor == nullptr )
          mCellCursor.move( ccoUp );
        break;

      case Qt::Key_Down :
        mEditor->keyPress( key, ch, mEditor );
        if( mEditor == nullptr )
          mCellCursor.move( ccoDown );
        break;

      case Qt::Key_Right :
        mEditor->keyPress( key, ch, mEditor );
        if( mEditor == nullptr )
          mCellCursor.move( ccoRight );
        break;

      case Qt::Key_Left :
        mEditor->keyPress( key, ch, mEditor );
        if( mEditor == nullptr )
          mCellCursor.move( ccoLeft );
        break;

      default:
        mEditor->keyPress( key, ch, mEditor );
        break;
      }
    }

  else {
    //Handle key press by window
    switch( key ) {
      case Qt::Key_Shift :
        mShift = true;
        break;

      case Qt::Key_Control :
        mControl = true;
        break;

      case Qt::Key_Up :
        mCellCursor.move( ccoUp );
        break;

      case Qt::Key_Down :
        mCellCursor.move( ccoDown );
        break;

      case Qt::Key_Left :
        keyLeft();
        break;

      case Qt::Key_Right :
        keyRight();
        break;

      case Qt::Key_Return :
        mEditor = CsCursorEdit::build( mCellCursor, mComposition );
        break;

      case Qt::Key_Insert :
        //Insert new line
        mComposition.lineInsert( qBound( 0, mCellCursor.lineIndex(), mComposition.lineCount() ), false );
        break;

      case Qt::Key_Delete :
        keyDelete();
        break;

      case Qt::Key_Plus :
        keyTakt( true );
        break;

      case Qt::Key_Minus :
        keyTakt( false );
        break;

      case Qt::Key_End :
        keyEnd();
        break;

      case Qt::Key_A :
      case Qt::Key_B :
      case Qt::Key_C :
      case Qt::Key_D :
      case Qt::Key_E :
      case Qt::Key_F :
      case Qt::Key_G :
        if( mCellCursor.isMatchClass( { cccChord, cccNote } ) )
          mEditor = CsCursorEdit::build( mCellCursor, mComposition );

      default:
        if( ch.isPrint() && mCellCursor.isMatchClass( {cccRemark, cccLyric, cccTranslation, cccTitle,   cccVoice,
                                                       cccSinger, cccVoiceDual, cccComposer, cccVoiceLeft,
                                                       cccLyricist, cccStyle, cccAuthor, cccTempo } )  )
            mEditor = CsCursorEdit::build( mCellCursor, mComposition );
        if( mEditor != nullptr )
          mEditor->keyPress( key, ch, mEditor );
        break;
      }

    if( mEditor != nullptr )
      //When actived editor then we dont accept shift and control
      mShift = mControl = false;
    }


  mAutoScroll = true;
  event->accept();
  update();
  }



void CsVisualScoreEdit::keyReleaseEvent(QKeyEvent *event)
  {
  int key = event->key();
  QChar ch;
  if( !event->text().isEmpty() )
    ch = event->text().at(0);

  if( mEditor == nullptr ) {
    //Editor not actived, accept event
    switch( key ) {
      case Qt::Key_Shift :
        mShift = false;
        break;
      case Qt::Key_Control :
        mControl = false;
        break;
      }
    }
  else {
    //All events are translated into editor
    mEditor->keyRelease( key, ch );
    }
  }




//!
//! \brief editCopy Copies selected lines to local clipboard
//!
void CsVisualScoreEdit::editCopy()
  {
  //Copy selected lines to local clipboard
  mLineClipboard.clear();
  for( int line = 0; line < mComposition.lineCount(); line++ )
    if( mSelectedLines.contains(line) )
      mLineClipboard.append( mComposition.line(line) );

  emit actionEditPaste( mLineClipboard.count() != 0 );
  }



