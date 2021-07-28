#include "CsWinMain.h"

#include <QSettings>
#include <QGuiApplication>
#include <QMenu>
#include <QMenuBar>
#include <QCloseEvent>


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


  createMenu();
  //Clipboard notification
  //connect( QGuiApplication::clipboard(), &QClipboard::changed, this, &CsWinMain::onClipboardChanged );

  }

CsWinMain::~CsWinMain()
  {
  }




//!
//! \brief activeEditor Returns current actived editor
//! \return             Current actived editor or nullptr if none
//!
CsWinEditor *CsWinMain::activeEditor() const
  {
  return dynamic_cast<CsWinEditor*>( mWEditors->currentWidget() );
  }

void CsWinMain::cmFileNew()
  {

  }

void CsWinMain::cmFileOpen()
  {

  }

void CsWinMain::cmFileLoad()
  {

  }

void CsWinMain::cmFileImport()
  {

  }

void CsWinMain::cmFileSave()
  {

  }

void CsWinMain::cmFileSaveAs()
  {

  }

void CsWinMain::cmFileSaveAll()
  {

  }

void CsWinMain::cmFileClose()
  {

  }

void CsWinMain::cmFileCloseAll()
  {

  }

void CsWinMain::cmFileExit()
  {

  }

void CsWinMain::cmFilePrevious()
  {
  QAction *action = qobject_cast<QAction *>(sender());
  if( action )
    fileOpen( action->data().toString() );
  }







void CsWinMain::closeEvent(QCloseEvent *ev)
  {
  //Try close all files
  cmFileCloseAll();

  //Test, it is all editors are closed
  bool editorPresent = false;
  for( int i = 0; i < mWEditors->count() && !editorPresent; i++ )
    if( dynamic_cast<CsWinEditor*>( mWEditors->widget(i) ) != nullptr )
      editorPresent = true;

  //If all closed
  if( !editorPresent ) {
    //Save settings: main window maximisation and splitter position
    QSettings s;
    s.setValue( QString(SDK_WMAIN_MAX), isMaximized() );
    s.setValue( QString(SDK_MAIN_SPLITTER), mWSplitter->saveState() );
    ev->accept();
    }
  else ev->ignore();
  }




//!
//! \brief fileOpen Open file with path
//! \param path     Path of file
//!
void CsWinMain::fileOpen(const QString path)
  {

  }




void CsWinMain::createMenu()
  {
  menuFile = new QMenu( tr("File") );

  actionFileNew      = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileNew.png")), tr("New file"), this, &CsWinMain::cmFileNew );
  actionFileOpen     = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileOpen.png")), tr("Open file ..."), this, &CsWinMain::cmFileOpen );
  menuFilePrevious   = menuFile->addMenu( tr("Previous files") );
  actionFileLoad     = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileDownload.png")), tr("Load from public cloud..."), this, &CsWinMain::cmFileLoad );
  actionFileImport   = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileImport.png")), tr("Import..."), this, &CsWinMain::cmFileImport );
  menuFile->addSeparator();
  actionFileSave     = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileSave.png")), tr("Save file"), this, &CsWinMain::cmFileSave );
  actionFileSaveAs   = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileSaveAs.png")), tr("Save file as..."), this, &CsWinMain::cmFileSaveAs );
  actionFileSaveAll  = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileSaveAll.png")), tr("Save all files"), this, &CsWinMain::cmFileSaveAll );
  actionFilePublic   = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileUpload.png")), tr("Public to cloud..."), this, [this] () { if( auto editor = activeEditor() ) editor->cmFilePublic(); } );
  actionFileExport   = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileExport.png")), tr("Export..."), this,  [this] () { if( auto editor = activeEditor() ) editor->cmFileExport(); } );
  menuFile->addSeparator();
  actionFilePrint    = menuFile->addAction( QIcon(QStringLiteral(":/pic/filePrint.png")), tr("Print..."), this, [this] () { if( auto editor = activeEditor() ) editor->cmFilePrint(); } );
  menuFile->addSeparator();
  actionFileClose    = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileClose.png")), tr("Close file"), this, &CsWinMain::cmFileClose );
  actionFileCloseAll = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileCloseAll.png")), tr("Close all files"), this, &CsWinMain::cmFileCloseAll );
  menuFile->addSeparator();
  actionFileExit     = menuFile->addAction( QIcon(QStringLiteral(":/pic/exit.png")), tr("Exit programm"), this, &CsWinMain::cmFileExit );

  //Last previous files menu
  for (int i = 0; i < CS_PREVIOUS_FILES_COUNT; ++i) {
    actionFilePrevious[i] = new QAction(this);
    actionFilePrevious[i]->setVisible(false);
    connect( actionFilePrevious[i], &QAction::triggered, this, &CsWinMain::cmFilePrevious );
    menuFilePrevious->addAction( actionFilePrevious[i] );
    }

  menuEdit = new QMenu( tr("Edit") );

  menuView = new QMenu( tr("View") );

  menuPlay = new QMenu( tr("Play") );

  menuScore = new QMenu( tr("Score") );

  menuTools = new QMenu( tr("Tools") );

  menuHelp = new QMenu( tr("Help") );

  QMenuBar *bar = menuBar();
  bar->addMenu( menuFile );
  actionMenuEdit = bar->addMenu( menuEdit );
  bar->addMenu( menuView );
  bar->addMenu( menuPlay );
  bar->addMenu( menuScore );
  bar->addMenu( menuTools );
  bar->addMenu( menuHelp );

  }


//=============================================================================================================================
//                                     Commands
QMenu *CsWinMain::menuFile;
QMenu *CsWinMain::menuFilePrevious;
QMenu *CsWinMain::menuEdit;
QMenu *CsWinMain::menuView;
QMenu *CsWinMain::menuPlay;
QMenu *CsWinMain::menuScore;
QMenu *CsWinMain::menuTools;
QMenu *CsWinMain::menuHelp;

QActionPtr  CsWinMain::actionMenuEdit;

//Tool bars for editor command
QToolBar   *CsWinMain::barMain;
QToolBar   *CsWinMain::barEditor;
QToolBar   *CsWinMain::barTrain;
QToolBar   *CsWinMain::barKaraoke;


QActionPtr  CsWinMain::actionFileNew;
QActionPtr  CsWinMain::actionFileOpen;
QActionPtr  CsWinMain::actionFileLoad;
QActionPtr  CsWinMain::actionFileImport;
QActionPtr  CsWinMain::actionFileSave;
QActionPtr  CsWinMain::actionFileSaveAs;
QActionPtr  CsWinMain::actionFileSaveAll;
QActionPtr  CsWinMain::actionFilePublic;
QActionPtr  CsWinMain::actionFileExport;
QActionPtr  CsWinMain::actionFilePrint;
QActionPtr  CsWinMain::actionFileClose;
QActionPtr  CsWinMain::actionFileCloseAll;
QActionPtr  CsWinMain::actionFileExit;

QActionPtr  CsWinMain::actionFilePrevious[CS_PREVIOUS_FILES_COUNT];

QActionPtr  CsWinMain::actionEditUndo;
QActionPtr  CsWinMain::actionEditRedo;
QActionPtr  CsWinMain::actionEditCut;
QActionPtr  CsWinMain::actionEditCopy;
QActionPtr  CsWinMain::actionEditPaste;
QActionPtr  CsWinMain::actionEditDelete;
QActionPtr  CsWinMain::actionEditSelectAll;
QActionPtr  CsWinMain::actionEditUnSelect;

QActionPtr  CsWinMain::actionViewEditor;
QActionPtr  CsWinMain::actionViewTrain;
QActionPtr  CsWinMain::actionViewKaraoke;

QActionPtr  CsWinMain::actionPlayStart;
QActionPtr  CsWinMain::actionPlayPause;
QActionPtr  CsWinMain::actionPlayStop;
QActionPtr  CsWinMain::actionPlayTrain;

QActionPtr  CsWinMain::actionScoreRemark;
QActionPtr  CsWinMain::actionScoreRemarkManage;
QActionPtr  CsWinMain::actionScoreChordManage;
QActionPtr  CsWinMain::actionScoreNoteManage;

QActionPtr  CsWinMain::actionToolsOption;

QActionPtr  CsWinMain::actionHelpContents;
QActionPtr  CsWinMain::actionHelpIndex;
QActionPtr  CsWinMain::actionHelpAbout;
QActionPtr  CsWinMain::actionHelpRegistration;
QActionPtr  CsWinMain::actionHelpHome;
QActionPtr  CsWinMain::actionHelpBackward;
QActionPtr  CsWinMain::actionHelpForward;

QActionPtr  CsWinMain::actionGuiderCapture;
QActionPtr  CsWinMain::actionGuiderPause;
