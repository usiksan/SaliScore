#ifndef CSANDROIDWINMAIN_H
#define CSANDROIDWINMAIN_H

#include "score/CsComposition.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <QSplitter>

class CsPlayList;
class CsVisualPlayList;
class CsVisualPartList;
class CsVisualScoreTrain;


class CsAndroidWinMain : public QMainWindow
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
