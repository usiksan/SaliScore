#include "config.h"
#include "CsAndroidWinMain.h"
#include "CsVisualPlayList.h"
#include "CsVisualPartList.h"
#include "CsVisualScoreTrain.h"
#include "repo/CsRepoClient.h"

#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QToolBar>
#include <QToolButton>
#include <QTableWidget>
#include <QHeaderView>
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#include <QtAndroid>
#endif


CsAndroidWinMain::CsAndroidWinMain(CsPlayList &playList, QWidget *parent) :
  QMainWindow(parent),
  mPlayList(playList)
  {
  //==========================================
  //      Top layer of layout
  //At left side - play list
  mWLeftPart = new QStackedWidget();

  //At central part - wiziwig editors
  mWCentralPart = new QStackedWidget();

  mWSplitter    = new QSplitter();
  mWSplitter->addWidget( mWLeftPart );
  mWSplitter->addWidget( mWCentralPart );

  setCentralWidget( mWSplitter );


  //==========================================
  //      Second layer of layout
  // 1. Left part
  // 1.1. Play list
  mWLeftPlayList = new CsVisualPlayList( mPlayList );
  mWLeftPart->addWidget( mWLeftPlayList );

  // 1.2. Part list
  mWLeftPartList = new CsVisualPartList( mPlayList );
  mWLeftPart->addWidget( mWLeftPartList );

  // We begin from play list
  mWLeftPart->setCurrentWidget( mWLeftPlayList );

  // Bring up part list when it selected in play list
  connect( mWLeftPlayList, &CsVisualPlayList::selectPart, this, [this] ( int partIndex ) {
    mWLeftPart->setCurrentWidget( mWLeftPartList );
    mWLeftPartList->setPart( partIndex );
    });

  // Bring up play list when "back" pressed in part list
  connect( mWLeftPartList, &CsVisualPartList::clickBack, this, [this] () { mWLeftPart->setCurrentWidget( mWLeftPlayList ); } );

  // When repoClient update play list we bring up play list
  connect( repoClient, &CsRepoClient::playlistChanged, this, [this] () {
    mWLeftPlayList->playListUpgrade();
    mWLeftPart->setCurrentWidget( mWLeftPlayList );
    if( mWLeftPart->isHidden() ) {
      mWLeftPart->show();
      if( width() < height() )
        mWCentralPart->hide();
      }
    });

  // When in part list composition selected we load it to current composition
  connect( mWLeftPartList, &CsVisualPartList::compositionClicked, this, [this] (const QString &compositionId) {
    if( mComposition.isDirty() ) {
      mPlayList.compositionSet( mComposition );
      //mComposition.fileSave();
      }
    mComposition.fileLoad( compositionId );
    //mComposition.settingsRead( mPlayList.composition(compositionId) );
    mWCentralScoreTrain->compositionChanged();
    });


  // 2. Central part
  // 2.1. Score view and train
  mWCentralScoreTrain = new CsVisualScoreTrain( mComposition );
  mWCentralPart->addWidget( mWCentralScoreTrain );

  QToolBar *tlBar = addToolBar( QString("ToolBar") );
  tlBar->setIconSize( QSize(32,32) );
  tlBar->setFloatable(false);
  tlBar->setMovable(false);
  tlBar->addAction( QIcon(QString(":/pic/androidMenu.png")), tr("Menu"), this, [this] () {
    //Toggle left part view
    if( mWLeftPart->isHidden() ) {
      mWLeftPart->show();
      if( width() < height() )
        mWCentralPart->hide();
      }
    else {
      mWLeftPart->hide();
      mWCentralPart->show();
      }
    });

  //Restore splitter positions
  QSettings s;
//  if( s.contains(QString(KEY_MAIN_SPLITTER)) )
//    mWSplitter->restoreState( s.value(QString(KEY_MAIN_SPLITTER)).toByteArray() );

#if 0
  QString str("1");
#ifdef Q_OS_ANDROID
  if(QAndroidJniObject::isClassAvailable("CsMidiAndroid")) {
    QAndroidJniObject someJavaObject = QAndroidJniObject("CsMidiAndroid","(Landroid/content/Context;)V", QtAndroid::androidContext().object() );
    str = someJavaObject.callObjectMethod<jstring>("sayHello").toString();
    }
  else {
    str = QStringLiteral("SOME JAVA CLASS UNAVAIABLE!");
    }
#endif

  setCentralWidget( new QLabel( str )  );
#endif
  }

CsAndroidWinMain::~CsAndroidWinMain()
  {
  }

