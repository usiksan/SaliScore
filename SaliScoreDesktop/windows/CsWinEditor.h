#ifndef CSWINEDITOR_H
#define CSWINEDITOR_H

#include <QWidget>
#include <QAbstractScrollArea>

class CsWinEditor : public QAbstractScrollArea
  {
    Q_OBJECT
  public:
    explicit CsWinEditor(QWidget *parent = nullptr);

  signals:

  };

#endif // CSWINEDITOR_H
