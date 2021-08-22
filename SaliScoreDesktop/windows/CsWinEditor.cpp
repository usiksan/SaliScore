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

  switch( key ) {
    case Qt::Key_Up :
      if( mEditor != nullptr )
        mEditor->keyPress( key, ch, mEditor );
      if( mEditor == nullptr )
        mCellCursor.move( ccoUp );
      break;

    case Qt::Key_Down :
      if( mEditor != nullptr )
        mEditor->keyPress( key, ch, mEditor );
      if( mEditor == nullptr )
        mCellCursor.move( ccoDown );
      break;

    case Qt::Key_Left :
      if( mEditor == nullptr )
        mCellCursor.move( ccoLeft );
      else
        mEditor->keyPress( key, ch, mEditor );
      break;

    case Qt::Key_Right :
      if( mEditor == nullptr )
        mCellCursor.move( ccoRight );
      else
        mEditor->keyPress( key, ch, mEditor );
      break;

    case Qt::Key_Return :
      if( mEditor == nullptr )
        mEditor = CsCursorEdit::build( mCellCursor, mComposition );
      else
        mEditor->keyPress( key, ch, mEditor );
      break;

    case Qt::Key_Insert :
      //Insert new line
      mComposition.lineInsert( qBound( 0, mCellCursor.lineIndex(), mComposition.lineCount() ), false );
      break;

    case Qt::Key_A :
    case Qt::Key_B :
    case Qt::Key_C :
    case Qt::Key_D :
    case Qt::Key_E :
    case Qt::Key_F :
    case Qt::Key_G :
      if( mEditor == nullptr && mCellCursor.isMatchClass( { cccChord, cccNote } ) )
        mEditor = CsCursorEdit::build( mCellCursor, mComposition );

    default:
      if( mEditor == nullptr && ch.isPrint() && mCellCursor.isMatchClass( {cccRemark, cccLyric, cccTranslation, cccTitle,   cccVoice,
                                                                           cccSinger, cccVoiceDual, cccComposer, cccVoiceRight,
                                                                           cccLyricist, cccStyle, cccAuthor, cccTempo } )  )
          mEditor = CsCursorEdit::build( mCellCursor, mComposition );
      if( mEditor != nullptr )
        mEditor->keyPress( key, ch, mEditor );
      break;
    }

  mAutoScroll = true;
  event->accept();
  update();
  }



void CsWinEditor::upKeyReleaseEvent(QKeyEvent *event)
  {
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
