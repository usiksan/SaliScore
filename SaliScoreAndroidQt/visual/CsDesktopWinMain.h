#ifndef CSDESKTOPWINMAIN_H
#define CSDESKTOPWINMAIN_H

#include "score/CsComposition.h"

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
class CsVisualPlayList;
class CsVisualPartList;
class CsVisualScoreTrain;

class CsDesktopWinMain : public QMainWindow
  {
    Q_OBJECT

    QSplitter          *mWSplitter;          //!< Central widget of application, it delimit space into two parts: wiziwig editors and help
    QStackedWidget     *mWLeftPart;          //!< Left part of main window. Contains play list
    CsVisualPlayList   *mWLeftPlayList;      //!< Widget with play list
    CsVisualPartList   *mWLeftPartList;      //!< Widget with part list

    QStackedWidget     *mWCentralPart;       //!< Central part with editor, trainer and karaoke
    CsVisualScoreTrain *mWCentralScoreTrain; //!< Visual score view and train


    CsPlayList         &mPlayList;           //!< Main play list
    CsComposition       mComposition;        //!< Current composition

  public:
    explicit CsDesktopWinMain(  CsPlayList &playList, QWidget *parent = nullptr);

  signals:

  };

#endif // CSDESKTOPWINMAIN_H
