#include "CsVisualPlayList.h"

#include <QPainter>
#include <QInputDialog>
#include <QMessageBox>

#ifdef Q_OS_ANDROID
#define CS_STATIC_ITEM 7
#else
#define CS_STATIC_ITEM 3
#endif

CsVisualPlayList::CsVisualPlayList(QWidget *parent) :
  SvVisualAbstractList( parent )
  {
  //Connect with play list
  CsPlayList::pl()->mChanged.connect( mPlayListChanged, [this] () { update(); } );
  }

int CsVisualPlayList::itemHeight(int index) const
  {
#ifdef Q_OS_ANDROID
  if( index == 3 || index == 6 )
    return 6;
#else
  if( index == 2 )
    return 6;
#endif
  return 36;
  }



int CsVisualPlayList::itemPaint(int index, int y, QPainter &painter)
  {
  int w = viewport()->size().width();
#ifdef Q_OS_ANDROID
  if( index == 3 || index == 6 ) {
#else
  if( index == 2 ) {
#endif
    //Draw line
    painter.drawLine( 5, y + 2, w - 5, y + 2 );
    }
  else {
    mDelIconX = w - 32;
    //At left side draw edit icon
    if( index >= CS_STATIC_ITEM )
      QIcon(QStringLiteral(":/pic/objectEditEnable.png")).paint( &painter, 0, y, 32, 32 );

    //At center draw part title
    painter.drawText( 34, y, w - 34*2, 32, Qt::AlignLeft | Qt::AlignVCenter, title(index) );

    //At right side draw delete icon
    if( index >= CS_STATIC_ITEM && index < CsPlayList::pl()->partCount() + CS_STATIC_ITEM )
      QIcon(QStringLiteral(":/pic/delete_red.png")).paint( &painter, mDelIconX, y, 32, 32 );
    }

  return itemHeight( index );
  }



int CsVisualPlayList::itemCount() const
  {
  return CsPlayList::pl()->partCount() + 1 + CS_STATIC_ITEM;
  }


void CsVisualPlayList::itemClicked(int x, int itemIndex)
  {
  switch( itemIndex ) {
#ifdef Q_OS_ANDROID
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
#else
    case 0 :
      emit selectNoteTrainer();
      break;
    case 1 :
      emit selectGammaTrainer();
      break;
    case 2 : return;
#endif

    default :
      if( x < 33 ) {
        //Pressed on Edit icon
        QString newTitle = QInputDialog::getText( this, tr("Enter new name for list"), tr("List name:"), QLineEdit::Normal, title(itemIndex) );
        if( !newTitle.isEmpty() && newTitle != title(itemIndex) ) {
          //Update
          itemIndex -= CS_STATIC_ITEM;
          if( itemIndex < CsPlayList::pl()->partCount() )
            CsPlayList::pl()->partTitleSet( itemIndex, newTitle );
          else
            //Append new part list
            CsPlayList::pl()->partAppend( newTitle );
          //Update content
          update();
          }
        }
      else if( x < mDelIconX ) {
        //Pressed on name - select part
        itemIndex -= CS_STATIC_ITEM;
        if( itemIndex < CsPlayList::pl()->partCount() )
          emit selectPart( itemIndex );
        }
      else {
        //Pressed on Delete icon
        itemIndex -= CS_STATIC_ITEM;
        if( itemIndex < CsPlayList::pl()->partCount() && QMessageBox::question( this, tr("Warning!"), tr("Are you sure to delete list \'%1\'").arg(CsPlayList::pl()->partTitle(itemIndex))) == QMessageBox::Yes ) {
          //Delete list
          CsPlayList::pl()->partDelete( itemIndex );
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
#ifdef Q_OS_ANDROID
  //Settings
  //Help
  //Web
  //-------------
  //Note trainer
  //Gamma
  //-------------
  //All compositions
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
#else
  //Note trainer
  //Gamma
  //-------------
  //All compositions
  switch( itemIndex ) {
    case 0 : return tr("Note trainer");
    case 1 : return tr("Gamma trainer");
    case 2 : return tr("---");
    //case 3 : return tr("All compositions");
    }
  itemIndex -= 3;
#endif
  if( itemIndex < CsPlayList::pl()->partCount() )
    return CsPlayList::pl()->partTitle(itemIndex);

  return tr("New list");
  }
