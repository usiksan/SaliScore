#include "CsWinScore.h"
#include "CsWinMain.h"
#include "CsDlgDefRemark.h"
#include "CsDlgDefChord.h"
#include "CsDlgDefNote.h"
#include "CsDlgDefTranslation.h"

#include <QFileInfo>
#include <QJsonDocument>
#include <QToolBar>

CsWinScore::CsWinScore(const QString filePath, CsComposition &src, QWidget *parent) :
  CsWinPage( filePath, parent ),
  mComposition(src)
  {
  setPath(filePath);

  if( filePath != path() )
    mComposition.dirtySet();

  mComposition.remarkAppend( QStringLiteral("ru"), QStringLiteral("Russian") );
  mComposition.chordAppend( QStringLiteral("soft"), QStringLiteral("Soft variant") );
  mComposition.noteAppend( QStringLiteral("piano"), QStringLiteral("Piano soft variant") );
  mComposition.translationAppend( QStringLiteral("en"), QStringLiteral("English") );

  int ln = mComposition.lineAppend( true );
  mComposition.remarkSet( ln, QStringLiteral("ru"),  QStringLiteral("Проигрыш") );

  ln = mComposition.lineAppend( false );
  CsChordList chordList;
  chordList.append( CsChord(128,noteC,chordMin) );
  chordList.append( CsChord(256,noteD,chordMin) );
  mComposition.chordListSet( ln, QStringLiteral("soft"), chordList );

  CsNoteList noteList;
  noteList.append( CsNote( 128, duraEighth, noteG, octaveSecond ) );
  noteList.append( CsNote( 128 + duraEighth, duraEighth, noteC, octaveFirst ) );
  noteList.append( CsNote( 128 + 2*duraEighth, duraEighth, noteB, octaveFirst ) );
  noteList.append( CsNote( 128 + 3*duraEighth, duraEighth, noteA, octaveFirst ) );
  noteList.append( CsNote( 128 + 5*duraEighth, duraEighth + duraSixteenth + duraThirtySecond, noteCd, octaveSecond ) );
  noteList.append( CsNote( 128 + 8*duraEighth, duraEighth + duraSixteenth + duraThirtySecond, noteFd, octaveFirst ) );
  mComposition.noteListSet( ln, QStringLiteral("piano"), noteList );

  CsLyricList lyricList;
  lyricList.append( CsLyric(20, QStringLiteral("Бывают дни") ) );
  lyricList.append( CsLyric(256, QStringLiteral("когда опустишь") ) );
  lyricList.append( CsLyric(400, QStringLiteral("руки") ) );
  mComposition.lyricSet( ln, lyricList );

  mComposition.translationSet( ln, QStringLiteral("en"), QStringLiteral("There days when put down hands ") );

  ln = mComposition.lineAppend( true );
  mComposition.remarkSet( ln, QStringLiteral("ru"),  QStringLiteral("Проигрыш") );


  addWidget( mWinTrain = new CsWinTrain( mComposition ) );
  addWidget( mWinKaraoke = new CsWinKaraoke( mComposition ) );
  addWidget( mWinEditor = new CsWinEditor( mComposition ) );

  if( filePath.startsWith(CS_DEFAULT_FILE_NAME) )
    cmViewEditor();
  else if( CsWinMain::actionViewKaraoke->isChecked() )
    cmViewKaraoke();
  else if( CsWinMain::actionViewTrain->isChecked() )
    cmViewTrain();
  else
    cmViewEditor();
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
  }

void CsWinScore::cmViewKaraoke()
  {
  setCurrentWidget( mWinKaraoke );
  //Hide toolbars
  CsWinMain::barPlayList->hide();
  CsWinMain::barKaraoke->hide();
  CsWinMain::barEditor->hide();
  //Show active bar
  CsWinMain::barTrain->show();
  CsWinMain::actionViewKaraoke->setChecked(true);
  }





void CsWinScore::cmViewRemark()
  {
  CsDlgDefRemark dlgRem( mComposition, this );
  dlgRem.fill();
  dlgRem.exec();
  }




void CsWinScore::cmViewChord()
  {
  CsDlgDefChord dlgChord( mComposition, this );
  dlgChord.fill();
  dlgChord.exec();
  }



void CsWinScore::cmViewNote()
  {
  CsDlgDefNote dlgNote( mComposition, this );
  dlgNote.fill();
  dlgNote.exec();
  }



void CsWinScore::cmViewTranslation()
  {
  CsDlgDefTranslation dlgTranslation( mComposition, this );
  dlgTranslation.fill();
  dlgTranslation.exec();
  }

void CsWinScore::cmPlayStart()
  {

  }

void CsWinScore::cmPlayPause()
  {

  }

void CsWinScore::cmPlayStop()
  {

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

