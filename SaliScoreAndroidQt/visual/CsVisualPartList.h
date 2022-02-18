#ifndef CSVISUALPARTLIST_H
#define CSVISUALPARTLIST_H

#include "config.h"
#include "score/CsPlayList.h"
#include "CsVisualRegularList.h"

class CsVisualPartList : public CsVisualRegularList
  {
    Q_OBJECT

    CsPlayList &mPlayList;  //!< Play list which displayed with this widget
    int         mPartIndex; //!< Part index
  public:
    CsVisualPartList( CsPlayList &playList, QWidget *parent = nullptr );

  signals:
    void compositionClicked( const QString &compositionId );

  public slots:
    void setPart( int partIndex );

    // CsVisualRegularList interface
  public:
    virtual void cellPaint(int x, int y, int w, int h, int column, int row, QPainter &painter) override;
    virtual void cellClicked(int column, int row) override;

    // CsVisualAbstractList interface
  public:
    virtual int itemCount() const override;
  };

#endif // CSVISUALPARTLIST_H
