#include "CsWinMain.h"
#include "import/saliScore/CsImportSaliScore.h"
#include "import/text/CsImportText.h"
#include "midi/CsMidiSequencer.h"
#include "CsDlgDefRemark.h"
#include "CsDlgDefChord.h"
#include "CsDlgDefNote.h"
#include "CsDlgDefTranslation.h"
#include "CsDlgRegistration.h"
#include "CsDlgScoreSettings.h"
#include "repo/CsRepoClient.h"

#include <QSettings>
#include <QGuiApplication>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QJsonDocument>
#include <QClipboard>


CsWinMain::CsWinMain(CsPlayList &playList, CsMidiSequencer *midiSequencer, QWidget *parent) :
  QMainWindow(parent),
  mMidiSequencer(midiSequencer),
  mPlayer( mComposition ),
  mPlayList(playList),
  mNotSaved(false)
  {

  //At left side - play list
  mWLeftPart = new QStackedWidget();

  //At central part - wiziwig editors
  mWCentralPart = new QStackedWidget();

  //At right part - help
  mWHelp = new CsWinHelp();
  mWHelp->setMinimumWidth(200);

  mWSplitter    = new QSplitter();
  mWSplitter->addWidget( mWLeftPart );
  mWSplitter->addWidget( mWCentralPart );
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

  //Build edit, train and karaoke windows
  mWCentralPart->addWidget( new CsWinScoreMode( mWinTrain = new CsWinTrain( mComposition, mPlayer ) ) );
  mWCentralPart->addWidget( new CsWinScoreMode( mWinKaraoke = new CsWinKaraoke( mComposition, mPlayer ) ) );
  mWCentralPart->addWidget( new CsWinScoreMode( mWinEditor = new CsWinEditor( mComposition, mPlayer ) ) );


  createMenu();

  //Build play list
  //Play list consist from two view:
  // 0 - tree view play list (actived default)
  // 1 - remote server find system (activate when Load...)
  mWLeftPart->addWidget( buildPlayList() );
  mWLeftPart->addWidget( buildRemoteFind() );
  mWLeftPart->setCurrentIndex(WIN_INDEX_PLAY_LIST);

  //mWCentralPart->addWidget( mWinIntro = new CsWinIntro() );

  //Create imports
  mImportManager.registerImport( new CsImportSaliScore() );
  mImportManager.registerImport( new CsImportText() );

  //Clipboard notification
  //connect( QGuiApplication::clipboard(), &QClipboard::changed, this, &CsWinMain::onClipboardChanged );

  //Notification for tick
  connect( mMidiSequencer, &CsMidiSequencer::tick, this, [this] ( int count ) { mPlayer.next( count ); } );
  connect( mMidiSequencer, &CsMidiSequencer::noteOn, this, [this] ( int note ) { mPlayer.noteOn( note ); } );
  connect( mMidiSequencer, &CsMidiSequencer::midiStart, this, &CsWinMain::cmPlayStart );
  connect( mMidiSequencer, &CsMidiSequencer::midiStop, this, &CsWinMain::cmPlayStop );
  connect( repoClient, &CsRepoClient::playlistChanged, mWPlayList, &CsWinPlayList::buildContent );

  mUpdateTimer.setInterval(100);

  cmViewEditor();

  mWPlayList->buildContent();
  }



CsWinMain::~CsWinMain()
  {
  }





void CsWinMain::cmFileNew()
  {
  if( canCloseEditor() ) {
    mNotSaved = true;
    mComposition.clear();
    mWinEditor->compositionChanged();
    mWinTrain->compositionChanged();
    mWinKaraoke->compositionChanged();
    }
  }




void CsWinMain::cmFileLoad()
  {
  mWLeftPart->setCurrentIndex( WIN_INDEX_REMOTE_FIND );
  }




void CsWinMain::cmFileImport()
  {
  if( canCloseEditor() ) {
    QString fname = QFileDialog::getOpenFileName( this, tr("Select file to open"), QString{}, mImportManager.formats() );
    bool ok = false;
    CsComposition composition = mImportManager.readFile( fname, ok );
    if( ok ) {
      mNotSaved = true;
      mComposition = composition;
      mWinEditor->compositionChanged();
      mWinTrain->compositionChanged();
      mWinKaraoke->compositionChanged();
      }
    }
  }




void CsWinMain::cmFileSave()
  {
  if( mComposition.version() == 0 )
    mComposition.makeCopy();

  if( mComposition.isDirty() )
    mComposition.versionUpdate();

  QFile file( mComposition.header().path() );
  if( (mComposition.isDirty() || mNotSaved) && file.open(QIODevice::WriteOnly) ) {
    //Write contents to file
    file.write( mComposition.toByteArray() );

    //Write completed, reset dirty
    mComposition.dirtyReset();
    mNotSaved = false;
    }

  mWPlayList->settingsSave( mComposition );
  mComposition.stateDirtyReset();
  }



void CsWinMain::cmFileCopy()
  {

  }

void CsWinMain::cmFilePublic()
  {

  }

void CsWinMain::cmFileExport()
  {

  }

void CsWinMain::cmFilePrint()
  {

  }




void CsWinMain::cmEditPasteImport()
  {
  if( canCloseEditor() ) {
    QClipboard *clip = QGuiApplication::clipboard();
    if( !clip->text().isEmpty() ) {
      bool ok = false;
      CsComposition composition = mImportManager.read( clip->text().toUtf8(), ok );
      if( ok ) {
        mNotSaved = true;
        mComposition = composition;
        mWinEditor->compositionChanged();
        mWinTrain->compositionChanged();
        mWinKaraoke->compositionChanged();
        }
      }
    }
  }




void CsWinMain::cmEditSettings()
  {
  CsDlgScoreSettings dlg( mComposition, this );
  if( dlg.exec() )
    mWinEditor->update();
  }





void CsWinMain::cmViewEditor()
  {
  mWCentralPart->setCurrentWidget( mWinEditor->winScoreMode() );
  //Hide toolbars
  barPlayList->hide();
  barTrain->hide();
  barKaraoke->hide();
  //Show active bar
  barEditor->show();
  actionViewEditor->setChecked(true);
  //Show edit menu
  actionMenuEditDisabled->setVisible(false);
  actionMenuEdit->setVisible(true);
  //Hide train menu
  actionMenuTrain->setVisible(false);
  //Activate karaoke
  mWinEditor->activate();
  }





void CsWinMain::cmViewTrain()
  {
  mWCentralPart->setCurrentWidget( mWinTrain->winScoreMode() );
  //Hide toolbars
  barPlayList->hide();
  barKaraoke->hide();
  barEditor->hide();
  //Show active bar
  barTrain->show();
  actionViewTrain->setChecked(true);
  //Hide edit menu
  actionMenuEdit->setVisible(false);
  actionMenuEditDisabled->setVisible(true);
  //Show train menu
  actionMenuTrain->setVisible(true);
  //Activate karaoke
  mWinTrain->activate();
  }





void CsWinMain::cmViewKaraoke()
  {
  mWCentralPart->setCurrentWidget( mWinKaraoke->winScoreMode() );
  //Hide toolbars
  barPlayList->hide();
  barTrain->hide();
  barEditor->hide();
  //Show active bar
  barKaraoke->show();
  actionViewKaraoke->setChecked(true);
  //Hide edit menu
  actionMenuEdit->setVisible(false);
  actionMenuEditDisabled->setVisible(true);
  //Hide train menu
  actionMenuTrain->setVisible(false);
  //Activate karaoke
  mWinKaraoke->activate();
  }




void CsWinMain::cmViewRemark()
  {
  CsDlgDefRemark dlgRem( actionViewEditor->isChecked(), mComposition, this );
  dlgRem.fill();
  dlgRem.exec();
  }




void CsWinMain::cmViewChord()
  {
  CsDlgDefChord dlgChord( actionViewEditor->isChecked(), mComposition, this );
  dlgChord.fill();
  dlgChord.exec();
  }




void CsWinMain::cmViewNote()
  {
  CsDlgDefNote dlgNote( actionViewEditor->isChecked(), mComposition, this );
  dlgNote.fill();
  dlgNote.exec();
  }




void CsWinMain::cmViewTranslation()
  {
  CsDlgDefTranslation dlgTranslation( actionViewEditor->isChecked(), mComposition, this );
  dlgTranslation.fill();
  dlgTranslation.exec();
  }





void CsWinMain::cmPlayStart()
  {
  if( CsWinMain::actionViewKaraoke->isChecked() ) {
    mWinKaraoke->playStart();
    connect( &mUpdateTimer, &QTimer::timeout, mWinKaraoke, &CsWinScoreView::viewUpdate );
    }
  else if( CsWinMain::actionViewTrain->isChecked() ) {
    mWinTrain->playStart();
    connect( &mUpdateTimer, &QTimer::timeout, mWinTrain, &CsWinScoreView::viewUpdate );
    }
  else {
    mWinEditor->playStart();
    connect( &mUpdateTimer, &QTimer::timeout, mWinEditor, &CsWinScoreView::viewUpdate );
    }
  mPlayer.start();
  mUpdateTimer.start();
  }





void CsWinMain::cmPlayStop()
  {
  mPlayer.stop();

  //Update view to remove player position
  if( CsWinMain::actionViewKaraoke->isChecked() )
    mWinKaraoke->viewUpdate();
  else if( CsWinMain::actionViewTrain->isChecked() )
    mWinTrain->viewUpdate();
  else
    mWinEditor->viewUpdate();
  mUpdateTimer.stop();
  mUpdateTimer.disconnect();
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
  //Prepare and display registration dialog
  CsDlgRegistration dlg(this);
  //If after registration user yet not registered then exit application
  if( dlg.exec() == 0 && !repoClient->isRegistered() )
    close();
  }







void CsWinMain::closeEvent(QCloseEvent *ev)
  {
  //If can close editor
  if( canCloseEditor() ) {
    //Save settings: main window maximisation and splitter position
    QSettings s;
    s.setValue( QStringLiteral(KEY_WMAIN_MAX), isMaximized() );
    s.setValue( QStringLiteral(KEY_WMAIN_SIZE), size() );
    s.setValue( QStringLiteral(KEY_MAIN_SPLITTER), mWSplitter->saveState() );
    ev->accept();
    }
  else ev->ignore();
  }





void CsWinMain::activateComposition(int partIndex, int compositionIndex)
  {
  if( canCloseEditor() ) {
    QString id = mPlayList.partCompositionId( partIndex, compositionIndex );
    CsCompositionSettings s = mPlayList.composition( id );
    bool ok = false;
    CsComposition composition = mImportManager.readFile( s.path(), ok );
    if( ok ) {
      mNotSaved = false;
      mComposition = composition;
      mComposition.settingsRead( s );
      mWinEditor->compositionChanged();
      mWinTrain->compositionChanged();
      mWinKaraoke->compositionChanged();
      }

    }
  }









//!
//! \brief buildPlayList Builds play list view widget
//! \return              Play list view widget
//!
QWidget *CsWinMain::buildPlayList()
  {
  QVBoxLayout *box = new QVBoxLayout();
  box->addWidget( new QLabel(tr("Play List")) );
  box->addWidget( mWPlayList = new CsWinPlayList( mPlayList ), 100 );

  connect( mWPlayList, &CsWinPlayList::currentItemChanged, this, [this] ( QTreeWidgetItem *current, QTreeWidgetItem *previous ) {
    Q_UNUSED(previous)
    int partIndex;
    int compositionIndex;
    if( mWPlayList->itemPosition( current, partIndex, compositionIndex ) ) {
      //Make activate composition with compositionIndex from part with partIndex
      activateComposition( partIndex, compositionIndex );
      }
    });

  QToolBar *bar = new QToolBar();
  bar->addAction( actionPlMinus );
  bar->addAction( actionPlPlus );
  box->addWidget( bar );

  QWidget *we = new QWidget();
  we->setLayout( box );
  return we;
  }




QWidget *CsWinMain::buildRemoteFind()
  {
  QHBoxLayout *hbox = new QHBoxLayout();
  hbox->addWidget( new QLabel(tr("Find string:")) );
  hbox->addWidget( new QLineEdit() );
  QPushButton *but = new QPushButton( tr("Find") );
  hbox->addWidget( but );

  QVBoxLayout *box = new QVBoxLayout();
  box->addLayout( hbox );
  box->addWidget( mWRemote = new CsWinRemote() );

  QToolBar *bar = new QToolBar();
  bar->addAction( actionFileLoad );
  box->addWidget( bar );

  QWidget *we = new QWidget();
  we->setLayout( box );
  return we;
  }




//!
//! \brief canCloseEditor Check, can be closed editor
//! \return               true - editor can be closed
//!
bool CsWinMain::canCloseEditor()
  {
  if( mComposition.isDirty() ) {
    //Contents is changed
    auto res = QMessageBox::question( this, tr("Warning!"), tr("Composition is changed. Do You want to save changes?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
    if( res == QMessageBox::Yes ) {
      cmFileSave();
      return true;
      }
    if( res == QMessageBox::No )
      return true;
    return false;
    }
  return true;
  }








void CsWinMain::createMenu()
  {
  menuFile = new QMenu( tr("File") );

  actionFileNew      = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileNew.png")), tr("New file"), this, &CsWinMain::cmFileNew );
  actionFileImport   = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileOpen.png")), tr("Open file ..."), this, &CsWinMain::cmFileImport );
  actionFileLoad     = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileCloud.png")), tr("Load from public cloud..."), this, &CsWinMain::cmFileLoad );
  menuFile->addSeparator();
  actionFileSave     = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileSave.png")), tr("Save score"), this, &CsWinMain::cmFileSave );
  actionFileCopy     = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileSaveAs.png")), tr("Make copy score"), this, &CsWinMain::cmFileCopy );
  actionFilePublic   = menuFile->addAction( QIcon(QStringLiteral(":/pic/filePublic.png")), tr("Public to cloud"), this, &CsWinMain::cmFilePublic );
  actionFileExport   = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileExport.png")), tr("Export..."), this,  &CsWinMain::cmFileExport );
  menuFile->addSeparator();
  actionFilePrint    = menuFile->addAction( QIcon(QStringLiteral(":/pic/filePrint.png")), tr("Print..."), this, &CsWinMain::cmFilePrint );
  menuFile->addSeparator();
  actionFileExit     = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileExit.png")), tr("Exit programm"), this, &CsWinMain::close );

  menuEditDisabled = new QMenu( tr("Edit") );
  actionEditPasteImport = menuEditDisabled->addAction( QIcon(QStringLiteral(":/pic/viewEditor.png")), tr("Import from clipboard"), this, &CsWinMain::cmEditPasteImport );

  menuEdit = new QMenu( tr("Edit") );
  menuEdit->addAction( QIcon(QStringLiteral(":/pic/viewEditor.png")), tr("Import from clipboard"), this, &CsWinMain::cmEditPasteImport );
  menuEdit->addSeparator();
  actionEditUndo = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editUndo.png")), tr("Undo last edit operation"), mWinEditor, &CsWinEditor::cmEditUndo );
  actionEditRedo = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editRedo.png")), tr("Redo last undoed operation"), mWinEditor, &CsWinEditor::cmEditRedo );
  menuEdit->addSeparator();
  actionEditCopy = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editCopy.png")), tr("Copy selection to clipboard"), mWinEditor, &CsWinEditor::cmEditCopy );
  actionEditCut  = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editCut.png")), tr("Cut selection to clipboard"), mWinEditor, &CsWinEditor::cmEditCut );
  actionEditPaste = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editPaste.png")), tr("Paste from clipboard"), mWinEditor, &CsWinEditor::cmEditPaste );
  actionEditDelete = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editDelete.png")), tr("Delete slection"), mWinEditor, &CsWinEditor::cmEditDelete );
  menuEdit->addSeparator();
  actionEditSettings = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editSettings.png")), tr("Edit score settings"), this, &CsWinMain::cmEditSettings );

  menuView = new QMenu( tr("View") );
  actionViewEditor  = menuView->addAction( QIcon(QStringLiteral(":/pic/viewEditor.png")), tr("Editor mode"), this, &CsWinMain::cmViewEditor );
  actionViewTrain   = menuView->addAction( QIcon(QStringLiteral(":/pic/viewTrain.png")), tr("Train mode"), this, &CsWinMain::cmViewTrain );
  actionViewKaraoke = menuView->addAction( QIcon(QStringLiteral(":/pic/viewKaraoke.png")), tr("Karaoke mode"), this, &CsWinMain::cmViewKaraoke );
  actionViewEditor->setCheckable(true);
  actionViewTrain->setCheckable(true);
  actionViewKaraoke->setCheckable(true);
  menuView->addSeparator();
  actionViewRemark      = menuView->addAction( QIcon(QStringLiteral(":/pic/viewRemark.png")), tr("Remark manage..."), this, &CsWinMain::cmViewRemark );
  actionViewChord       = menuView->addAction( QIcon(QStringLiteral(":/pic/viewChord.png")), tr("Chord manage..."), this, &CsWinMain::cmViewChord );
  actionViewNote        = menuView->addAction( QIcon(QStringLiteral(":/pic/viewNote.png")), tr("Note manage..."), this, &CsWinMain::cmViewNote );
  actionViewTranslation = menuView->addAction( QIcon(QStringLiteral(":/pic/viewTranslation.png")), tr("Translation manage..."), this, &CsWinMain::cmViewTranslation );

  QActionGroup *group = new QActionGroup(menuView);
  group->addAction( actionViewEditor );
  group->addAction( actionViewTrain );
  group->addAction( actionViewKaraoke );

  menuPlay = new QMenu( tr("Play") );
  actionPlayStart = menuPlay->addAction( QIcon(QStringLiteral(":/pic/playStart.png")), tr("Start"), this, &CsWinMain::cmPlayStart );
  actionPlayPause = menuPlay->addAction( QIcon(QStringLiteral(":/pic/playPause.png")), tr("Pause"), this, [this] () {  mPlayer.pause(); } );
  actionPlayStop  = menuPlay->addAction( QIcon(QStringLiteral(":/pic/playStop.png")), tr("Stop"), this, &CsWinMain::cmPlayStop );

  menuTrain = new QMenu( tr("Train") );
  actionFragmentTrain = menuTrain->addAction( QIcon(QStringLiteral(":/pic/playTrain.png")), tr("Fragment train"), mWinTrain, &CsWinTrain::cmFragmentTrain );
  actionFragment0     = menuTrain->addAction( QIcon(QStringLiteral(":/pic/fragment0.png")), tr("All composition"), mWinTrain, &CsWinTrain::cmFragment0 );
  actionFragment1     = menuTrain->addAction( QIcon(QStringLiteral(":/pic/fragment1.png")), tr("Fragment 1"), mWinTrain, &CsWinTrain::cmFragment1 );
  actionFragment2     = menuTrain->addAction( QIcon(QStringLiteral(":/pic/fragment2.png")), tr("Fragment 2"), mWinTrain, &CsWinTrain::cmFragment2 );
  actionFragmentStart = menuTrain->addAction( QIcon(QStringLiteral(":/pic/fragmentStart.png")), tr("Fix fragment start"), mWinTrain, &CsWinTrain::cmFragmentStart );
  actionFragmentStop  = menuTrain->addAction( QIcon(QStringLiteral(":/pic/fragmentStop.png")), tr("Fix fragment stop"), mWinTrain, &CsWinTrain::cmFragmentStop );
  actionFragment0->setCheckable(true);
  actionFragment1->setCheckable(true);
  actionFragment2->setCheckable(true);

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
  actionMenuEditDisabled = bar->addMenu( menuEditDisabled );
  actionMenuEdit = bar->addMenu( menuEdit );
  bar->addMenu( menuView );
  bar->addMenu( menuPlay );
  actionMenuTrain = bar->addMenu( menuTrain );
  bar->addMenu( menuScore );
  bar->addMenu( menuTools );
  bar->addMenu( menuHelp );

  barMain = new QToolBar( tr("Files") );
  barMain->addAction( actionFileNew );
  barMain->addAction( actionFileCopy );
  barMain->addAction( actionFileSave );
  addToolBar( barMain );

  barEditor   = new QToolBar( tr("Editor") );
  barEditor->addAction( actionViewEditor );
  barEditor->addAction( actionViewTrain );
  barEditor->addAction( actionViewKaraoke );
  barEditor->addSeparator();
  barEditor->addAction( actionEditPaste );
  barEditor->addAction( actionEditCopy );
  barEditor->addAction( actionEditCut );
  barEditor->addSeparator();
  barEditor->addAction( actionEditUndo );
  barEditor->addAction( actionEditRedo );
  barEditor->addSeparator();
  barEditor->addAction( actionPlayStart );
  barEditor->addAction( actionPlayPause );
  barEditor->addAction( actionPlayStop );
  addToolBar( barEditor );

  barTrain    = new QToolBar( tr("Train") );
  barTrain->addAction( actionViewEditor );
  barTrain->addAction( actionViewTrain );
  barTrain->addAction( actionViewKaraoke );
  barTrain->addSeparator();
  barTrain->addAction( actionPlayStart );
  barTrain->addAction( actionPlayPause );
  barTrain->addAction( actionPlayStop );
  barTrain->addSeparator();
  barTrain->addAction( actionFragmentTrain );
  barTrain->addAction( actionFragment0 );
  barTrain->addAction( actionFragment1 );
  barTrain->addAction( actionFragment2 );
  barTrain->addAction( actionFragmentStart );
  barTrain->addAction( actionFragmentStop );
  addToolBar( barTrain );

  barKaraoke  = new QToolBar( tr("Karaoke") );
  barKaraoke->addAction( actionViewEditor );
  barKaraoke->addAction( actionViewTrain );
  barKaraoke->addAction( actionViewKaraoke );
  barKaraoke->addSeparator();
  barKaraoke->addAction( actionPlayStart );
  barKaraoke->addAction( actionPlayPause );
  barKaraoke->addAction( actionPlayStop );
  addToolBar( barKaraoke );

  barPlayList = new QToolBar( tr("PlayList") );

  //At start hide all bars except main
  barEditor->hide();
  barTrain->hide();
  barKaraoke->hide();

  //QMenu *menu = new QMenu("hidden");
  actionPlMinus = new QAction( QIcon(QStringLiteral(":/pic/plMinus.png")), tr("Remove composition from list...") );
  actionPlPlus  = new QAction( QIcon(QStringLiteral(":/pic/plPlus.png")), tr("Add composition from server") );
  connect( actionPlPlus, &QAction::triggered, this, &CsWinMain::cmFileLoad );
  //actionPlLoad;
  actionPlPlayList = new QAction( QIcon(QStringLiteral(":/pic/plList.png")), tr("Switch to Play list") );

  }


//=============================================================================================================================
//                                     Commands
QMenu *CsWinMain::menuFile;
QMenu *CsWinMain::menuEditDisabled;
QMenu *CsWinMain::menuEdit;
QMenu *CsWinMain::menuView;
QMenu *CsWinMain::menuPlay;
QMenu *CsWinMain::menuTrain;
QMenu *CsWinMain::menuScore;
QMenu *CsWinMain::menuTools;
QMenu *CsWinMain::menuHelp;

QActionPtr  CsWinMain::actionMenuEditDisabled;
QActionPtr  CsWinMain::actionMenuEdit;
QActionPtr  CsWinMain::actionMenuTrain;

//Tool bars for editor command
QToolBar   *CsWinMain::barMain;
QToolBar   *CsWinMain::barEditor;
QToolBar   *CsWinMain::barTrain;
QToolBar   *CsWinMain::barKaraoke;
QToolBar   *CsWinMain::barPlayList;


QActionPtr  CsWinMain::actionFileNew;
QActionPtr  CsWinMain::actionFileImport;
QActionPtr  CsWinMain::actionFileCopy;
QActionPtr  CsWinMain::actionFileLoad;
QActionPtr  CsWinMain::actionFileSave;
QActionPtr  CsWinMain::actionFilePublic;
QActionPtr  CsWinMain::actionFileExport;
QActionPtr  CsWinMain::actionFilePrint;
QActionPtr  CsWinMain::actionFileExit;


QActionPtr  CsWinMain::actionEditUndo;
QActionPtr  CsWinMain::actionEditRedo;
QActionPtr  CsWinMain::actionEditCut;
QActionPtr  CsWinMain::actionEditCopy;
QActionPtr  CsWinMain::actionEditPaste;
QActionPtr  CsWinMain::actionEditPasteImport;
QActionPtr  CsWinMain::actionEditDelete;
QActionPtr  CsWinMain::actionEditSelectAll;
QActionPtr  CsWinMain::actionEditUnSelect;
QActionPtr  CsWinMain::actionEditSettings;

QActionPtr  CsWinMain::actionViewEditor;
QActionPtr  CsWinMain::actionViewTrain;
QActionPtr  CsWinMain::actionViewKaraoke;
QActionPtr  CsWinMain::actionViewRemark;
QActionPtr  CsWinMain::actionViewChord;
QActionPtr  CsWinMain::actionViewNote;
QActionPtr  CsWinMain::actionViewTranslation;

QActionPtr  CsWinMain::actionPlayStart;
QActionPtr  CsWinMain::actionPlayPause;
QActionPtr  CsWinMain::actionPlayStop;

QActionPtr  CsWinMain::actionFragmentTrain;
QActionPtr  CsWinMain::actionFragment0;
QActionPtr  CsWinMain::actionFragment1;
QActionPtr  CsWinMain::actionFragment2;
QActionPtr  CsWinMain::actionFragmentStart;
QActionPtr  CsWinMain::actionFragmentStop;

QActionPtr  CsWinMain::actionScoreRemark;
QActionPtr  CsWinMain::actionScoreRemarkManage;
QActionPtr  CsWinMain::actionScoreChordManage;
QActionPtr  CsWinMain::actionScoreNoteManage;


QActionPtr  CsWinMain::actionPlMinus;
QActionPtr  CsWinMain::actionPlPlus;
QActionPtr  CsWinMain::actionPlLoad;
QActionPtr  CsWinMain::actionPlPlayList;

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
