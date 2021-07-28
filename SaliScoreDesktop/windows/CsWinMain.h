#ifndef CSWINMAIN_H
#define CSWINMAIN_H

#include <QMainWindow>

class CsWinMain : public QMainWindow
  {
    Q_OBJECT

  public:
    CsWinMain(QWidget *parent = nullptr);
    ~CsWinMain();
  };
#endif // CSWINMAIN_H
