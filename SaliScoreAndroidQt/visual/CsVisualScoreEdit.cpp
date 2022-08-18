#include "CsVisualScoreEdit.h"
#include "CsVisualWinMain.h"
#include "CsPainterEditor.h"

#include <QPaintEvent>
#include <QPainter>
#include <QSettings>
#include <QScrollBar>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>


CsVisualScoreEdit::CsVisualScoreEdit(CsComposition &comp, CsVisualWinMain *player, QWidget *parent) :
  CsVisualScore( comp, player, parent ),
  mCellCursor( comp ),
  mEditor(nullptr),
  mShift(false),
  mControl(false)
  {

  }

void CsVisualScoreEdit::cmEditUndo()
  {

  }

void CsVisualScoreEdit::cmEditRedo()
  {

  }

void CsVisualScoreEdit::cmEditCut()
  {
  editCopy();
  cmEditDelete();
  }



void CsVisualScoreEdit::cmEditCopy()
  {
  editCopy();

  //Remove selection
  unselectAll();
  }



void CsVisualScoreEdit::cmEditPaste()
  {

  int insertLine = mCellCursor.lineIndex() >= 0 ? mCellCursor.lineIndex() : 0;

  const auto remarkList = mComposition.remarkDefList();
  const auto chordList  = mComposition.chordDefList();
  const auto noteList   = mComposition.noteDefList();
  const auto translationList = mComposition.translationDefList();

  //Insert lines from local clipboard to current position
  for( int i = 0; i < mLineClipboard.count(); i++ ) {
    //Insert empty line
    mComposition.lineInsert( insertLine, mLineClipboard.at(i).isRemark() );

    //Fill inserted line with information
    //Simple inserting not good idea because beatween copy and paste user can
    // change part listing

    if( mLineClipboard.at(i).isRemark() ) {
      //For remark update all parts
      for( auto const &def : qAsConst(remarkList) )
        mComposition.remarkSet( insertLine, def.mName, mLineClipboard.at(i).remarkGet(def.mName) );
      }
    else {
      //Update chord parts
      for( auto const &def : qAsConst(chordList) )
        mComposition.chordListSet( insertLine, def.mName, mLineClipboard.at(i).chordListGet(def.mName) );

      //Update note parts
      for( auto const &def : qAsConst(noteList) )
        mComposition.noteListSet( insertLine, def.mName, mLineClipboard.at(i).noteListGet(def.mName) );

      //Update lyric
      mComposition.lyricSet( insertLine, mLineClipboard.at(i).lyricGet() );

      //Update translation parts
      for( auto const &def : qAsConst(translationList) )
        mComposition.translationSet( insertLine, def.mName, mLineClipboard.at(i).translationGet(def.mName) );
      }

    //Move to next insert position
    insertLine++;
    }
  update();
  }




void CsVisualScoreEdit::cmEditDelete()
  {
  //Delete all selected lines
  //Because when line deleted all downer lines are shifted up then we must
  //delete lines from end
  for( int line = mComposition.lineCount() - 1; line >= 0; line-- )
    if( mSelectedLines.contains(line) )
      mComposition.lineRemove( line );
  unselectAll();
  }





void CsVisualScoreEdit::cmEditDeleteLine()
  {
  if( (mCellCursor.cellClass() == cccChord) || (mCellCursor.cellClass() == cccNote) || (mCellCursor.cellClass() == cccLyric) ||
      (mCellCursor.cellClass() == cccRemark) ) {
    mComposition.lineRemove( mCellCursor.lineIndex() );
    mCellCursor.updatePosition();
    }
  }



void CsVisualScoreEdit::cmNoteShiftUp()
  {
  if( mCellCursor.isMatch(cccNote) ) {
    if( mEditor == nullptr )
      mCellCursor.noteToneShift(true);
    }
  }

void CsVisualScoreEdit::cmNoteShiftDn()
  {
  if( mCellCursor.isMatch(cccNote) ) {
    if( mEditor == nullptr )
      mCellCursor.noteToneShift(false);
    }
  }

void CsVisualScoreEdit::cmNoteDurationLess()
  {
  if( mCellCursor.isMatch(cccNote) ) {
    if( mEditor == nullptr )
      mCellCursor.noteDurationShift(false);
    }
  }

void CsVisualScoreEdit::cmNoteDurationGrow()
  {
  if( mCellCursor.isMatch(cccNote) ) {
    if( mEditor == nullptr )
      mCellCursor.noteDurationShift(true);
    }
  }




void CsVisualScoreEdit::contentPaint(QPainter &painter)
  {
  CsCursorPosition play( mPlayer->playLinePosition(), mPlayer->playLineIndex() );
  CsPainterEditor cp( &painter, QStringLiteral(KEY_EDITOR_SETTINGS), mComposition, mPlayer->playIsRun() ? &play : nullptr, mStartX, size(), &mCellCursor, mEditor, mSelectedLines );

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
          emit actionEditSelectionPresent( mSelectedLines.count() != 0 );
          break;

        case cccClef : {
          //Toggle clef at part
          auto clef = mComposition.noteClefGet( ref.part() );
          if( clef == noteG ) clef = noteC;
          else if( clef == noteC ) clef = noteF;
          else clef = noteG;
          mComposition.noteClefSet( ref.part(), clef );
          }
          break;

        case cccDenomenator :
        case cccNumerator :
          emit scoreSettings();
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

    //Update active note with editor value
    if( mEditor != nullptr && mEditor->isMatch(cccNote) )
      mCellCursor.setPitchDuration( mEditor->pitch(), mEditor->duration() );

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
        if( mControl )
          keyToneShift( true );
        else
          mCellCursor.move( ccoUp );
        break;

      case Qt::Key_Down :
        if( mControl )
          keyToneShift( false );
        else
          mCellCursor.move( ccoDown );
        break;

      case Qt::Key_Less :
        if( mCellCursor.isMatch(cccNote) )
          mCellCursor.noteDurationShift(false);
        break;

      case Qt::Key_Greater :
        if( mCellCursor.isMatch(cccNote) )
          mCellCursor.noteDurationShift(true);
        break;

      case Qt::Key_Asterisk :
        if( mCellCursor.isMatch(cccNote) )
          mCellCursor.noteDurationPart(true);
        break;

      case Qt::Key_Slash :
        if( mCellCursor.isMatch(cccNote) )
          mCellCursor.noteDurationPart(false);
        break;

      case Qt::Key_Left :
        keyLeft();
        break;

      case Qt::Key_Right :
        keyRight();
        break;

      case Qt::Key_Return :
        if( mControl || mShift )
          editAppendLine( mShift );
        else
          mEditor = CsCursorEdit::build( mCellCursor, mComposition );
        break;

      case Qt::Key_Insert :
        //Insert new line
        editInsertLine( mShift );
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

      case Qt::Key_Space :
        if( mCellCursor.isMatch( cccNote ) )
          insertActiveNote();
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
        if( ch.isPrint() && mCellCursor.isMatchClass( {cccRemark, cccLyric, cccTranslation, cccAttribute } )  )
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




void CsVisualScoreEdit::mousePressEvent(QMouseEvent *event)
  {
  if( event->button() == Qt::MiddleButton && mCellCursor.isMatch(cccNote) ) {
    insertActiveNote();
    return;
    }
  CsVisualScore::mousePressEvent( event );
  }




void CsVisualScoreEdit::wheelEvent(QWheelEvent *event)
  {
  if( mCellCursor.isMatch(cccNote) ) {
    int delta = event->angleDelta().y() / 12;
    if( mControl ) {
      mCellCursor.noteToneShift( delta > 0 );
      return;
      }
    if( mShift ) {
      mCellCursor.noteDurationShift( delta > 0 );
      return;
      }
    }
  CsVisualScore::wheelEvent( event );
  }



void CsVisualScoreEdit::editInsertLine(bool remark)
  {
  mComposition.lineInsert( qBound( 0, mCellCursor.lineIndex(), mComposition.lineCount() ), remark );
  }



void CsVisualScoreEdit::editAppendLine(bool remark)
  {
  mComposition.lineInsert( qBound( 0, mCellCursor.lineIndex() + 1, mComposition.lineCount() ), remark );
  mCellCursor.set( mCellCursor.lineIndex() + 1, mCellCursor.linePosition() );
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





template <typename Line>
bool moveLeft( Line &line, int position, int step )
  {
  //Find item at nearest right of cursor
  int index;
  for( index = 0; index < line.count(); index++ )
    if( position <= line.at(index).position() )
      break;

  if( index >= line.count() )
    //Nothing found
    return false;

  //Found item at nearest right of cursor
  int leftPosition = -1;
  if( index )
    leftPosition = line.at(index - 1).position();
  //Calculate new position
  int newPosition = line.at(index).position() - step;
  //Align position by step
  newPosition = newPosition / step * step;

  if( leftPosition < newPosition ) {
    //Change item position
    line[index].positionSet( newPosition );
    return true;
    }
  return false;
  }




template <typename Line>
bool moveRight( Line &line, int position, int step )
  {
  //Find item at nearest right of cursor
  int index;
  for( index = 0; index < line.count(); index++ )
    if( position <= line.at(index).position() )
      break;

  if( index >= line.count() )
    //Nothing found
    return false;

  //Found item at nearest right of cursor
  int rightPosition = 512 * 8;
  if( index + 1 < line.count() )
    rightPosition = line.at(index + 1).position();

  //Calculate new position
  int newPosition = line.at(index).position() + step;
  //Align position by step
  newPosition = newPosition / step * step;

  if( newPosition < rightPosition ) {
    //Change item position
    line[index].positionSet( newPosition );
    return true;
    }
  return false;
  }




template <typename Line>
bool moveLeftAll( Line &line, int position, int step )
  {
  //Find item at nearest right of cursor
  int index;
  for( index = 0; index < line.count(); index++ )
    if( position <= line.at(index).position() )
      break;

  if( index >= line.count() )
    //Nothing found
    return false;

  //Found item at nearest right of cursor
  int leftPosition = -1;
  if( index )
    leftPosition = line.at(index - 1).position();
  //Calculate new position
  int newPosition = line.at(index).position() - step;
  //Align position by step
  newPosition = newPosition / step * step;

  int delta = newPosition - line.at(index).position();

  if( leftPosition < newPosition ) {
    //Change positions of all item on right of index
    while( index < line.count() ) {
      line[index].positionSet( line.at(index).position() + delta );
      index++;
      }
    return true;
    }
  return false;
  }





template <typename Line>
bool moveRightAll( Line &line, int position, int step )
  {
  //Find item at nearest right of cursor
  int index;
  for( index = 0; index < line.count(); index++ )
    if( position <= line.at(index).position() )
      break;

  if( index >= line.count() )
    //Nothing found
    return false;

  //Found item at nearest right of cursor
  int rightPosition = line.at(index).position() + 512 * 8 - line.at( line.count() - 1 ).position();

  //Calculate new position
  int newPosition = line.at(index).position() + step;
  //Align position by step
  newPosition = newPosition / step * step;
  int delta = newPosition - line.at(index).position();

  if( newPosition < rightPosition ) {
    //Change positions of all item on right of index
    while( index < line.count() ) {
      line[index].positionSet( line.at(index).position() + delta );
      index++;
      }
    return true;
    }
  return false;
  }






//!
//! \brief keyLeft Handle key left pressing
//!
void CsVisualScoreEdit::keyLeft()
  {
  if( mControl || mShift ) {
    //We move chord, note or lyric to left
    if( mCellCursor.cellClass() == cccChord ) {
      //Get chord line pointed by cursor
      auto chordLine = mComposition.chordListGet( mCellCursor.lineIndex(), mCellCursor.partName() );
      if( (mShift ? moveLeftAll( chordLine, mCellCursor.linePosition(), mComposition.stepChord() ) :
          moveLeft( chordLine, mCellCursor.linePosition(), mComposition.stepChord() ))  )
        //Update chord line into composition
        mComposition.chordListSet( mCellCursor.lineIndex(), mCellCursor.partName(), chordLine );
      }
    else if( mCellCursor.cellClass() == cccNote ) {
      //Get note line pointed by cursor
      auto noteLine = mComposition.noteListGet( mCellCursor.lineIndex(), mCellCursor.partName() );
      if( (mShift ? moveLeftAll( noteLine, mCellCursor.linePosition(), mComposition.stepNote() ) :
           moveLeft( noteLine, mCellCursor.linePosition(), mComposition.stepNote() ))  )
        //Update note line into composition
        mComposition.noteListSet( mCellCursor.lineIndex(), mCellCursor.partName(), noteLine );
      }
    }

  //Simple cursor moving
  mCellCursor.move( ccoLeft );
  }




//!
//! \brief keyRight Handle key right pressing
//!
void CsVisualScoreEdit::keyRight()
  {
  if( mControl || mShift ) {
    //We move chord, note or lyric to right
    if( mCellCursor.cellClass() == cccChord ) {
      //Get chord line pointed by cursor
      auto chordLine = mComposition.chordListGet( mCellCursor.lineIndex(), mCellCursor.partName() );
      if( (mShift ? moveRightAll( chordLine, mCellCursor.linePosition(), mComposition.stepChord() ) :
           moveRight( chordLine, mCellCursor.linePosition(), mComposition.stepChord() ))  )
        //Update chord line into composition
        mComposition.chordListSet( mCellCursor.lineIndex(), mCellCursor.partName(), chordLine );
      }
    else if( mCellCursor.cellClass() == cccNote ) {
      //Get note line pointed by cursor
      auto noteLine = mComposition.noteListGet( mCellCursor.lineIndex(), mCellCursor.partName() );
      if( (mShift ? moveRightAll( noteLine, mCellCursor.linePosition(), mComposition.stepNote() ) :
           moveRight( noteLine, mCellCursor.linePosition(), mComposition.stepNote() ))  )
        //Update note line into composition
        mComposition.noteListSet( mCellCursor.lineIndex(), mCellCursor.partName(), noteLine );
      }
    }

  //Simple cursor moving
  mCellCursor.move( ccoRight );
  }




void CsVisualScoreEdit::keyToneShift( bool up )
  {
  if( mCellCursor.cellClass() == cccNote ) {
    mCellCursor.noteToneShift(up);
    //Get note line pointed by cursor
    auto noteLine = mComposition.noteListGet( mCellCursor.lineIndex(), mCellCursor.partName() );

    //Find note behind cursor
    int noteIndex;
    for( noteIndex = 0; noteIndex < noteLine.count(); noteIndex++ )
      if( noteLine.at(noteIndex).position() == mCellCursor.linePosition() )
        break;

    if( noteIndex < noteLine.count() ) {
      //Note found, change pitch
      noteLine[noteIndex].noteShift( up );
      mComposition.noteListSet( mCellCursor.lineIndex(), mCellCursor.partName(), noteLine );
      }
    }
  }




//!
//! \brief keyEnd Handle key End pressing
//!
void CsVisualScoreEdit::keyEnd()
  {
  if( mControl ) {
    if( (mCellCursor.cellClass() == cccChord) || (mCellCursor.cellClass() == cccNote) || (mCellCursor.cellClass() == cccLyric) ) {
      int takt = mCellCursor.linePosition();
      if( takt & 0xff ) takt = (takt + 255) >> 8;
      else takt >>= 8;
      mComposition.lineTaktCountSet( mCellCursor.lineIndex(), takt );
      }
    }
  }




//!
//! \brief keyTakt Append or remove takt from end of line
//! \param plus    if true then takt appended in other hand - removed
//!
void CsVisualScoreEdit::keyTakt(bool plus)
  {
  if( (mCellCursor.cellClass() == cccChord) || (mCellCursor.cellClass() == cccNote) || (mCellCursor.cellClass() == cccLyric) ) {
    int takt = mComposition.lineTaktCount( mCellCursor.lineIndex() );
    takt = qBound( 1, takt + (plus ? 1 : -1), 32 );
    mComposition.lineTaktCountSet( mCellCursor.lineIndex(), takt );
    }
  }




//!
//! \brief keyDelete Handle key Delete pressing
//!
void CsVisualScoreEdit::keyDelete()
  {
  if( mSelectedLines.count() )
    cmEditDelete();
  else if( mControl ) {
    cmEditDeleteLine();
    }
  else {
    if( mCellCursor.cellClass() == cccChord || mCellCursor.cellClass() == cccNote ) {
      mEditor = CsCursorEdit::build( mCellCursor, mComposition );
      mEditor->keyPress( Qt::Key_Delete, QChar{}, mEditor );
      }
    }
  }





//!
//! \brief unselectAll Remove all selections
//!
void CsVisualScoreEdit::unselectAll()
  {
  mSelectedLines.clear();
  emit actionEditSelectionPresent( false );
  update();
  }



//!
//! \brief insertActiveNote Inserts active note
//!
void CsVisualScoreEdit::insertActiveNote()
  {
  mEditor = CsCursorEdit::build( mCellCursor, mComposition );
  mEditor->setPitchDuration( mCellCursor.note().pitch(), mCellCursor.note().duration() );
  mEditor->keyPress( Qt::Key_Return, QChar(), mEditor );
  }



