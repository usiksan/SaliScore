#ifndef CSVISUALABSTRACTLIST_H
#define CSVISUALABSTRACTLIST_H

#include <QAbstractScrollArea>
#include <QList>

class CsVisualAbstractList : public QAbstractScrollArea
  {
    Q_OBJECT
    int        mStartY;
    int        mStartIndex;
    QList<int> mItemBounds;
  public:
    CsVisualAbstractList( QWidget *parent = nullptr );

    virtual void paintContent( QPainter &painter );

    virtual int  itemHeight( int index ) const { Q_UNUSED(index) return 25; }

    virtual int  paintItem( int index, int y, QPainter &painter );

    virtual int  count() const { return 10; }

    // QWidget interface
  protected:
    virtual void paintEvent(QPaintEvent *event) override;
  };

#endif // CSVISUALABSTRACTLIST_H
