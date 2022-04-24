#ifndef CSVISUALPARTLIST_H
#define CSVISUALPARTLIST_H

#include "CsConfig.h"
#include "score/CsPlayList.h"
#include "CsVisualRegularList.h"

class CsVisualPartList : public CsVisualRegularList
  {
    Q_OBJECT

    int         mPartIndex; //!< Part index
  public:
    CsVisualPartList( QWidget *parent = nullptr );

    int partIndex() const { return mPartIndex; }

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
