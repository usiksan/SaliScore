#include "CsVisualList.h"

#include <QResizeEvent>

CsVisualList::CsVisualList(CsPlayList &playList, QWidget *parent) :
  QTreeWidget(parent),
  mPlayList(playList)
  {
  setColumnCount(5);
  setColumnWidth( 0, 32 );
  setColumnWidth( 1, 32 );
  setColumnWidth( 2, 32 );
  setColumnWidth( 4, 32 );
  setHeaderHidden(true);
  setSortingEnabled(false);
  setItemsExpandable(false);
  setRootIsDecorated(false);
  setDragDropMode(QAbstractItemView::NoDragDrop);
  setAutoScroll(true);
  //setDragEnabled(true);

  upgradeList();
  }




//!
//! \brief upgradeList called when need to upgrade full list
//!
void CsVisualList::upgradeList()
  {
  //Clear list contents
  clear();

  QTreeWidgetItem *item = new QTreeWidgetItem();
  item->setIcon( 0, QIcon(QString(":/pic/objectEditEnable.png")) );
  item->setIcon( 1, QIcon(QString(":/pic/objectEditEnable.png")) );
  item->setIcon( 2, QIcon(QString(":/pic/objectEditEnable.png")) );
  item->setText( 3, tr("Note trainer\nwith next line") );
  item->setIcon( 4, QIcon(QString(":/pic/delete_red.png")) );
  addTopLevelItem( item );
  for( int i = 0; i < 100; i++ )
    addTopLevelItem( new QTreeWidgetItem({QString("1"), QString("2"), QString("3"), tr("Gamma"), QString("5") }) );
  }


void CsVisualList::resizeEvent(QResizeEvent *event)
  {
  //Do parent work
  QTreeWidget::resizeEvent( event );

  //We need correct column width
  int w = event->size().width();
  setColumnWidth( 3, w - 32 * 4 - 20 );
  }
