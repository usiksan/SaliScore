#include "CsWinMain.h"
#include "import/saliScore/CsImportSaliScore.h"

#include <QSettings>
#include <QGuiApplication>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>


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
  if( s.contains(QString(KEY_MAIN_SPLITTER)) )
    mWSplitter->restoreState( s.value(QString(KEY_MAIN_SPLITTER)).toByteArray() );

  if( s.contains(QStringLiteral(KEY_WMAIN_MAX)) ) {
    if( !s.value(QStringLiteral(KEY_WMAIN_MAX)).toBool() )
      //Restore main window size
      resize( s.value(QStringLiteral(KEY_WMAIN_SIZE)).toSize() );
    }


  createMenu();

  //Create imports
  mImportManager.registerImport( new CsImportSaliScore() );

  //Clipboard notification
  //connect( QGuiApplication::clipboard(), &QClipboard::changed, this, &CsWinMain::onClipboardChanged );

  //Notification on tab changes
  connect( mWEditors, &QTabWidget::currentChanged, this, [this] ( int index ) { if( auto page = editor(index) ) page->activate(); } );

  }

CsWinMain::~CsWinMain()
  {
  }





void CsWinMain::cmFileNew()
  {
  static int defaultIndex = 0;
  QString path(CS_DEFAULT_FILE_NAME);
  if( defaultIndex > 0 )
    path += QString::number(defaultIndex);
  defaultIndex++;
  CsComposition com;
  appendEditor( new CsWinScore( path, com ) );
  }



void CsWinMain::cmFileOpen()
  {
  QString fname = QFileDialog::getOpenFileName( this, tr("Select file to open"), QString{}, mImportManager.formats() );
  fileOpen( fname );
  }




void CsWinMain::cmFileLoad()
  {

  }

void CsWinMain::cmFileImport()
  {

  }

void CsWinMain::cmFileSave()
  {
  fileSaveIndex( mWEditors->currentIndex() );
  }

void CsWinMain::cmFileSaveAs()
  {
  fileSaveAsIndex( mWEditors->currentIndex() );
  }

void CsWinMain::cmFileSaveAll()
  {
  for( int i = 0; i < mWEditors->count(); i++ )
    fileSaveIndex( i );
  }

void CsWinMain::cmFileClose()
  {
  fileCloseIndex( mWEditors->currentIndex() );
  }


void CsWinMain::cmFileCloseAll()
  {
  for( int i = mWEditors->count() - 1; i >= 0; i-- )
    fileCloseIndex( i );
  }


void CsWinMain::cmFilePrevious()
  {
  QAction *action = qobject_cast<QAction *>(sender());
  if( action )
    fileOpen( action->data().toString() );
  }



void CsWinMain::cmViewEditor()
  {
  actionViewEditor->setChecked(true);
  actionViewTrain->setChecked(false);
  actionViewKaraoke->setChecked(false);
  }

void CsWinMain::cmViewTrain()
  {

  }

void CsWinMain::cmViewKaraoke()
  {

  }




void CsWinMain::cmHelpContent()
  {

  }

void CsWinMain::cmHelpAbout()
  {
  QMessageBox::about( this, tr("About SaliScore"), tr("SaliLAB simple score edit, training and karaoke programm\n SaliScore version %1.%2").arg( CS_VERSION_MAJOR ).arg( CS_VERSION_MINOR ) );
  }

void CsWinMain::cmHelpWeb()
  {
  QDesktopServices::openUrl( QUrl(QString(CS_ORGANIZATION_DOMAIN)) );
  }

void CsWinMain::cmHelpRegistration()
  {

  }







void CsWinMain::closeEvent(QCloseEvent *ev)
  {
  //Try close all files
  cmFileCloseAll();

  //Test, it is all editors are closed
  bool editorPresent = false;
  for( int i = 0; i < mWEditors->count() && !editorPresent; i++ )
    if( dynamic_cast<CsWinPage*>( mWEditors->widget(i) ) != nullptr )
      editorPresent = true;

  //If all closed
  if( !editorPresent ) {
    //Save settings: main window maximisation and splitter position
    QSettings s;
    s.setValue( QStringLiteral(KEY_WMAIN_MAX), isMaximized() );
    s.setValue( QStringLiteral(KEY_WMAIN_SIZE), size() );
    s.setValue( QStringLiteral(KEY_MAIN_SPLITTER), mWSplitter->saveState() );
    ev->accept();
    }
  else ev->ignore();
  }



//!
//! \brief activeEditor Returns current actived editor
//! \return             Current actived editor or nullptr if none
//!
CsWinPage *CsWinMain::activeEditor() const
  {
  return dynamic_cast<CsWinPage*>( mWEditors->currentWidget() );
  }




//!
//! \brief editor Retrive editor by tab index
//! \param index  Index of tab which editor need to be retrieved
//! \return       Editor by tab index or nullptr if no editor in this index
//!
CsWinPage *CsWinMain::editor(int index) const
  {
  //Check index bound
  if( index < 0 || index >= mWEditors->count() )
    return nullptr;
  return dynamic_cast<CsWinPage*>( mWEditors->widget(index) );
  }




//!
//! \brief fileSaveIndex
//! \param index
//!
void CsWinMain::fileSaveIndex(int index)
  {
  CsWinPage *ed = editor( index );
  if( ed == nullptr ) return;

  //Test if file name yet not assigned
  if( ed->name().startsWith(QStringLiteral(CS_DEFAULT_FILE_NAME)) )
    fileSaveAsIndex( index );
  else
    ed->cmFileSave( ed->path() );
  }



//!
//! \brief fileSaveAsIndex Save file which editor on index tab
//! \param index           Tab index for editor
//!
void CsWinMain::fileSaveAsIndex(int index)
  {
  CsWinPage *ed = editor( index );
  if( ed == nullptr ) return;

  QString title = ed->path();
  title = QFileDialog::getSaveFileName(this, tr("Save File"), title, QString("SaliScore Files (*%1)").arg(ed->extension()) );
  if( title.isEmpty() ) return;
  ed->setPath( title );
  mWEditors->setTabText( index, ed->name() );
  mWEditors->setTabToolTip( index, ed->path() );
  updateRecentFiles( ed->path() );

  //Сохранить файл
  fileSaveIndex( index );
  }




//!
//! \brief fileCloseIndex Closes file editor with index tab
//! \param index          Tab index editor which need to be closed
//!
void CsWinMain::fileCloseIndex(int index)
  {
  CsWinPage *ed = editor( index );
  if( ed == nullptr ) return;

  if( ed->isDirty() ) {
    int r = QMessageBox::question( this, tr("Warning!"), tr("File \"%1\" is modified! Do you want to save changes?").arg(ed->path()), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
    if( r == QMessageBox::Cancel ) return;
    if( r == QMessageBox::Yes ) {
      fileSaveIndex(index);
      if( ed->isDirty() )
        //Saving unsuccessfull
        return;
      }
    }
  mWEditors->removeTab(index);
  ed->deleteLater();
  }




//!
//! \brief fileOpen Open file with path
//! \param path     Path of file
//!
void CsWinMain::fileOpen(const QString path)
  {
  //If empty path nothing done
  if( path.isEmpty() )
    return;

  //Try find already opened file
  for( int i = 0; i < mWEditors->count(); i++ ) {
    CsWinPage *ed = editor(i);
    if( ed != nullptr ) {
      if( ed->path() == path ) {
        //File with this path found
        mWEditors->setCurrentIndex(i);
        return;
        }
      }
    }

  bool ok = false;
  CsComposition composition = mImportManager.readFile( path, ok );
  if( ok ) {
    //Successfull import
    CsWinScore *winScore = new CsWinScore( path, composition );
    appendEditor( winScore );
    updateRecentFiles( winScore->path() );
    }

  }




//!
//! \brief appendEditor Appends editor to editors tab
//! \param editor       Appended editor
//!
void CsWinMain::appendEditor(CsWinPage *editor)
  {
  int index = mWEditors->addTab( editor, editor->name() );
  mWEditors->setTabToolTip( index, editor->path() );
  mWEditors->setCurrentIndex( index );
  }




//!
//! \brief updateRecentFiles Append file to recent file list
//! \param path              File path to append to list
//!
void CsWinMain::updateRecentFiles( const QString &path )
  {
  QSettings s;
  QStringList list;

  //Append file to recent file list
  if( s.contains( KEY_RECENT_FILES ) ) {
    list = s.value( KEY_RECENT_FILES ).toStringList();
    if( !path.isEmpty() ) {
      int i = list.indexOf( path );
      if( i >= 0 )
        //Pull up
        list.removeAt( i );
      else {
        //Remove last
        while( list.count() >= CS_PREVIOUS_FILES_COUNT )
          list.removeLast();
        }
      list.insert( 0, path );
      }
    }
  else if( !path.isEmpty() )
    list.append( path );

  s.setValue( KEY_RECENT_FILES, list );

  //Syncro with menu
  for( int i = 0; i < CS_PREVIOUS_FILES_COUNT; ++i )
    if( i < list.count() ) {
      QFileInfo info( list.at(i) );
      actionFilePrevious[i]->setText( info.completeBaseName() );
      actionFilePrevious[i]->setData( list.at(i) );
      actionFilePrevious[i]->setToolTip( list.at(i) );
      actionFilePrevious[i]->setVisible(true);
      }
    else actionFilePrevious[i]->setVisible(false);

  menuFilePrevious->setEnabled( list.count() != 0 );
  }




void CsWinMain::createMenu()
  {
  menuFile = new QMenu( tr("File") );

  actionFileNew      = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileNew.png")), tr("New file"), this, &CsWinMain::cmFileNew );
  actionFileOpen     = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileOpen.png")), tr("Open file ..."), this, &CsWinMain::cmFileOpen );
  menuFilePrevious   = menuFile->addMenu( tr("Previous files") );
  actionFileLoad     = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileDownload.png")), tr("Load from public cloud..."), this, &CsWinMain::cmFileLoad );
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
  actionFileExit     = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileExit.png")), tr("Exit programm"), this, &CsWinMain::close );

  //Last previous files menu
  for (int i = 0; i < CS_PREVIOUS_FILES_COUNT; ++i) {
    actionFilePrevious[i] = new QAction(this);
    actionFilePrevious[i]->setVisible(false);
    connect( actionFilePrevious[i], &QAction::triggered, this, &CsWinMain::cmFilePrevious );
    menuFilePrevious->addAction( actionFilePrevious[i] );
    }

  menuEdit = new QMenu( tr("Edit") );

  menuView = new QMenu( tr("View") );
  actionViewEditor  = menuView->addAction( QIcon(QStringLiteral(":/pic/exit.png")), tr("Editor mode"), this, [this] () {if( auto editor = activeScore() ) editor->cmViewEditor(); } );
  actionViewTrain   = menuView->addAction( QIcon(QStringLiteral(":/pic/exit.png")), tr("Train mode"), this, [this] () {if( auto editor = activeScore() ) editor->cmViewTrain(); } );
  actionViewKaraoke = menuView->addAction( QIcon(QStringLiteral(":/pic/exit.png")), tr("Karaoke mode"), this, [this] () {if( auto editor = activeScore() ) editor->cmViewTrain(); } );
  actionViewEditor->setCheckable(true);
  actionViewTrain->setCheckable(true);
  actionViewKaraoke->setCheckable(true);

  QActionGroup *group = new QActionGroup(menuView);
  group->addAction( actionViewEditor );
  group->addAction( actionViewTrain );
  group->addAction( actionViewKaraoke );

  menuPlay = new QMenu( tr("Play") );

  menuScore = new QMenu( tr("Score") );

  menuTools = new QMenu( tr("Tools") );

  menuHelp = new QMenu( tr("Help") );
  actionHelpContents     = menuHelp->addAction( QIcon(QString(":/pic/helpContent.png")), tr("Contents"), this, &CsWinMain::cmHelpContent );
  actionHelpAbout        = menuHelp->addAction( QIcon(QString(":/pic/helpAbout.png")), tr("About"), this, &CsWinMain::cmHelpAbout );
  actionHelpWeb          = menuHelp->addAction( QIcon(QString(":/pic/helpWebsite.png")), tr("WEB home page"), this, &CsWinMain::cmHelpWeb );
  actionHelpRegistration = menuHelp->addAction( QIcon(QString(":/pic/helpRegistration.png")), tr("Registration"), this, &CsWinMain::cmHelpRegistration );
  actionHelpHome         = new QAction( QIcon(QString(":/pic/helpHome.png")), QObject::tr("Home help page") );
//  frame->connect( cmHelpHome, &QAction::triggered, frame, &SdWMain::cmHelpContents );
  actionHelpBackward     = new QAction( QIcon(QString(":/pic/helpPrevious.png")), QObject::tr("Backward help page") );
//  frame->connect( cmHelpBackward, &QAction::triggered, frame, &SdWMain::cmHelpBackward );
  actionHelpForward      = new QAction( QIcon(QString(":/pic/helpNext.png")), QObject::tr("Forward help page") );
//  frame->connect( cmHelpForward, &QAction::triggered, frame, &SdWMain::cmHelpForward );


  QMenuBar *bar = menuBar();
  bar->addMenu( menuFile );
  actionMenuEdit = bar->addMenu( menuEdit );
  bar->addMenu( menuView );
  bar->addMenu( menuPlay );
  bar->addMenu( menuScore );
  bar->addMenu( menuTools );
  bar->addMenu( menuHelp );

  updateRecentFiles( QString{} );

  barMain = new QToolBar( tr("Files") );
  barMain->addAction( actionFileNew );
  barMain->addAction( actionFileOpen );
  barMain->addAction( actionFileSave );
  addToolBar( barMain );

  barEditor   = new QToolBar( tr("Editor") );
  barTrain    = new QToolBar( tr("Train") );
  barKaraoke  = new QToolBar( tr("Karaoke") );
  barPlayList = new QToolBar( tr("PlayList") );

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
QToolBar   *CsWinMain::barPlayList;


QActionPtr  CsWinMain::actionFileNew;
QActionPtr  CsWinMain::actionFileOpen;
QActionPtr  CsWinMain::actionFileLoad;
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
QActionPtr  CsWinMain::actionHelpAbout;
QActionPtr  CsWinMain::actionHelpWeb;
QActionPtr  CsWinMain::actionHelpRegistration;
QActionPtr  CsWinMain::actionHelpHome;
QActionPtr  CsWinMain::actionHelpBackward;
QActionPtr  CsWinMain::actionHelpForward;

QActionPtr  CsWinMain::actionGuiderCapture;
QActionPtr  CsWinMain::actionGuiderPause;
