#include "CsVisualPartList.h"

#include <QIcon>
#include <QPainter>
#include <QMessageBox>

CsVisualPartList::CsVisualPartList(CsPlayList &playList, QWidget *parent) :
  CsVisualRegularList( {36,0,36}, parent ),
  mPlayList(playList),
  mPartIndex(-1)
  {

  }

void CsVisualPartList::setPart(int partIndex)
  {
  mPartIndex = partIndex;
  mTitle = mPlayList.partTitle(mPartIndex);
  //qDebug() << "setPart" << partIndex << itemCount();
  updateContent();
  }


void CsVisualPartList::cellPaint(int x, int y, int w, int h, int column, int row, QPainter &painter)
  {
  if( row < itemCount() ) {
    if( column == 0 ) {
      //Tool icon
      }
    else if( column == 1 ) {
      h = 20;
      //Song title
      QString id = mPlayList.partCompositionId( mPartIndex, row );
      //Song header
      CsCompositionInfo info = mPlayList.composition( id );
      //At top display composition name
      painter.drawText( x, y, w, h, Qt::AlignLeft | Qt::AlignVCenter, info.name() );
      //At bottom display singer and author
      painter.drawText( x + 20, y + h, w - 20, h, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("%1 [%2]").arg( info.singer(), info.author() ) );
      }
    else {
      //Delete icon
      QIcon(QStringLiteral(":/pic/delete_red.png")).paint( &painter, x, y, 32, 32 );
      }

    }
  }


void CsVisualPartList::cellClicked(int column, int row)
  {
  if( row < itemCount() ) {
    if( column == 0 ) {
      //Tool icon
      }
    else if( column == 1 ) {
      //Song title
      emit compositionClicked( mPlayList.partCompositionId( mPartIndex, row ) );
      }
    else {
      if( QMessageBox::question( this, tr("Warning!"), tr("Are you sure to delete song \'%1\' from list").arg(mPlayList.partCompositionName( mPartIndex, row ))) == QMessageBox::Yes ) {
        mPlayList.partCompositionRemove( mPartIndex, row );
        updateContent();
        }
      }
    }
  }


int CsVisualPartList::itemCount() const
  {
  return mPartIndex < 0 || mPartIndex >= mPlayList.partCount() ? 0 : mPlayList.partCompositionCount( mPartIndex );
  }
