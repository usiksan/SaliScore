#ifndef CSWINMAIN_H
#define CSWINMAIN_H

#include "CsConfig.h"
#include "CsWinHelp.h"

#include <QMainWindow>
#include <QSplitter>
#include <QTabWidget>
#include <QStringList>
#include <QClipboard>

class CsWinMain : public QMainWindow
  {
    Q_OBJECT

    QSplitter       *mWSplitter;     //!< Central widget of application, it delimit space into two parts: wiziwig editors and help
    QTabWidget      *mWEditors;      //!< Editors
    CsWinHelp       *mWHelp;         //!< Edge help


    static QMenu *menuFile;
    static QMenu *menuFilePrevious;
    static QMenu *menuObject;
    static QMenu *menuEdit;
    static QMenu *menuView;
    static QMenu *menuDraw;
    static QMenu *menuInsertSymbol;
    static QMenu *menuInsertSheet;
    static QMenu *menuInsertPart;
    static QMenu *menuInsertPart3d;
    static QMenu *menuInsertPcb;
    static QMenu *menuInsertComp;
    static QMenu *menuInstruments;
    static QMenu *menuRules;
    static QMenu *menuHelp;

    static QMenu *menuSelect;

    static QActionPtr cmMenuInsertSymbol;
    static QActionPtr cmMenuInsertSheet;
    static QActionPtr cmMenuInsertPart;
    static QActionPtr cmMenuInsertPart3d;
    static QActionPtr cmMenuInsertPcb;
    static QActionPtr cmMenuInsertComp;
    static QActionPtr cmMenuRules;

    //Tool bars for editor command
    static QToolBar *barMain;
    static QToolBar *barSymbol;
    static QToolBar *barPart;
    static QToolBar *barPart3d;
    static QToolBar *barComp;
    static QToolBar *barSheet;
    static QToolBar *barPcb;
    static QToolBar *barView;
    static QToolBar *barHelp;

    static void addEditCommands( QToolBar *bar );
    static void addViewCommands( QToolBar *bar );
    static void addDrawCommands( QToolBar *bar );



    static QActionPtr  cmFileNew;
    static QActionPtr  cmFileOpen;
    static QActionPtr  cmFileClose;
    static QActionPtr  cmFileCloseAll;
    static QActionPtr  cmFileSave;
    static QActionPtr  cmFileSaveAs;
    static QActionPtr  cmFileSaveAll;
    static QActionPtr  cmFilePrint;
    static QActionPtr  cmFileExit;
    static QActionPtr  cmFileImportPis;
    static QActionPtr  cmFileImport;
    static QActionPtr  cmFileExport;
    static QActionPtr  cmFileExportGerber;
    static QActionPtr  cmFileStoreToLibrary;
    static QActionPtr  cmFileLoadFromLibrary;

    static QActionPtr  cmFilePrevious[PREVIOUS_FILES_COUNT];

    static QActionPtr  cmObjectNew;
    static QActionPtr  cmObjectLoad;
    static QActionPtr  cmObjectRename;
    static QActionPtr  cmObjectDelete;
    static QActionPtr  cmObjectCopy;
    static QActionPtr  cmObjectPaste;
    static QActionPtr  cmObjectCut;
    static QActionPtr  cmObjectDuplicate;
    static QActionPtr  cmObjectSort;
    static QActionPtr  cmObjectParam;
    static QActionPtr  cmObjectEditEnable;
    static QActionPtr  cmObjectEditDisable;
    static QActionPtr  cmProjectParam;
    static QActionPtr  cmProjectUpgrade;

    static QActionPtr  cmEditUndo;
    static QActionPtr  cmEditRedo;
    static QActionPtr  cmEditCut;
    static QActionPtr  cmEditCopy;
    static QActionPtr  cmEditPaste;
    static QActionPtr  cmEditDelete;
    static QActionPtr  cmEditSelectAll;
    static QActionPtr  cmEditUnSelect;
    static QActionPtr  cmEditFind;
    static QActionPtr  cmEditReplace;
    static QActionPtr  cmEditRotateGroup;
    static QActionPtr  cmEditProperties;

    static QActionPtr  cmViewProject;
    static QActionPtr  cmView3d;
    static QActionPtr  cmViewFill;
    static QActionPtr  cmViewNets;
    static QActionPtr  cmViewGrid;
    static QActionPtr  cmViewLayers;

    static QActionPtr  cm3dMaster;
    static QActionPtr  cm3dImportStl;
    static QActionPtr  cm3dImportStep;
    static QActionPtr  cm3dImportVrml;
    static QActionPtr  cm3dShow2d;
    static QActionPtr  cm3dShowPads;

    static QActionPtr  cmNetSetup;
    static QActionPtr  cmModeLink;
    static QActionPtr  cmModeNetName;
    static QActionPtr  cmModeNetList;
    static QActionPtr  cmModePack;
    static QActionPtr  cmPads;

    static QActionPtr  cmShowRatNet;
    static QActionPtr  cmShowRuleErrors;
    static QActionPtr  cmRenumeration;
    static QActionPtr  cmShowField;
    static QActionPtr  cmShowPads;

    static QActionPtr  cmRulesEdit;
    static QActionPtr  cmRulesCheck;
    static QActionPtr  cmRulesErrorNext;

    static QActionPtr  cmSheetExpression;

    static QActionPtr  cmOption;
    static QActionPtr  cmTools;

    static QActionPtr  cmHelpContents;
    static QActionPtr  cmHelpIndex;
    static QActionPtr  cmHelpAbout;
    static QActionPtr  cmHelpRegistration;
    static QActionPtr  cmHelpHome;
    static QActionPtr  cmHelpBackward;
    static QActionPtr  cmHelpForward;

    static QActionPtr  cmGuiderCapture;
    static QActionPtr  cmGuiderPause;

    //Full mode action table
    static QActionPtr  cmModeTable[MD_LAST];

    //Full list mode tool bars
    static QToolBarPtr mbarTable[PB_LAST];

  public:
    CsWinMain(QWidget *parent = nullptr);
    ~CsWinMain();
  };
#endif // CSWINMAIN_H
