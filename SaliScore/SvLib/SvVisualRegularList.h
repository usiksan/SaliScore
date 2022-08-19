#ifndef SVVISUALREGULARLIST_H
#define SVVISUALREGULARLIST_H

#include "SvVisualAbstractList.h"

class SvVisualRegularList : public SvVisualAbstractList
  {
    Q_OBJECT

  protected:
    QList<int> mCellWidth;  //!< Width of each cell. Value 0 - streched width
    QList<int> mCellBoundX; //!< Cell bound X position
    QString    mTitle;      //!< Title of list
  public:
    SvVisualRegularList( const QList<int> &cellWidth, QWidget *parent = nullptr );

    virtual void cellPaint( int x, int y, int w, int h, int column, int row, QPainter &painter ) = 0;

    virtual void cellClicked( int column, int row ) = 0;

  signals:
    void clickBack();

    // CsVisualAbstractList interface
  public:
    virtual int  itemHeight(int index) const override;
    virtual int  itemPaint(int index, int y, QPainter &painter) override;
    virtual void itemClicked(int x, int itemIndex) override;
    virtual int  headerHeight() const override;
    virtual void headerPaint(QPainter &painter) override;
    virtual void headerClicked(int x, int y) override;
  };

#endif // SVVISUALREGULARLIST_H
