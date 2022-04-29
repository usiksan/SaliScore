#ifndef CSDESKTOPWINMAIN_H
#define CSDESKTOPWINMAIN_H


#include "CsVisualWinMain.h"
#include "score/CsComposition.h"

#include "import/CsImportManager.h"
#include "synth/CsSynthSfManager.h"

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
class CsVisualScore;
class CsVisualScoreTrain;
class CsVisualScoreEdit;
class CsVisualScoreKaraoke;
class CsVisualPiano;

using QActionPtr = QAction*;

class CsDesktopWinMain : public CsVisualWinMain
  {
    Q_OBJECT

    QSplitter            *mWSplitter;            //!< Central widget of application, it delimit space into two parts: wiziwig editors and help
    QStackedWidget       *mWLeftPart;            //!< Left part of main window. Contains play list
    CsVisualPlayList     *mWLeftPlayList;        //!< Widget with play list
    CsVisualPartList     *mWLeftPartList;        //!< Widget with part list

    QStackedWidget       *mWCentralPart;         //!< Central part with editor, trainer and karaoke
    CsVisualScoreTrain   *mWCentralScoreTrain;   //!< Visual score view and train
    CsVisualScoreEdit    *mWCentralScoreEdit;    //!< Visual score edit
    CsVisualScoreKaraoke *mWCentralScoreKaraoke; //!< Visual score karaoke player

    QStackedWidget       *mWInstrum;
    CsVisualPiano        *mWInstrumPiano;

    CsImportManager       mImportManager;        //!< Import files manager. Contains converters from other formats
    CsSynthSfManager     *mSynthSfManager;

  public:
    explicit CsDesktopWinMain( QWidget *parent = nullptr);

    virtual void playUpdate() override;

    virtual void closeEvent( QCloseEvent *ev ) override;

  signals:

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
    void cmEditSettings();

    void cmViewEditor();
    void cmViewTrain();
    void cmViewKaraoke();
    void cmViewRemark();
    void cmViewChord();
    void cmViewNote();
    void cmViewTranslation();

    void cmHelpContent();
    void cmHelpAbout();
    void cmHelpWeb();
    void cmHelpRegistration();



  private:
    void compositionChanged();

    //!
    //! \brief canCloseEditor Check, can be closed editor
    //! \return               true - editor can be closed
    //!
    bool canCloseEditor();

    CsVisualScore *visualActive();

    void           visualCurrentUpdate();

    //!
    //! \brief visualActiveSet Change visual mode
    //! \param visualScore     New activated visual score mode (edit, train or karaoke)
    //! \param visualAction    Visal action (tool button and menu) which respond visual score mode
    //! \param activeBar       Tool bar respond visual score mode
    //! \param activeMenu0     First menu respond visual score mode
    //! \param activeMenu1     Second menu respond visual score mode
    //!
    void           visualActiveSet( CsVisualScore *visualScore, QAction *visualAction, QToolBar *activeBar, QMenu *activeMenu0, QMenu *activeMenu1 );

    //======================================================================================
    //                           Commands
    static QMenu *menuFile;
    static QMenu *menuEditDisabled;
    static QMenu *menuEdit;
    static QMenu *menuView;
    static QMenu *menuPlay;
    static QMenu *menuTrain;
    static QMenu *menuScore;
    static QMenu *menuTools;
    static QMenu *menuHelp;

    static QActionPtr actionMenuEditDisabled;
    static QActionPtr actionMenuEdit;
    static QActionPtr actionMenuTrain;

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
    static QActionPtr  actionEditSettings;

    static QActionPtr  actionViewEditor;
    static QActionPtr  actionViewTrain;
    static QActionPtr  actionViewKaraoke;
    static QActionPtr  actionViewRemark;
    static QActionPtr  actionViewChord;
    static QActionPtr  actionViewNote;
    static QActionPtr  actionViewTranslation;

    static QActionPtr  actionPlayStart;
    static QActionPtr  actionPlayTrain;
    static QActionPtr  actionPlayPause;
    static QActionPtr  actionPlayStop;

    static QActionPtr  actionFragmentTrain;
    static QActionPtr  actionFragment0;
    static QActionPtr  actionFragment1;
    static QActionPtr  actionFragment2;
    static QActionPtr  actionFragmentStart;
    static QActionPtr  actionFragmentStop;

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

#endif // CSDESKTOPWINMAIN_H
