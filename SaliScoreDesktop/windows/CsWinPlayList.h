#ifndef CSWINPLAYLIST_H
#define CSWINPLAYLIST_H

#include "score/CsPlayList.h"

#include <QTreeWidget>

class CsWinPlayList : public QTreeWidget
  {
    Q_OBJECT

    CsPlayList &mPlayList;
  public:
    explicit CsWinPlayList( CsPlayList &pl, QWidget *parent = nullptr);

    void settingsSave( const CsComposition &comp );

    void partAppend( const QString &partName );

    void compositionAppend( int partIndex, const QString &id );

    void updateContent();

    bool itemPosition( QTreeWidgetItem *item, int &partIndex, int &compositionIndex ) const;
  signals:

  public slots:

    void buildContent();
  };

#endif // CSWINPLAYLIST_H
