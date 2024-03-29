#ifndef CSVISUALPARTLIST_H
#define CSVISUALPARTLIST_H

#include "CsConfig.h"
#include "score/CsPlayList.h"
#include "SvLib/SvVisualRegularList.h"

class CsVisualPartList : public SvVisualRegularList
  {
    Q_OBJECT

    int            mPartIndex;   //!< Part index
    CsComposition &mComposition; //!< Editing composition. It need to display current composition of part
  public:
    CsVisualPartList( CsComposition &composition, QWidget *parent = nullptr );

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
