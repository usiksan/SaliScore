#include "score/CsNoteChord.h"
#include "CsVisualPiano.h"

#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QDebug>

CsVisualPiano::CsVisualPiano(QWidget *parent)
  : QWidget{parent}
  , mNoteStart(octaveBig)
  , mNoteStop(octaveBig + 60)
  {

  }



void CsVisualPiano::playNote(int note, int velo)
  {
  if( velo == 0 )
    mPressedKeys.remove( note );
  else
    mPressedKeys.insert( note );
  update();
  }




void CsVisualPiano::playHighlight(int channel, int note, int velo)
  {
  if( channel == 0 ) {
    if( velo == 0 )
      mHighlightKeys.remove( note );
    else
      mHighlightKeys.insert( note );
    update();
    }
  }



void CsVisualPiano::settings()
  {

  }



void CsVisualPiano::mousePressEvent(QMouseEvent *event)
  {
//  qDebug() << "Pressed " << event->pos();
  int note = findKey( event->pos() );
  if( note >= 0 ) {
    //Append note into playing set
    mPressedKeys.insert( note );
    emit midiNote( note, 127 );
    update();
    }
  }




void CsVisualPiano::mouseReleaseEvent(QMouseEvent *event)
  {
  int note = findKey( event->pos() );
  if( note >= 0 ) {
    //Remove note from playing set
    mPressedKeys.remove( note );
    emit midiNote( note, 0 );
    update();
    }
  }



void CsVisualPiano::paintEvent(QPaintEvent *event)
  {
  Q_UNUSED(event)
  QPainter painter(this);
  QSize s = size();
  painter.fillRect( QRect( QPoint{}, s ), Qt::lightGray );
  if( mWhiteKeys.count() == 0 ) {
    //Count of white keys
    int whiteCount = 0;
    for( int i = mNoteStart; i <= mNoteStop; i++ )
      if( csKeyIsWhite(i) ) whiteCount++;

    if( whiteCount == 0 ) return;

    //Width of white key
    QSize whiteKey( qMin( s.width() / whiteCount, s.height() / 4 ), s.height() - 3 );
    QSize blackKey( whiteKey.width() * 8 / 10, whiteKey.height() * 7 / 10 );

    //Start of display
    int startX = (s.width() - whiteKey.width() * whiteCount) / 2;

    //Begin to draw white keys
    for( int i = mNoteStart; i < mNoteStop; i++ )
      if( csKeyIsWhite(i) ) {
        //Create white key
        QRect r( QPoint(startX + mWhiteKeys.count() * whiteKey.width(), 0), whiteKey );
        mWhiteKeysMap.insert( mWhiteKeys.count(), i );
        mWhiteKeys.append( r );
        }
      else {
        //Create black key
        QRect r( QPoint(startX + mWhiteKeys.count() * whiteKey.width() - blackKey.width() / 2, 0), blackKey );
        mBlackKeysMap.insert( mBlackKeys.count(), i );
        mBlackKeys.append( r );
        }
    }

  //Draw white keys
  painter.setPen( Qt::gray );
  for( int i = 0; i < mWhiteKeys.count(); i++ ) {
    int note = mWhiteKeysMap.value(i);
    if( mPressedKeys.contains( note ) )
      painter.setBrush( QColor(Qt::green).lighter() );
    else if( mHighlightKeys.contains( note ) )
      painter.setBrush( QColor(Qt::blue).lighter() );
    else
      painter.setBrush( Qt::white );
    painter.drawRect( mWhiteKeys.at(i) );
    }


  for( int i = 0; i < mBlackKeys.count(); i++ ) {
    int note = mBlackKeysMap.value(i);
    if( mPressedKeys.contains( note ) )
      painter.setBrush( QColor(Qt::green).darker() );
    else if( mHighlightKeys.contains( note ) )
      painter.setBrush( QColor(Qt::blue).darker() );
    else
      painter.setBrush( Qt::black );
    painter.drawRect( mBlackKeys.at(i) );
    }
  }




void CsVisualPiano::clear()
  {
  for( auto note : qAsConst(mPressedKeys) )
    emit midiNote( note, 0 );
  mPressedKeys.clear();
  mHighlightKeys.clear();

  mWhiteKeys.clear();
  mWhiteKeysMap.clear();
  mBlackKeys.clear();
  mBlackKeysMap.clear();
  }



int CsVisualPiano::findKey(QPoint pos)
  {
  //At first we find in black keys list
  for( int i = 0; i < mBlackKeys.count(); i++ )
    if( mBlackKeys.at(i).contains( pos ) )
      return mBlackKeysMap.value( i, -1 );

  //At second we find in white keys list
  for( int i = 0; i < mWhiteKeys.count(); i++ )
    if( mWhiteKeys.at(i).contains( pos ) )
      return mWhiteKeysMap.value( i, -1 );

  return -1;
  }




void CsVisualPiano::resizeEvent(QResizeEvent *event)
  {
  QWidget::resizeEvent( event );
  clear();
  }
