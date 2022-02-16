#include "CsVisualPlayList.h"

#include <QPainter>
#include <QInputDialog>
#include <QMessageBox>

CsVisualPlayList::CsVisualPlayList(CsPlayList &playList, QWidget *parent) :
  CsVisualAbstractList( parent ),
  mPlayList(playList)
  {

  }

int CsVisualPlayList::itemHeight(int index) const
  {
  if( index == 3 || index == 6 )
    return 6;
  return 36;
  }



int CsVisualPlayList::itemPaint(int index, int y, QPainter &painter)
  {
  int w = viewport()->size().width();
  if( index == 3 || index == 6 ) {
    //Draw line
    painter.drawLine( 5, y + 2, w - 5, y + 2 );
    }
  else {
    mDelIconX = w - 32;
    //At left side draw edit icon
    if( index >= 7 )
      QIcon(QStringLiteral(":/pic/objectEditEnable.png")).paint( &painter, 0, y, 32, 32 );

    //At center draw part title
    painter.drawText( 34, y, w - 34*2, 32, Qt::AlignLeft | Qt::AlignVCenter, title(index) );

    //At right side draw delete icon
    if( index >= 7 && index < mPlayList.partCount() + 7 )
      QIcon(QStringLiteral(":/pic/delete_red.png")).paint( &painter, mDelIconX, y, 32, 32 );
    }

  return itemHeight( index );
  }


void CsVisualPlayList::itemClicked(int x, int itemIndex)
  {
  switch( itemIndex ) {
    case 0 :
      emit selectSettings();
      break;
    case 1 :
      emit selectHelp();
      break;
    case 2 :
      emit web();
      break;
    case 3 : return;
    case 4 :
      emit selectNoteTrainer();
      break;
    case 5 :
      emit selectGammaTrainer();
      break;
    case 6 : return;

    default :
      if( x < 33 ) {
        //Pressed on Edit icon
        QString newTitle = QInputDialog::getText( this, tr("Enter new name for list"), tr("List name:"), QLineEdit::Normal, title(itemIndex) );
        if( !newTitle.isEmpty() && newTitle != title(itemIndex) ) {
          //Update
          itemIndex -= 7;
          if( itemIndex < mPlayList.partCount() )
            mPlayList.partTitleSet( itemIndex, newTitle );
          else
            //Append new part list
            mPlayList.partAppend( newTitle );
          //Update content
          update();
          }
        }
      else if( x < mDelIconX ) {
        //Pressed on name - select part
        itemIndex -= 7;
        if( itemIndex < mPlayList.partCount() )
          emit selectPart( itemIndex );
        }
      else {
        //Pressed on Delete icon
        itemIndex -= 7;
        if( itemIndex < mPlayList.partCount() && QMessageBox::question( this, tr("Warning!"), tr("Are you sure to delete list \'%1\'").arg(mPlayList.partTitle(itemIndex))) == QMessageBox::Yes ) {
          //Delete list
          mPlayList.partDelete( itemIndex );
          update();
          }
        }
    }

  }



//!
//! \brief playListUpgrade Upgrade play list when it changed outside
//!
void CsVisualPlayList::playListUpgrade()
  {
  mStartY = mStartIndex = 0;
  updateContent();
  }



QString CsVisualPlayList::title(int itemIndex)
  {
  //Settings
  //Help
  //Web
  //-------------
  //Note trainer
  //Gamma
  //-------------
  switch( itemIndex ) {
    case 0 : return tr("Settings");
    case 1 : return tr("Help");
    case 2 : return tr("Web");
    case 3 : return tr("---");
    case 4 : return tr("Note trainer");
    case 5 : return tr("Gamma trainer");
    case 6 : return tr("---");
    }
  itemIndex -= 7;
  if( itemIndex < mPlayList.partCount() )
    return mPlayList.partTitle(itemIndex);

  return tr("New list");
  }
