#include "CsWinPlayList.h"

CsWinPlayList::CsWinPlayList(CsPlayList &pl, QWidget *parent) :
  QTreeWidget(parent),
  mPlayList(pl)
  {
  //public author singer song
  setColumnCount(4);
  setHeaderLabels( { tr("Public"), tr("Author"), tr("Singer"), tr("Song") } );
  }




void CsWinPlayList::updateContent()
  {
  clear();
  for( int i = 0; i < mPlayList.partCount(); i++ ) {
    QTreeWidgetItem *top = new QTreeWidgetItem( {mPlayList.partTitle(i)} );
    top->setIcon( 0, QIcon(QStringLiteral(":/pic/fileNew.png")) );
    insertTopLevelItem( i, top );

    int compositionCount = mPlayList.partCompositionCount(i);
    for( int compositionIndex = 0; compositionIndex < compositionCount; compositionIndex++ ) {
      QString id = mPlayList.partCompositionId( i, compositionIndex );
      CsCompositionSettings s = mPlayList.composition( id );
      QTreeWidgetItem *item = new QTreeWidgetItem( { QString{}, s.author(), s.singer(), s.name() } );
      item->setIcon( 0, QIcon(QStringLiteral(":/pic/fileSave.png")) );
      top->insertChild( compositionIndex, item );
      }

    }
  }
