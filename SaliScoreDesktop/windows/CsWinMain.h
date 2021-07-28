#ifndef CSWINMAIN_H
#define CSWINMAIN_H

#include "CsConfig.h"
#include "CsWinHelp.h"
#include "CsWinScore.h"
#include "import/CsImportManager.h"

#include <QMainWindow>
#include <QSplitter>
#include <QTabWidget>
#include <QStringList>
#include <QClipboard>
#include <QAction>

using QActionPtr = QAction*;

class CsWinMain : public QMainWindow
  {
    Q_OBJECT

    QSplitter       *mWSplitter;     //!< Central widget of application, it delimit space into two parts: wiziwig editors and help
    QTabWidget      *mWEditors;      //!< Editors
    CsWinHelp       *mWHelp;         //!< Edge help

    CsImportManager  mImportManager; //!< Import files manager. Contains converters from other formats
  public:
    CsWinMain(QWidget *parent = nullptr);
    ~CsWinMain();

  private slots:

    //Menu File
    void cmFileNew();
    void cmFileOpen();
    void cmFileLoad();
    void cmFileImport();
    void cmFileSave();
    void cmFileSaveAs();
    void cmFileSaveAll();
    void cmFileClose();
    void cmFileCloseAll();
    void cmFilePrevious();

  public:
    virtual void closeEvent( QCloseEvent *ev ) override;

  private:
    //!
    //! \brief activeEditor Returns current actived editor
    //! \return             Current actived editor or nullptr if none
    //!
    CsWinScore *activeEditor() const;

    //!
    //! \brief editor Retrive editor by tab index
    //! \param index  Index of tab which editor need to be retrieved
    //! \return       Editor by tab index or nullptr if no editor in this index
    //!
    CsWinScore *editor( int index ) const;

    //!
    //! \brief fileSaveIndex Save file which editor on index tab
    //! \param index         Tab index for editor
    //!
    void         fileSaveIndex( int index );

    //!
    //! \brief fileSaveAsIndex Save file which editor on index tab
    //! \param index           Tab index for editor
    //!
    void         fileSaveAsIndex( int index );

    //!
    //! \brief fileCloseIndex Closes file editor with index tab
    //! \param index          Tab index editor which need to be closed
    //!
    void         fileCloseIndex( int index );

    //!
    //! \brief fileOpen Open file with path
    //! \param path     Path of file
    //!
    void         fileOpen( const QString path );

    //!
    //! \brief appendEditor Appends editor to editors tab
    //! \param editor       Appended editor
    //!
    void         appendEditor( CsWinScore *editor );

    void createMenu();
  public:
    //======================================================================================
    //                           Commands
    static QMenu *menuFile;
    static QMenu *menuFilePrevious;
    static QMenu *menuEdit;
    static QMenu *menuView;
    static QMenu *menuPlay;
    static QMenu *menuScore;
    static QMenu *menuTools;
    static QMenu *menuHelp;

    static QActionPtr actionMenuEdit;

    //Tool bars for editor command
    static QToolBar *barMain;
    static QToolBar *barEditor;
    static QToolBar *barTrain;
    static QToolBar *barKaraoke;


    static QActionPtr  actionFileNew;
    static QActionPtr  actionFileOpen;
    static QActionPtr  actionFileLoad;
    static QActionPtr  actionFileImport;
    static QActionPtr  actionFileSave;
    static QActionPtr  actionFileSaveAs;
    static QActionPtr  actionFileSaveAll;
    static QActionPtr  actionFilePublic;
    static QActionPtr  actionFileExport;
    static QActionPtr  actionFilePrint;
    static QActionPtr  actionFileClose;
    static QActionPtr  actionFileCloseAll;
    static QActionPtr  actionFileExit;

    static QActionPtr  actionFilePrevious[CS_PREVIOUS_FILES_COUNT];

    static QActionPtr  actionEditUndo;
    static QActionPtr  actionEditRedo;
    static QActionPtr  actionEditCut;
    static QActionPtr  actionEditCopy;
    static QActionPtr  actionEditPaste;
    static QActionPtr  actionEditDelete;
    static QActionPtr  actionEditSelectAll;
    static QActionPtr  actionEditUnSelect;

    static QActionPtr  actionViewEditor;
    static QActionPtr  actionViewTrain;
    static QActionPtr  actionViewKaraoke;

    static QActionPtr  actionPlayStart;
    static QActionPtr  actionPlayPause;
    static QActionPtr  actionPlayStop;
    static QActionPtr  actionPlayTrain;

    static QActionPtr  actionScoreRemark;
    static QActionPtr  actionScoreRemarkManage;
    static QActionPtr  actionScoreChordManage;
    static QActionPtr  actionScoreNoteManage;

    static QActionPtr  actionToolsOption;

    static QActionPtr  actionHelpContents;
    static QActionPtr  actionHelpIndex;
    static QActionPtr  actionHelpAbout;
    static QActionPtr  actionHelpRegistration;
    static QActionPtr  actionHelpHome;
    static QActionPtr  actionHelpBackward;
    static QActionPtr  actionHelpForward;

    static QActionPtr  actionGuiderCapture;
    static QActionPtr  actionGuiderPause;
  };
#endif // CSWINMAIN_H
