#include "CsWinPlayList.h"

CsWinPlayList::CsWinPlayList(CsPlayList &pl, QWidget *parent) :
  QTreeWidget(parent),
  mPlayList(pl)
  {
  //public author singer song
  setColumnCount(4);
  setHeaderLabels( { tr("Public"), tr("Author"), tr("Singer"), tr("Song") } );
  }




void CsWinPlayList::buildContent()
  {
  clear();
  for( int i = 0; i < mPlayList.partCount(); i++ ) {
    QTreeWidgetItem *top = new QTreeWidgetItem( { QString{}, mPlayList.partTitle(i)} );
    mPlayList.partItemSet( i, top );
    top->setIcon( 0, QIcon(QStringLiteral(":/pic/plPart.png")) );
    insertTopLevelItem( i, top );

    int compositionCount = mPlayList.partCompositionCount(i);
    for( int compositionIndex = 0; compositionIndex < compositionCount; compositionIndex++ ) {
      QString id = mPlayList.partCompositionId( i, compositionIndex );
      CsCompositionSettings s = mPlayList.composition( id );
      QTreeWidgetItem *item = new QTreeWidgetItem( { QString{}, s.author(), s.singer(), s.name() } );
      mPlayList.partCompositionItemSet( i, compositionIndex, item );
      item->setIcon( 0, QIcon( s.isPublic() ? QStringLiteral(":/pic/plPublic.png") : QStringLiteral(":/pic/plPrivate.png")) );
      top->insertChild( compositionIndex, item );
      }

    }
  }



void CsWinPlayList::settingsSave(const CsComposition &comp)
  {
  mPlayList.compositionSet( comp );

  //Deside where insert new song
  int partIndex;
  QTreeWidgetItem *cur = currentItem();

  if( cur == nullptr ) {
    //Nothing select - append in My Songs
    if( mPlayList.partCount() == 0 )
      partAppend( tr("My Songs") );
    partIndex = 0;
    }
  else {
    QTreeWidgetItem *top = cur->parent();
    if( top != nullptr )
      partIndex = indexOfTopLevelItem( top );
    else
      partIndex = indexOfTopLevelItem( cur );
    }

  compositionAppend( partIndex, comp.header().songId() );
  updateContent();
  }



void CsWinPlayList::partAppend(const QString &partName)
  {
  if( mPlayList.partAppend( partName ) ) {
    //Successfull appendion
    QTreeWidgetItem *top = new QTreeWidgetItem( { QString{}, partName } );
    mPlayList.partItemSet( mPlayList.partCount() - 1, top );
    top->setIcon( 0, QIcon(QStringLiteral(":/pic/plPart.png")) );
    insertTopLevelItem( mPlayList.partCount() - 1, top );
    }
  }



void CsWinPlayList::compositionAppend(int partIndex, const QString &id)
  {
  //We append into partIndex
  if( mPlayList.partCompositionAppend( partIndex, id ) ) {
    //Composition appended
    int compositionIndex = mPlayList.partCompositionCount( partIndex ) - 1;
    QTreeWidgetItem *top = mPlayList.partItem( partIndex );
    CsCompositionSettings s = mPlayList.composition( id );
    QTreeWidgetItem *item = new QTreeWidgetItem( { QString{}, s.author(), s.singer(), s.name() } );
    mPlayList.partCompositionItemSet( partIndex, compositionIndex, item );
    item->setIcon( 0, QIcon( s.isPublic() ? QStringLiteral(":/pic/plPublic.png") : QStringLiteral(":/pic/plPrivate.png")) );
    top->insertChild( compositionIndex, item );
    }

  }



void CsWinPlayList::updateContent()
  {
  for( int partIndex = 0; partIndex < mPlayList.partCount(); partIndex++ ) {
    mPlayList.partItem(partIndex)->setText( 1, mPlayList.partTitle(partIndex) );

    int compositionCount = mPlayList.partCompositionCount( partIndex );
    for( int compositionIndex = 0; compositionIndex < compositionCount; compositionIndex++ ) {
      QTreeWidgetItem *item = mPlayList.partCompositionItem( partIndex, compositionIndex );

      QString id = mPlayList.partCompositionId( partIndex, compositionIndex );
      CsCompositionSettings s = mPlayList.composition( id );
      item->setText( 1, s.author() );
      item->setText( 2, s.singer() );
      item->setText( 3, s.name() );
      item->setIcon( 0, QIcon( s.isPublic() ? QStringLiteral(":/pic/plPublic.png") : QStringLiteral(":/pic/plPrivate.png")) );
      }
    }
  }




bool CsWinPlayList::itemPosition(QTreeWidgetItem *item, int &partIndex, int &compositionIndex) const
  {
  QTreeWidgetItem *top = item->parent();
  if( top == nullptr )
    return false;
  partIndex = indexOfTopLevelItem( top );
  compositionIndex = top->indexOfChild( item );
  return true;
  }
