#include "CsWinScore.h"
#include "CsWinMain.h"

#include <QFileInfo>
#include <QJsonDocument>
#include <QToolBar>

CsWinScore::CsWinScore(const QString filePath, CsComposition &src, QWidget *parent) :
  CsWinPage( filePath, parent ),
  mComposition(src),
  mPlayer(mComposition),
  mDefferedReset(true)
  {
  setPath(filePath);

  if( filePath != path() )
    mComposition.dirtySet();

  addWidget( mWinTrain = new CsWinScoreMode( new CsWinTrain( mComposition, mPlayer ) ) );
  addWidget( mWinKaraoke = new CsWinScoreMode( new CsWinKaraoke( mComposition, mPlayer ) ) );
  addWidget( mWinEditor = new CsWinScoreMode( new CsWinEditor( mComposition, mPlayer ) ) );

  if( filePath.startsWith(CS_DEFAULT_FILE_NAME) )
    cmViewEditor();
  else if( CsWinMain::actionViewKaraoke->isChecked() )
    cmViewKaraoke();
  else if( CsWinMain::actionViewTrain->isChecked() )
    cmViewTrain();
  else
    cmViewEditor();

  mUpdateTimer.setInterval(100);
  }










void CsWinScore::cmEditUndo()
  {

  }

void CsWinScore::cmEditRedo()
  {

  }

void CsWinScore::cmEditCut()
  {

  }

void CsWinScore::cmEditCopy()
  {

  }

void CsWinScore::cmEditPaste()
  {

  }

void CsWinScore::cmEditDelete()
  {

  }

void CsWinScore::cmViewEditor()
  {
  setCurrentWidget( mWinEditor );
  //Hide toolbars
  CsWinMain::barPlayList->hide();
  CsWinMain::barTrain->hide();
  CsWinMain::barKaraoke->hide();
  //Show active bar
  CsWinMain::barEditor->show();
  CsWinMain::actionViewEditor->setChecked(true);
  //Activate karaoke
  mWinEditor->view()->activate();
  }

void CsWinScore::cmViewTrain()
  {
  setCurrentWidget( mWinTrain );
  //Hide toolbars
  CsWinMain::barPlayList->hide();
  CsWinMain::barKaraoke->hide();
  CsWinMain::barEditor->hide();
  //Show active bar
  CsWinMain::barTrain->show();
  CsWinMain::actionViewTrain->setChecked(true);
  //Activate karaoke
  mWinTrain->view()->activate();
  }

void CsWinScore::cmViewKaraoke()
  {
  setCurrentWidget( mWinKaraoke );
  //Hide toolbars
  CsWinMain::barPlayList->hide();
  CsWinMain::barTrain->hide();
  CsWinMain::barEditor->hide();
  //Show active bar
  CsWinMain::barKaraoke->show();
  CsWinMain::actionViewKaraoke->setChecked(true);
  //Activate karaoke
  mWinKaraoke->view()->activate();
  }






void CsWinScore::playStart()
  {
  if( mDefferedReset ) {
    mDefferedReset = false;
    mPlayer.reset();
    mPlayer.show(true);
    }

  if( CsWinMain::actionViewKaraoke->isChecked() )
    connect( &mUpdateTimer, &QTimer::timeout, mWinKaraoke->view(), &CsWinScoreView::viewUpdate );
  else if( CsWinMain::actionViewTrain->isChecked() )
    connect( &mUpdateTimer, &QTimer::timeout, mWinTrain->view(), &CsWinScoreView::viewUpdate );
  else
    connect( &mUpdateTimer, &QTimer::timeout, mWinEditor->view(), &CsWinScoreView::viewUpdate );
  mUpdateTimer.start();
  }




void CsWinScore::playStop()
  {
  if( mDefferedReset )
    mPlayer.show(false);
  mDefferedReset = true;
  mUpdateTimer.stop();
  mUpdateTimer.disconnect();
  }




void CsWinScore::playTick(int tick)
  {
  mPlayer.next( tick );
  }






//!
//! \brief activate Called when window activated to ajust view
//!
void CsWinScore::activate()
  {
  if( currentWidget() == mWinEditor )
    cmViewEditor();
  else if( currentWidget() == mWinTrain )
    cmViewTrain();
  else
    cmViewKaraoke();
  }



QString CsWinScore::extension() const
  {
  return QStringLiteral(CS_BASE_EXTENSION);
  }


//!
//! \brief cmFileSave Saves file into path
//! \param path       Path to file
//!
void CsWinScore::cmFileSave(const QString path)
  {
  QFile file(path);
  if( file.open(QIODevice::WriteOnly) ) {
    //Create writer and use it to write composition contents to json object
    CsJsonWriter js{};
    mComposition.jsonWrite( js );

    //Append file type and version
    js.jsonString( CS_BASE_TYPE_KEY, QStringLiteral(CS_BASE_TYPE) );
    js.jsonInt( CS_BASE_VERSION_KEY, CS_BASE_VERSION );

    //Write contents to file
    file.write( QJsonDocument(js.object()).toJson() );

    //Write completed, reset dirty
    mComposition.dirtyReset();
    }
  }


void CsWinScore::cmFilePublic()
  {

  }

void CsWinScore::cmFileExport()
  {

  }

void CsWinScore::cmFilePrint()
  {

  }

