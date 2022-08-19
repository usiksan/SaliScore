#ifndef SVVISUALABSTRACTLISTVIEW_H
#define SVVISUALABSTRACTLISTVIEW_H

#include <QWidget>

class SvVisualAbstractList;

class SvVisualAbstractListView : public QWidget
  {
    Q_OBJECT

    SvVisualAbstractList *mVisualList; //!< Widget which is abstract scroll area

  public:
    explicit SvVisualAbstractListView( SvVisualAbstractList *visualList, QWidget *parent = nullptr);

  signals:


    // QWidget interface
  protected:
    virtual void paintEvent(QPaintEvent *event) override;
  };

#endif // SVVISUALABSTRACTLISTVIEW_H
