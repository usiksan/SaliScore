#ifndef CSWINMAIN_H
#define CSWINMAIN_H

#include "CsConfig.h"

#include "score/CsComposition.h"
#include "score/CsPlayList.h"
#include "play/CsPlay.h"

#include "import/CsImportManager.h"

#include "CsWinHelp.h"
#include "CsWinIntro.h"
#include "CsWinPage.h"
#include "CsWinScoreMode.h"
#include "CsWinTrain.h"
#include "CsWinEditor.h"
#include "CsWinKaraoke.h"
#include "CsWinPlayList.h"
#include "CsWinRemote.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <QSplitter>
#include <QTabWidget>
#include <QStringList>
#include <QClipboard>
#include <QAction>
#include <QTimer>
#include <QLineEdit>

using QActionPtr = QAction*;

class CsMidiSequencer;

#define WIN_INDEX_PLAY_LIST   0
#define WIN_INDEX_REMOTE_FIND 1

class CsWinMain : public QMainWindow
  {
    Q_OBJECT

    QSplitter       *mWSplitter;     //!< Central widget of application, it delimit space into two parts: wiziwig editors and help
    QStackedWidget  *mWLeftPart;     //!< Play list
    QStackedWidget  *mWCentralPart;  //!< Central part with editor, trainer and karaoke
    CsWinHelp       *mWHelp;         //!< Edge help

    CsWinPlayList   *mWPlayList;
    CsWinRemote     *mWRemote;

    CsWinIntro      *mWinIntro;
    CsWinScoreMode  *mWinEditor;
    CsWinScoreMode  *mWinTrain;
    CsWinScoreMode  *mWinKaraoke;

    CsMidiSequencer *mMidiSequencer;

    CsImportManager  mImportManager; //!< Import files manager. Contains converters from other formats

    CsComposition    mComposition;
    CsPlay           mPlayer;
    CsPlayList      &mPlayList;
    bool             mDefferedReset;
    bool             mNotSaved;
    QTimer           mUpdateTimer;
  public:
    CsWinMain( CsPlayList &playList, CsMidiSequencer *midiSequencer, QWidget *parent = nullptr);
    ~CsWinMain();

  private slots:

    //Menu File
    void cmFileNew();
    void cmFileImport();
    void cmFileLoad();
    void cmFileSave();
    void cmFileCopy();
    void cmFilePublic();
    void cmFileExport();
    void cmFilePrint();

    void cmEditPasteImport();

    void cmViewEditor();
    void cmViewTrain();
    void cmViewKaraoke();
    void cmViewRemark();
    void cmViewChord();
    void cmViewNote();
    void cmViewTranslation();

    void cmPlayStart();
    void cmPlayStop();

    void cmHelpContent();
    void cmHelpAbout();
    void cmHelpWeb();
    void cmHelpRegistration();

  public:
    virtual void closeEvent( QCloseEvent *ev ) override;

  private:
    void         activateComposition( int partIndex, int compositionIndex );

    //!
    //! \brief buildPlayList Builds play list view widget
    //! \return              Play list view widget
    //!
    QWidget     *buildPlayList();

    //!
    //! \brief buildRemoteFind Builds remote find view list
    //! \return                Remote find view list
    //!
    QWidget     *buildRemoteFind();

    //!
    //! \brief canCloseEditor Check, can be closed editor
    //! \return               true - editor can be closed
    //!
    bool         canCloseEditor();

    void         createMenu();
  public:
    //======================================================================================
    //                           Commands
    static QMenu *menuFile;
    static QMenu *menuEditDisabled;
    static QMenu *menuEdit;
    static QMenu *menuView;
    static QMenu *menuPlay;
    static QMenu *menuScore;
    static QMenu *menuTools;
    static QMenu *menuHelp;

    static QActionPtr actionMenuEditDisabled;
    static QActionPtr actionMenuEdit;

    //Tool bars for editor command
    static QToolBar *barMain;
    static QToolBar *barEditor;
    static QToolBar *barTrain;
    static QToolBar *barKaraoke;
    static QToolBar *barPlayList;

    static QActionPtr  actionFileNew;
    static QActionPtr  actionFileImport;
    static QActionPtr  actionFileLoad;
    static QActionPtr  actionFileSave;
    static QActionPtr  actionFileCopy;
    static QActionPtr  actionFilePublic;
    static QActionPtr  actionFileExport;
    static QActionPtr  actionFilePrint;
    static QActionPtr  actionFileExit;

    static QActionPtr  actionEditUndo;
    static QActionPtr  actionEditRedo;
    static QActionPtr  actionEditCut;
    static QActionPtr  actionEditCopy;
    static QActionPtr  actionEditPaste;
    static QActionPtr  actionEditPasteImport;
    static QActionPtr  actionEditDelete;
    static QActionPtr  actionEditSelectAll;
    static QActionPtr  actionEditUnSelect;

    static QActionPtr  actionViewEditor;
    static QActionPtr  actionViewTrain;
    static QActionPtr  actionViewKaraoke;
    static QActionPtr  actionViewRemark;
    static QActionPtr  actionViewChord;
    static QActionPtr  actionViewNote;
    static QActionPtr  actionViewTranslation;

    static QActionPtr  actionPlayStart;
    static QActionPtr  actionPlayPause;
    static QActionPtr  actionPlayStop;
    static QActionPtr  actionPlayTrain;

    static QActionPtr  actionScoreRemark;
    static QActionPtr  actionScoreRemarkManage;
    static QActionPtr  actionScoreChordManage;
    static QActionPtr  actionScoreNoteManage;

    static QActionPtr  actionPlMinus;
    static QActionPtr  actionPlPlus;
    static QActionPtr  actionPlLoad;
    static QActionPtr  actionPlPlayList;

    static QActionPtr  actionToolsOption;

    static QActionPtr  actionHelpContents;
    static QActionPtr  actionHelpAbout;
    static QActionPtr  actionHelpWeb;
    static QActionPtr  actionHelpRegistration;
    static QActionPtr  actionHelpHome;
    static QActionPtr  actionHelpBackward;
    static QActionPtr  actionHelpForward;

    static QActionPtr  actionGuiderCapture;
    static QActionPtr  actionGuiderPause;
  };
#endif // CSWINMAIN_H
