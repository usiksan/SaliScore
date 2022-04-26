#ifndef SVVISUALABSTRACTLIST_H
#define SVVISUALABSTRACTLIST_H

#include <QAbstractScrollArea>
#include <QList>

class SvVisualAbstractList : public QAbstractScrollArea
  {
    Q_OBJECT
  protected:
    int        mStartY;       //!< Coordinate Y of list viewport begin
    int        mStartIndex;   //!< Index of first visible item
    int        mFullHeight;   //!< Full list height in pixels
    QList<int> mItemBounds;   //!< Y position bounds for each visible item
    int        mFooterBoundY; //!< Coordinate Y where footer draw. Footer resides below this bound
    int        mMousePressY;  //!< Coordinate Y where mouse was pressed
    int        mMouseStartY;  //!< Start Y display position at moment of mouse pressed
    bool       mMousePress;   //!< True if mouse pressed
  public:
    SvVisualAbstractList( QWidget *parent = nullptr );

    virtual void contentPaint( QPainter &painter );

    virtual void contentClicked( int x, int y );


    virtual int  itemHeight( int index ) const { Q_UNUSED(index) return 25; }

    virtual int  itemPaint( int index, int y, QPainter &painter );

    virtual int  itemCount() const { return 10; }

    virtual void itemClicked( int x, int itemIndex ) { Q_UNUSED(x) Q_UNUSED(itemIndex) }


    virtual int  headerHeight() const { return 0; }

    virtual void headerPaint( QPainter &painter ) { Q_UNUSED(painter) }

    virtual void headerClicked( int x, int y ) { Q_UNUSED(x) Q_UNUSED(y) }


    virtual int  footerHeight() const { return 0; }

    virtual void footerPaint( QPainter &painter ) { Q_UNUSED(painter) }

    virtual void footerClicked( int x, int y ) { Q_UNUSED(x) Q_UNUSED(y) }

  protected:
    void updateVerticalScrollBar( int fullHeight );

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

#endif // SVVISUALABSTRACTLIST_H
