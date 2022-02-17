#include "config.h"
#include "CsAndroidWinMain.h"
#include "CsVisualPlayList.h"
#include "CsVisualPartList.h"
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

  //At left side - play list
  mWLeftPart = new QStackedWidget();

  //At central part - wiziwig editors
  mWCentralPart = new QStackedWidget();

  mWSplitter    = new QSplitter();
  mWSplitter->addWidget( mWLeftPart );
  mWSplitter->addWidget( mWCentralPart );

  setCentralWidget( mWSplitter );


  mWLeftPlayList = new CsVisualPlayList( mPlayList );
  mWLeftPart->addWidget( mWLeftPlayList );

  mWLeftPartList = new CsVisualPartList( mPlayList );
  mWLeftPart->addWidget( mWLeftPartList );

  mWLeftPart->setCurrentWidget( mWLeftPlayList );
  connect( mWLeftPlayList, &CsVisualPlayList::selectPart, this, [this] ( int partIndex ) {
    mWLeftPart->setCurrentWidget( mWLeftPartList );
    mWLeftPartList->setPart( partIndex );
    });
  connect( mWLeftPartList, &CsVisualPartList::clickBack, this, [this] () { mWLeftPart->setCurrentWidget( mWLeftPlayList ); } );
  connect( repoClient, &CsRepoClient::playlistChanged, this, [this] () {
    mWLeftPlayList->playListUpgrade();
    mWLeftPart->setCurrentWidget( mWLeftPlayList );
    if( mWLeftPart->isHidden() ) {
      mWLeftPart->show();
      if( width() < height() )
        mWCentralPart->hide();
      }
    });

  mWCentralPart->addWidget( new CsVisualAbstractList() );

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

