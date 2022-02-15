#ifndef CSVISUALABSTRACTLISTVIEW_H
#define CSVISUALABSTRACTLISTVIEW_H

#include <QWidget>

class CsVisualAbstractList;

class CsVisualAbstractListView : public QWidget
  {
    Q_OBJECT

    CsVisualAbstractList *mVisualList; //!< Widget which is abstract scroll area

  public:
    explicit CsVisualAbstractListView( CsVisualAbstractList *visualList, QWidget *parent = nullptr);

  signals:


    // QWidget interface
  protected:
    virtual void paintEvent(QPaintEvent *event) override;
  };

#endif // CSVISUALABSTRACTLISTVIEW_H
