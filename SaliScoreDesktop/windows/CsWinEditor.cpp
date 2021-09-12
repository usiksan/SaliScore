#include "CsWinEditor.h"
#include "CsWinScoreMode.h"
#include "CsPainterEditor.h"

#include <QPaintEvent>
#include <QPainter>
#include <QSettings>
#include <QScrollBar>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>


CsWinEditor::CsWinEditor(CsComposition &comp, CsPlay &play, QWidget *parent) :
  CsWinScoreView( comp, play, parent ),
  mOffsetX(0),
  mOffsetY(0),
  mSizeY(0),
  mCellCursor( comp ),
  mAutoScroll(false),
  mEditor(nullptr),
  mShift(false),
  mControl(false)
  {

  }



void CsWinEditor::paint()
  {
  QPainter painter(this);
  CsPainterEditor cp( &painter, QStringLiteral(KEY_EDITOR_SETTINGS), mComposition, mPlayer, mOffsetX, size(), &mCellCursor, mEditor );

  //Fill background
  painter.fillRect( QRect( QPoint(), size() ), cp.backgroundColor() );

  //Draw properties
  int posy = cp.drawTitleAndProperties( -mOffsetY, mComposition );

  //Draw lines of score
  for( int i = 0; i < mComposition.lineCount(); i++ )
    posy = cp.drawLine( posy, i, mComposition.line(i), mAutoScroll );

  //Update editor's reference list
  mReferenceList = cp.referenceList();

  //Norm posy to vertical size
  posy += mOffsetY;
  if( posy != mSizeY ) {
    //Height changed - update scrollbars
    mSizeY = posy;
    mWinScroll->verticalScrollBar()->setRange( 0, posy );
    mWinScroll->verticalScrollBar()->setSingleStep( size().height() / 100 );
    mWinScroll->verticalScrollBar()->setPageStep( size().height() * 9 / 10 );
    //verticalScrollBar()->setValue( -mOrigin.y() );
    }

  if( mAutoScroll ) {
    QRect r( cp.cellCursorRect() );
    //Vertical autoscroll
    //qDebug() << "offy" << mOffsetY << r.top() << r.bottom() << mSizeY;
    if( r.top() < 0 )
      mOffsetY = qBound( 0, mOffsetY + r.top(), mSizeY );
    else if( r.bottom() > size().height() )
      mOffsetY = qBound( 0, mOffsetY + r.bottom() - size().height() + 10, mSizeY );

    //Horizontal autoscroll
    if( r.left() < 0 )
      mOffsetX = qBound( 0, mOffsetX + r.left(), 1024 );
    else if( r.right() > size().width() )
      mOffsetX = qBound( 0, mOffsetX + r.right() - size().width(), 1024 );

    mAutoScroll = false;
    update();
    }
  }




void CsWinEditor::playStart()
  {
  //Begin play from current cursor
  if( mCellCursor.cellClass() == cccChord || mCellCursor.cellClass() == cccNote ) {
    mPlayer.jump( mCellCursor.lineIndex(), mCellCursor.position() );
    }
  else if( mCellCursor.cellClass() == cccLyric ) {
    mPlayer.jump( mCellCursor.lineIndex(), 0 );
    }
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
void CsWinEditor::keyLeft()
  {
  if( mControl ) {
    //We move chord, note or lyric to left
    if( mCellCursor.cellClass() == cccChord ) {
      //Get chord line pointed by cursor
      auto chordLine = mComposition.chordListGet( mCellCursor.lineIndex(), mCellCursor.partName() );
      if( (mShift ? moveLeftAll( chordLine, mCellCursor.position(), mComposition.stepChord() ) :
          moveLeft( chordLine, mCellCursor.position(), mComposition.stepChord() ))  )
        //Update chord line into composition
        mComposition.chordListSet( mCellCursor.lineIndex(), mCellCursor.partName(), chordLine );
      }
    else if( mCellCursor.cellClass() == cccNote ) {
      //Get note line pointed by cursor
      auto noteLine = mComposition.noteListGet( mCellCursor.lineIndex(), mCellCursor.partName() );
      if( (mShift ? moveLeftAll( noteLine, mCellCursor.position(), mComposition.stepNote() ) :
           moveLeft( noteLine, mCellCursor.position(), mComposition.stepNote() ))  )
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
void CsWinEditor::keyRight()
  {
  if( mControl ) {
    //We move chord, note or lyric to right
    if( mCellCursor.cellClass() == cccChord ) {
      //Get chord line pointed by cursor
      auto chordLine = mComposition.chordListGet( mCellCursor.lineIndex(), mCellCursor.partName() );
      if( (mShift ? moveRightAll( chordLine, mCellCursor.position(), mComposition.stepChord() ) :
           moveRight( chordLine, mCellCursor.position(), mComposition.stepChord() ))  )
        //Update chord line into composition
        mComposition.chordListSet( mCellCursor.lineIndex(), mCellCursor.partName(), chordLine );
      }
    else if( mCellCursor.cellClass() == cccNote ) {
      //Get note line pointed by cursor
      auto noteLine = mComposition.noteListGet( mCellCursor.lineIndex(), mCellCursor.partName() );
      if( (mShift ? moveRightAll( noteLine, mCellCursor.position(), mComposition.stepNote() ) :
           moveRight( noteLine, mCellCursor.position(), mComposition.stepNote() ))  )
        //Update note line into composition
        mComposition.noteListSet( mCellCursor.lineIndex(), mCellCursor.partName(), noteLine );
      }
    }

  //Simple cursor moving
  mCellCursor.move( ccoRight );
  }





//!
//! \brief keyEnd Handle key End pressing
//!
void CsWinEditor::keyEnd()
  {
  if( mControl ) {
    if( (mCellCursor.cellClass() == cccChord) || (mCellCursor.cellClass() == cccNote) || (mCellCursor.cellClass() == cccLyric) ) {
      int takt = mCellCursor.position();
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
void CsWinEditor::keyTakt(bool plus)
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
void CsWinEditor::keyDelete()
  {
  if( mControl ) {
    if( (mCellCursor.cellClass() == cccChord) || (mCellCursor.cellClass() == cccNote) || (mCellCursor.cellClass() == cccLyric) ||
        (mCellCursor.cellClass() == cccRemark) ) {
      mComposition.lineRemove( mCellCursor.lineIndex() );
      mCellCursor.updatePosition();
      }
    }
  }




void CsWinEditor::setupWinScroll(CsWinScoreMode *winScroll)
  {
  CsWinScoreView::setupWinScroll( winScroll );
  winScroll->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
  winScroll->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
  //Tracking scroll bar position
  connect( winScroll->verticalScrollBar(), &QScrollBar::valueChanged, this, [this] (int value) {
    mOffsetY = qBound( 0, value, mSizeY );
    update();
    });
  connect( winScroll->horizontalScrollBar(), &QScrollBar::valueChanged, this, [this] (int value) {
    mOffsetX = qBound( 0, value, 1024 );
    update();
    //originSet( SdPoint(value,originGet().y()) );
    });

  winScroll->horizontalScrollBar()->setRange( 0, 1024 );
  winScroll->horizontalScrollBar()->setSingleStep( 1 );
  winScroll->horizontalScrollBar()->setPageStep( 128 );
  }




void CsWinEditor::upWheelEvent(QWheelEvent *event)
  {
  int delta = event->angleDelta().y() / 12;
  mOffsetY = qBound( 0, mOffsetY - delta, mSizeY );
  mWinScroll->verticalScrollBar()->setValue( mOffsetY );
  update();
  }




void CsWinEditor::upMousePressEvent(QMouseEvent *event)
  {
  if( mEditor != nullptr ) {
    //If actived editor, then apply changes and finish editing
    mEditor->apply();
    delete mEditor;
    mEditor = nullptr;
    }

  for( int i = mReferenceList.count() - 1; i >= 0; i-- )
    if( mReferenceList.at(i).isHit( event->pos() ) ) {
      const CsReference &ref( mReferenceList.at(i) );

      switch( ref.type() ) {
        case cccTitle :
        case cccVoice :
        case cccSinger :
        case cccVoiceDual :
        case cccComposer :
        case cccVoiceRight :
        case cccLyricist :
        case cccStyle :
        case cccAuthor :
        case cccTempo :
          mCellCursor.jump( ref.type() );
          break;

        case cccRemark :
        case cccChord :
        case cccNote :
        case cccLyric :
        case cccTranslation :
          mCellCursor.jump( ref.type(), ref.index(), ref.line(), ref.part() );
          break;

        }
      update();
      return;
      }
  }

void CsWinEditor::upMouseReleaseEvent(QMouseEvent *event)
  {
  }

void CsWinEditor::upMouseMoveEvent(QMouseEvent *event)
  {
  }

void CsWinEditor::upKeyPressEvent(QKeyEvent *event)
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
                                                       cccSinger, cccVoiceDual, cccComposer, cccVoiceRight,
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



void CsWinEditor::upKeyReleaseEvent(QKeyEvent *event)
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




void CsWinEditor::compositionChanged()
  {
  CsWinScoreView::compositionChanged();
  mReferenceList.clear();
  mOffsetX = mOffsetY = mSizeY = 0;

  mCellCursor.moveTop();

  if( mEditor != nullptr ) {
    delete mEditor;
    mEditor = nullptr;
    }
  update();
  }
