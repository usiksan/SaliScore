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

    void updateContent();
  signals:

  };

#endif // CSWINPLAYLIST_H
