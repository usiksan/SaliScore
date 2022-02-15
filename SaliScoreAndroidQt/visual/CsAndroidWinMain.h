#ifndef CSANDROIDWINMAIN_H
#define CSANDROIDWINMAIN_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QSplitter>
#include <QTabWidget>
#include <QStringList>
#include <QClipboard>
#include <QAction>
#include <QTimer>
#include <QLineEdit>

class CsPlayList;
class CsVisualList;

class CsAndroidWinMain : public QMainWindow
  {
    Q_OBJECT

    QSplitter       *mWSplitter;     //!< Central widget of application, it delimit space into two parts: wiziwig editors and help
    QStackedWidget  *mWLeftPart;     //!< Left part of main window. Contains play list
    CsVisualList    *mWLeftPlayList;
    QStackedWidget  *mWCentralPart;  //!< Central part with editor, trainer and karaoke

    CsPlayList      &mPlayList;      //!< Main play list
  public:
    CsAndroidWinMain( CsPlayList &playList, QWidget *parent = nullptr);
    ~CsAndroidWinMain();

  public slots:
//    void cmPlayStart();
//    void cmPlayStop();
//    void cmPlayMetronomToggle();
//    void cmFragment0();
//    void cmFragment1();
//    void cmFragment2();
//    void cmFragment
  };
#endif // CSANDROIDWINMAIN_H
