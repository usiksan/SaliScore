#ifndef CSVISUALLIST_H
#define CSVISUALLIST_H

#include <QTreeWidget>

class CsPlayList;

class CsVisualList : public QTreeWidget
  {
    Q_OBJECT

    CsPlayList &mPlayList; //!< Main play list
    QString     mPartName; //!< Name of part when list display content of part
  public:
    CsVisualList( CsPlayList &playList, QWidget *parent = nullptr );

  public slots:
    //!
    //! \brief upgradeList called when need to upgrade full list
    //!
    void upgradeList();

    // QWidget interface
  protected:
    virtual void resizeEvent(QResizeEvent *event) override;
  };

#endif // CSVISUALLIST_H
