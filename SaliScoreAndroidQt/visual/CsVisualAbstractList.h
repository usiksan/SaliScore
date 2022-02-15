#ifndef CSVISUALABSTRACTLIST_H
#define CSVISUALABSTRACTLIST_H

#include <QAbstractScrollArea>
#include <QList>

class CsVisualAbstractList : public QAbstractScrollArea
  {
    Q_OBJECT
  protected:
    int        mStartY;
    int        mStartIndex;
    int        mFullHeight;
    QList<int> mItemBounds;
    QList<int> mXBounds;
    int        mMousePressY;
    int        mMouseStartY;
    bool       mMousePress;
  public:
    CsVisualAbstractList( QWidget *parent = nullptr );

    virtual void paintContent( QPainter &painter );

    virtual int  itemHeight( int index ) const { Q_UNUSED(index) return 25; }

    virtual int  paintItem( int index, int y, QPainter &painter );

    virtual int  count() const { return 10; }

    virtual void clicked( int x, int itemIndex ) { Q_UNUSED(x) Q_UNUSED(itemIndex) }

  public slots:
    //!
    //! \brief updateContent Call update function on viewport
    //!
    void updateContent();

    // QWidget interface
  protected:
    virtual void paintEvent(QPaintEvent *event) override;

    // QWidget interface
  protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
  };

#endif // CSVISUALABSTRACTLIST_H
