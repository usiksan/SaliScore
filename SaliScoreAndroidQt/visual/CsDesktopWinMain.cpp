
#include "CsConfig.h"
#include "CsDesktopWinMain.h"
#include "CsVisualPlayList.h"
#include "CsVisualPartList.h"
#include "CsVisualScoreTrain.h"
#include "CsVisualScoreEdit.h"
#include "CsVisualScoreKaraoke.h"
#include "CsVisualPiano.h"
#include "CsDlgRegistration.h"
#include "repo/CsRepoClient.h"
#include "import/saliScore/CsImportSaliScore.h"
#include "import/text/CsImportText.h"
#include "midiKeyboard/CsMidiKeyboardLinux.h"

#include <QDebug>
#include <QGuiApplication>
#include <QVBoxLayout>
#include <QToolBar>
#include <QToolButton>
#include <QSettings>
#include <QMenu>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QClipboard>
#include <QMenuBar>


CsDesktopWinMain::CsDesktopWinMain(QWidget *parent) :
  CsVisualWinMain{parent}
  {
  //==========================================
  //      Top layer of layout
  //1. At left side - play list
  mWLeftPart = new QStackedWidget();

  //2. At central part - wiziwig editors with piano
  QWidget *central = new QWidget();
  //Central part divided into top and bottom parts
  //2.1 At top part - stack of wiziwig editors
  mWCentralPart = new QStackedWidget();

  //2.2 At bottom part - stack of instruments
  mWInstrum     = new QStackedWidget();

  //Fill 2
  QVBoxLayout *vbox = new QVBoxLayout();
  vbox->setSpacing(0);
  vbox->addWidget( mWCentralPart );
  vbox->addWidget( mWInstrum );
  central->setLayout( vbox );

  //Fill top
  mWSplitter    = new QSplitter();
  mWSplitter->addWidget( mWLeftPart );
  mWSplitter->addWidget( central );

  setCentralWidget( mWSplitter );


  //==========================================
  //      Second layer of layout
  // 1. Left part
  // 1.1. Play list
  mWLeftPlayList = new CsVisualPlayList();
  mWLeftPart->addWidget( mWLeftPlayList );

  // 1.2. Part list
  mWLeftPartList = new CsVisualPartList();
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
    if( canCloseEditor() ) {
      mComposition.fileLoad( compositionId );
      compositionChanged();
      }
    });


  // 2.1 At top part of Central - stack of wiziwig editors
  // 2.1.1 Score view and train
  mWCentralScoreTrain = new CsVisualScoreTrain( mComposition, this );
  mWCentralPart->addWidget( mWCentralScoreTrain );

  // 2.1.2. Score edit
  mWCentralScoreEdit = new CsVisualScoreEdit( mComposition, this );
  mWCentralPart->addWidget( mWCentralScoreEdit );

  // 2.1.3 Score karaoke
  mWCentralScoreKaraoke = new CsVisualScoreKaraoke( mComposition, this );
  mWCentralPart->addWidget( mWCentralScoreKaraoke );

  //2.2 At bottom part of Central - stack of instruments
  mWInstrumPiano = new CsVisualPiano();
  mWInstrum->addWidget( mWInstrumPiano );
  mWInstrum->setMaximumHeight( 100 );
  connect( mWInstrumPiano, &CsVisualPiano::midiNote, this, &CsDesktopWinMain::midiNote );
  connect( this, &CsDesktopWinMain::playHighlight, mWInstrumPiano, &CsVisualPiano::playHighlight );

  //Real midi keyboard
  CsMidiKeyboard *kbd = nullptr;
#ifdef Q_OS_LINUX
  kbd = new CsMidiKeyboardLinux();
#endif
  QAction *actionMidiKeyboard = new QAction( QIcon(QString(":/pic/helpHome.png")), tr("MIDI Keyboard status") );
  actionMidiKeyboard->setDisabled(true);
  if( kbd != nullptr ) {
    //From keyboard
    connect( kbd, &CsMidiKeyboard::midiNote, this, &CsDesktopWinMain::midiNote );
    connect( kbd, &CsMidiKeyboard::midiNote, mWInstrumPiano, &CsVisualPiano::playNote );
    connect( kbd, &CsMidiKeyboard::midiRun, this, &CsDesktopWinMain::midiRun );
    connect( kbd, &CsMidiKeyboard::midiVoice, this, &CsDesktopWinMain::midiVoice );
    connect( kbd, &CsMidiKeyboard::midiLink, actionMidiKeyboard, &QAction::setEnabled );
    //To keyboard
    connect( this, &CsDesktopWinMain::playRun, kbd, &CsMidiKeyboard::playRun );
    connect( this, &CsDesktopWinMain::playVoice, kbd, &CsMidiKeyboard::playVoice );

    kbd->init();
    }

  //Restore splitter positions
  QSettings s;
  if( s.contains(QString(KEY_MAIN_SPLITTER)) )
    mWSplitter->restoreState( s.value(QString(KEY_MAIN_SPLITTER)).toByteArray() );

  if( s.contains(QStringLiteral(KEY_WMAIN_MAX)) ) {
    if( !s.value(QStringLiteral(KEY_WMAIN_MAX)).toBool() )
      //Restore main window size
      resize( s.value(QStringLiteral(KEY_WMAIN_SIZE)).toSize() );
    }


  // Desktop menu and toolbar
  menuFile = new QMenu( tr("File") );

  actionFileNew      = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileNew.png")), tr("New file"), this, &CsDesktopWinMain::cmFileNew );
  actionFileImport   = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileOpen.png")), tr("Open file ..."), this, &CsDesktopWinMain::cmFileImport );
  actionFileLoad     = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileCloud.png")), tr("Load from public cloud..."), this, &CsDesktopWinMain::cmFileLoad );
  menuFile->addSeparator();
  actionFileSave     = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileSave.png")), tr("Save score"), this, &CsDesktopWinMain::cmFileSave );
  actionFileCopy     = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileSaveAs.png")), tr("Make copy score"), this, &CsDesktopWinMain::cmFileCopy );
  actionFilePublic   = menuFile->addAction( QIcon(QStringLiteral(":/pic/filePublic.png")), tr("Public to cloud"), this, &CsDesktopWinMain::cmFilePublic );
  actionFileExport   = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileExport.png")), tr("Export..."), this,  &CsDesktopWinMain::cmFileExport );
  menuFile->addSeparator();
  actionFilePrint    = menuFile->addAction( QIcon(QStringLiteral(":/pic/filePrint.png")), tr("Print..."), this, &CsDesktopWinMain::cmFilePrint );
  menuFile->addSeparator();
  actionFileExit     = menuFile->addAction( QIcon(QStringLiteral(":/pic/fileExit.png")), tr("Exit program"), this, &CsDesktopWinMain::close );

  menuEditDisabled = new QMenu( tr("Edit") );
  actionEditPasteImport = menuEditDisabled->addAction( QIcon(QStringLiteral(":/pic/viewEditor.png")), tr("Import from clipboard"), this, &CsDesktopWinMain::cmEditPasteImport );

  menuEdit = new QMenu( tr("Edit") );
  menuEdit->addAction( QIcon(QStringLiteral(":/pic/viewEditor.png")), tr("Import from clipboard"), this, &CsDesktopWinMain::cmEditPasteImport );
  menuEdit->addSeparator();
  actionEditUndo = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editUndo.png")), tr("Undo last edit operation"), mWCentralScoreEdit, &CsVisualScoreEdit::cmEditUndo );
  actionEditRedo = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editRedo.png")), tr("Redo last undoed operation"), mWCentralScoreEdit, &CsVisualScoreEdit::cmEditRedo );
  menuEdit->addSeparator();
  actionEditCopy = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editCopy.png")), tr("Copy selection to clipboard"), mWCentralScoreEdit, &CsVisualScoreEdit::cmEditCopy );
  actionEditCut  = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editCut.png")), tr("Cut selection to clipboard"), mWCentralScoreEdit, &CsVisualScoreEdit::cmEditCut );
  actionEditPaste = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editPaste.png")), tr("Paste from clipboard"), mWCentralScoreEdit, &CsVisualScoreEdit::cmEditPaste );
  actionEditDelete = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editDelete.png")), tr("Delete slection"), mWCentralScoreEdit, &CsVisualScoreEdit::cmEditDelete );
  menuEdit->addSeparator();
  actionEditSettings = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editSettings.png")), tr("Edit score settings"), this, &CsDesktopWinMain::cmEditSettings );

  menuView = new QMenu( tr("View") );
  actionViewEditor  = menuView->addAction( QIcon(QStringLiteral(":/pic/viewEditor.png")), tr("Editor mode"), this, &CsDesktopWinMain::cmViewEditor );
  actionViewTrain   = menuView->addAction( QIcon(QStringLiteral(":/pic/viewTrain.png")), tr("Train mode"), this, &CsDesktopWinMain::cmViewTrain );
  actionViewKaraoke = menuView->addAction( QIcon(QStringLiteral(":/pic/viewKaraoke.png")), tr("Karaoke mode"), this, &CsDesktopWinMain::cmViewKaraoke );
  actionViewEditor->setCheckable(true);
  actionViewTrain->setCheckable(true);
  actionViewKaraoke->setCheckable(true);
  menuView->addSeparator();
  actionViewRemark      = menuView->addAction( QIcon(QStringLiteral(":/pic/viewRemark.png")), tr("Remark manage..."), this, &CsDesktopWinMain::cmViewRemark );
  actionViewChord       = menuView->addAction( QIcon(QStringLiteral(":/pic/viewChord.png")), tr("Chord manage..."), this, &CsDesktopWinMain::cmViewChord );
  actionViewNote        = menuView->addAction( QIcon(QStringLiteral(":/pic/viewNote.png")), tr("Note manage..."), this, &CsDesktopWinMain::cmViewNote );
  actionViewTranslation = menuView->addAction( QIcon(QStringLiteral(":/pic/viewTranslation.png")), tr("Translation manage..."), this, &CsDesktopWinMain::cmViewTranslation );

  QActionGroup *group = new QActionGroup(menuView);
  group->addAction( actionViewEditor );
  group->addAction( actionViewTrain );
  group->addAction( actionViewKaraoke );

  menuPlay = new QMenu( tr("Play") );
  actionPlayStart = menuPlay->addAction( QIcon(QStringLiteral(":/pic/playStart.png")), tr("Start"), this, &CsDesktopWinMain::cmPlayRun );
  actionPlayTrain = menuPlay->addAction( QIcon(QStringLiteral(":/pic/playStart.png")), tr("Start train"), this, &CsDesktopWinMain::cmPlayTrain );
  actionPlayPause = menuPlay->addAction( QIcon(QStringLiteral(":/pic/playPause.png")), tr("Pause"), this, &CsDesktopWinMain::cmPlayPause );
  actionPlayStop  = menuPlay->addAction( QIcon(QStringLiteral(":/pic/playStop.png")), tr("Stop"), this, &CsDesktopWinMain::cmPlayStop );
  //actionPlayStart->setCheckable(true);
  actionPlayPause->setCheckable(true);
  actionPlayPause->setEnabled(false);
  actionPlayStop->setEnabled(false);
  connect( this, &CsDesktopWinMain::playRun, actionPlayStart, &QAction::setDisabled );
  connect( this, &CsDesktopWinMain::playRun, actionPlayTrain, &QAction::setDisabled );
  connect( this, &CsDesktopWinMain::playRun, actionPlayPause, &QAction::setEnabled );
  connect( this, &CsDesktopWinMain::playRun, actionPlayStop, &QAction::setEnabled );
  connect( this, &CsDesktopWinMain::playPause, actionPlayPause, &QAction::setChecked );

  menuTrain = new QMenu( tr("Train") );
  actionFragmentTrain = menuTrain->addAction( QIcon(QStringLiteral(":/pic/playTrain.png")), tr("Fragment train") /*, mWinTrain, &CsWinTrain::cmFragmentTrain */ );
  actionFragment0     = menuTrain->addAction( QIcon(QStringLiteral(":/pic/fragment0.png")), tr("All composition") /*, mWinTrain, &CsWinTrain::cmFragment0 */ );
  actionFragment1     = menuTrain->addAction( QIcon(QStringLiteral(":/pic/fragment1.png")), tr("Fragment 1") /*, mWinTrain, &CsWinTrain::cmFragment1 */ );
  actionFragment2     = menuTrain->addAction( QIcon(QStringLiteral(":/pic/fragment2.png")), tr("Fragment 2") /*, mWinTrain, &CsWinTrain::cmFragment2 */ );
  actionFragmentStart = menuTrain->addAction( QIcon(QStringLiteral(":/pic/fragmentStart.png")), tr("Fix fragment start") /*, mWinTrain, &CsWinTrain::cmFragmentStart */ );
  actionFragmentStop  = menuTrain->addAction( QIcon(QStringLiteral(":/pic/fragmentStop.png")), tr("Fix fragment stop") /*, mWinTrain, &CsWinTrain::cmFragmentStop */ );
  actionFragmentTrain->setCheckable(true);
  actionFragment0->setCheckable(true);
  actionFragment1->setCheckable(true);
  actionFragment2->setCheckable(true);

  menuScore = new QMenu( tr("Score") );

  menuTools = new QMenu( tr("Tools") );

  menuHelp = new QMenu( tr("Help") );
  actionHelpContents     = menuHelp->addAction( QIcon(QString(":/pic/helpContent.png")), tr("Contents"), this, &CsDesktopWinMain::cmHelpContent );
  actionHelpAbout        = menuHelp->addAction( QIcon(QString(":/pic/helpAbout.png")), tr("About"), this, &CsDesktopWinMain::cmHelpAbout );
  actionHelpWeb          = menuHelp->addAction( QIcon(QString(":/pic/helpWebsite.png")), tr("WEB home page"), this, &CsDesktopWinMain::cmHelpWeb );
  actionHelpRegistration = menuHelp->addAction( QIcon(QString(":/pic/helpRegistration.png")), tr("Registration"), this, &CsDesktopWinMain::cmHelpRegistration );
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

  barMain = new QToolBar( tr("Common") );
  barMain->addAction( actionFileNew );
  barMain->addAction( actionFileCopy );
  barMain->addAction( actionFileSave );
  barMain->addSeparator();
  barMain->addAction( actionViewEditor );
  barMain->addAction( actionViewTrain );
  barMain->addAction( actionViewKaraoke );
  barMain->addSeparator();
  barMain->addAction( actionMidiKeyboard );
  addToolBar( barMain );

  barEditor   = new QToolBar( tr("Editor") );
  barEditor->addAction( actionEditPaste );
  barEditor->addAction( actionEditCopy );
  barEditor->addAction( actionEditCut );
  barEditor->addSeparator();
  barEditor->addAction( actionEditUndo );
  barEditor->addAction( actionEditRedo );
  barEditor->addSeparator();
  barEditor->addAction( actionPlayStart );
  barEditor->addAction( actionPlayTrain );
  barEditor->addAction( actionPlayPause );
  barEditor->addAction( actionPlayStop );
  addToolBar( barEditor );

  barTrain    = new QToolBar( tr("Train") );
  barTrain->addAction( actionPlayStart );
  barTrain->addAction( actionPlayTrain );
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
  barKaraoke->addAction( actionPlayStart );
  barKaraoke->addAction( actionPlayPause );
  barKaraoke->addAction( actionPlayStop );
  addToolBar( barKaraoke );

  barPlayList = new QToolBar( tr("PlayList") );

  cmViewEditor();

  //Create imports
  mImportManager.registerImport( new CsImportSaliScore() );
  mImportManager.registerImport( new CsImportText() );

  }



void CsDesktopWinMain::playUpdate()
  {
  visualCurrentUpdate();
  }



void CsDesktopWinMain::closeEvent(QCloseEvent *ev)
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




void CsDesktopWinMain::cmFileNew()
  {
  if( canCloseEditor() ) {
    mComposition.clear();
    mComposition.remarkAppend( tr("Default"), tr("Default remark") );
    mComposition.chordAppend( tr("Main"), tr("Main chord part") );
    mComposition.noteAppend( tr("Solo"), tr("Solo part") );
    mComposition.lineAppend( true );
    mComposition.remarkSet( 0, tr("Default"), tr("Create own composition here") );
    mComposition.lineAppend( false );

    compositionChanged();
    }
  }




void CsDesktopWinMain::cmFileImport()
  {
  if( canCloseEditor() ) {
    QString fname = QFileDialog::getOpenFileName( this, tr("Select file to open"), QString{}, mImportManager.formats() );
    bool ok = false;
    CsComposition composition = mImportManager.readFile( fname, ok );
    if( ok ) {
      mComposition = composition;
      compositionChanged();
      }
    }
  }




void CsDesktopWinMain::cmFileLoad()
  {

  }



void CsDesktopWinMain::cmFileSave()
  {
  mComposition.fileSave();
  CsPlayList::pl()->compositionSet( mComposition );
  if( mWLeftPart->currentWidget() == mWLeftPartList ) {
    CsPlayList::pl()->partCompositionAppend( mWLeftPartList->partIndex(), mComposition.songId() );
    mWLeftPartList->update();
    }
  else
    CsPlayList::pl()->partCompositionAppend( 0, mComposition.songId() );
  visualCurrentUpdate();
  }




void CsDesktopWinMain::cmFileCopy()
  {
  mComposition.fileCopySave();
  CsPlayList::pl()->compositionSet( mComposition );
  visualCurrentUpdate();
  }

void CsDesktopWinMain::cmFilePublic()
  {

  }

void CsDesktopWinMain::cmFileExport()
  {

  }

void CsDesktopWinMain::cmFilePrint()
  {

  }

void CsDesktopWinMain::cmEditPasteImport()
  {
  if( canCloseEditor() ) {
    QClipboard *clip = QGuiApplication::clipboard();
    if( !clip->text().isEmpty() ) {
      bool ok = false;
      CsComposition composition = mImportManager.read( clip->text().toUtf8(), ok );
      if( ok ) {
        mComposition = composition;
        compositionChanged();
        }
      }
    }
  }




void CsDesktopWinMain::cmEditSettings()
  {
  //TODO score settings dialog
//  CsDlgScoreSettings dlg( mComposition, this );
//  if( dlg.exec() )
  //    mWinEditor->update();
  }




void CsDesktopWinMain::cmViewEditor()
  {
  visualActiveSet( mWCentralScoreEdit, actionViewEditor, barEditor, menuEdit, nullptr );
  }

void CsDesktopWinMain::cmViewTrain()
  {
  visualActiveSet( mWCentralScoreTrain, actionViewTrain, barTrain, menuEditDisabled, menuTrain );
  }

void CsDesktopWinMain::cmViewKaraoke()
  {
  visualActiveSet( mWCentralScoreKaraoke, actionViewKaraoke, barKaraoke, nullptr, nullptr );
  }

void CsDesktopWinMain::cmViewRemark()
  {

  }

void CsDesktopWinMain::cmViewChord()
  {

  }

void CsDesktopWinMain::cmViewNote()
  {

  }

void CsDesktopWinMain::cmViewTranslation()
  {

  }


void CsDesktopWinMain::cmHelpContent()
  {

  }

void CsDesktopWinMain::cmHelpAbout()
  {

  }

void CsDesktopWinMain::cmHelpWeb()
  {

  }

void CsDesktopWinMain::cmHelpRegistration()
  {
  //Prepare and display registration dialog
  CsDlgRegistration dlg(this);
  //If after registration user yet not registered then exit application
  if( dlg.exec() == 0 && !repoClient->isRegistered() )
    close();
  }



void CsDesktopWinMain::compositionChanged()
  {
  mWCentralScoreEdit->compositionChanged();
  mWCentralScoreKaraoke->compositionChanged();
  mWCentralScoreTrain->compositionChanged();
  }



//!
//! \brief canCloseEditor Check, can be closed editor
//! \return               true - editor can be closed
//!
bool CsDesktopWinMain::canCloseEditor()
  {
  cmPlayStop();
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




CsVisualScore *CsDesktopWinMain::visualActive()
  {
  CsVisualScore *vs = dynamic_cast<CsVisualScore*>( mWCentralPart->currentWidget() );
  Q_ASSERT_X( vs != nullptr, "invalid active score", "CsDesktopWinMain::visualActive" );
  return vs;
  }



void CsDesktopWinMain::visualCurrentUpdate()
  {
  visualActive()->update();
  }



//!
//! \brief visualActivate Change visual mode
//! \param visualScore    New activated visual score mode (edit, train or karaoke)
//! \param visualAction   Visal action (tool button and menu) which respond visual score mode
//! \param activeBar      Tool bar respond visual score mode
//! \param activeMenu0    First menu respond visual score mode
//! \param activeMenu1    Second menu respond visual score mode
//!
void CsDesktopWinMain::visualActiveSet(CsVisualScore *visualScore, QAction *visualAction, QToolBar *activeBar, QMenu *activeMenu0, QMenu *activeMenu1)
  {
  //Visual score widget
  mWCentralPart->setCurrentWidget( visualScore );
  //Tool button
  actionViewEditor->setChecked( actionViewEditor == visualAction );
  actionViewTrain->setChecked( actionViewTrain == visualAction );
  actionViewKaraoke->setChecked( actionViewKaraoke == visualAction );
  //Menu bar
  if( barEditor == activeBar ) barEditor->show();
  else barEditor->hide();
  if( barTrain == activeBar ) barTrain->show();
  else barTrain->hide();
  if( barKaraoke == activeBar ) barKaraoke->show();
  else barKaraoke->hide();
  //Menu
  actionMenuEditDisabled->setVisible( menuEditDisabled == activeMenu0 || menuEditDisabled == activeMenu1 );
  actionMenuEdit->setVisible( menuEdit == activeMenu0 || menuEdit == activeMenu1 );
  actionMenuTrain->setVisible( menuTrain == activeMenu0 || menuTrain == activeMenu1 );
  visualActive()->activate();
  }







//=============================================================================================================================
//                                     Commands
QMenu *CsDesktopWinMain::menuFile;
QMenu *CsDesktopWinMain::menuEditDisabled;
QMenu *CsDesktopWinMain::menuEdit;
QMenu *CsDesktopWinMain::menuView;
QMenu *CsDesktopWinMain::menuPlay;
QMenu *CsDesktopWinMain::menuTrain;
QMenu *CsDesktopWinMain::menuScore;
QMenu *CsDesktopWinMain::menuTools;
QMenu *CsDesktopWinMain::menuHelp;

QActionPtr  CsDesktopWinMain::actionMenuEditDisabled;
QActionPtr  CsDesktopWinMain::actionMenuEdit;
QActionPtr  CsDesktopWinMain::actionMenuTrain;

//Tool bars for editor command
QToolBar   *CsDesktopWinMain::barMain;
QToolBar   *CsDesktopWinMain::barEditor;
QToolBar   *CsDesktopWinMain::barTrain;
QToolBar   *CsDesktopWinMain::barKaraoke;
QToolBar   *CsDesktopWinMain::barPlayList;


QActionPtr  CsDesktopWinMain::actionFileNew;
QActionPtr  CsDesktopWinMain::actionFileImport;
QActionPtr  CsDesktopWinMain::actionFileCopy;
QActionPtr  CsDesktopWinMain::actionFileLoad;
QActionPtr  CsDesktopWinMain::actionFileSave;
QActionPtr  CsDesktopWinMain::actionFilePublic;
QActionPtr  CsDesktopWinMain::actionFileExport;
QActionPtr  CsDesktopWinMain::actionFilePrint;
QActionPtr  CsDesktopWinMain::actionFileExit;


QActionPtr  CsDesktopWinMain::actionEditUndo;
QActionPtr  CsDesktopWinMain::actionEditRedo;
QActionPtr  CsDesktopWinMain::actionEditCut;
QActionPtr  CsDesktopWinMain::actionEditCopy;
QActionPtr  CsDesktopWinMain::actionEditPaste;
QActionPtr  CsDesktopWinMain::actionEditPasteImport;
QActionPtr  CsDesktopWinMain::actionEditDelete;
QActionPtr  CsDesktopWinMain::actionEditSelectAll;
QActionPtr  CsDesktopWinMain::actionEditUnSelect;
QActionPtr  CsDesktopWinMain::actionEditSettings;

QActionPtr  CsDesktopWinMain::actionViewEditor;
QActionPtr  CsDesktopWinMain::actionViewTrain;
QActionPtr  CsDesktopWinMain::actionViewKaraoke;
QActionPtr  CsDesktopWinMain::actionViewRemark;
QActionPtr  CsDesktopWinMain::actionViewChord;
QActionPtr  CsDesktopWinMain::actionViewNote;
QActionPtr  CsDesktopWinMain::actionViewTranslation;

QActionPtr  CsDesktopWinMain::actionPlayStart;
QActionPtr  CsDesktopWinMain::actionPlayTrain;
QActionPtr  CsDesktopWinMain::actionPlayPause;
QActionPtr  CsDesktopWinMain::actionPlayStop;

QActionPtr  CsDesktopWinMain::actionFragmentTrain;
QActionPtr  CsDesktopWinMain::actionFragment0;
QActionPtr  CsDesktopWinMain::actionFragment1;
QActionPtr  CsDesktopWinMain::actionFragment2;
QActionPtr  CsDesktopWinMain::actionFragmentStart;
QActionPtr  CsDesktopWinMain::actionFragmentStop;

QActionPtr  CsDesktopWinMain::actionScoreRemark;
QActionPtr  CsDesktopWinMain::actionScoreRemarkManage;
QActionPtr  CsDesktopWinMain::actionScoreChordManage;
QActionPtr  CsDesktopWinMain::actionScoreNoteManage;


QActionPtr  CsDesktopWinMain::actionPlMinus;
QActionPtr  CsDesktopWinMain::actionPlPlus;
QActionPtr  CsDesktopWinMain::actionPlLoad;
QActionPtr  CsDesktopWinMain::actionPlPlayList;

QActionPtr  CsDesktopWinMain::actionToolsOption;

QActionPtr  CsDesktopWinMain::actionHelpContents;
QActionPtr  CsDesktopWinMain::actionHelpAbout;
QActionPtr  CsDesktopWinMain::actionHelpWeb;
QActionPtr  CsDesktopWinMain::actionHelpRegistration;
QActionPtr  CsDesktopWinMain::actionHelpHome;
QActionPtr  CsDesktopWinMain::actionHelpBackward;
QActionPtr  CsDesktopWinMain::actionHelpForward;

QActionPtr  CsDesktopWinMain::actionGuiderCapture;
QActionPtr  CsDesktopWinMain::actionGuiderPause;
