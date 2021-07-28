#include "CsWinMain.h"

#include <QSettings>
#include <QGuiApplication>


CsWinMain::CsWinMain(QWidget *parent) :
  QMainWindow(parent)
  {
  //At central part - wiziwig editors
  mWEditors     = new QTabWidget();
  mWEditors->setTabsClosable(true);

  //At right part - help
  mWHelp = new CsWinHelp();
  mWHelp->setMinimumWidth(200);

  mWSplitter    = new QSplitter();
  mWSplitter->addWidget( mWEditors );
  mWSplitter->addWidget( mWHelp );
  mWHelp->hide();

  setCentralWidget( mWSplitter );

  //Restore splitter positions
  QSettings s;
  if( s.contains(QString(SDK_MAIN_SPLITTER)) )
    mWSplitter->restoreState( s.value(QString(SDK_MAIN_SPLITTER)).toByteArray() );


  //Clipboard notification
  //connect( QGuiApplication::clipboard(), &QClipboard::changed, this, &CsWinMain::onClipboardChanged );

  }

CsWinMain::~CsWinMain()
  {
  }

