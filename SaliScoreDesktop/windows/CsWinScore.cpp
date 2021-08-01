#include "CsWinScore.h"

#include <QFileInfo>
#include <QJsonDocument>

CsWinScore::CsWinScore(const QString path, CsComposition &src, QWidget *parent) :
  CsWinPage( path, parent ),
  mComposition(src)
  {

  /*
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
  */

  addWidget( mWinTrain = new CsWinTrain( mComposition ) );
  addWidget( mWinKaraoke = new CsWinKaraoke() );
  addWidget( mWinEditor = new CsWinEditor() );

  //Layout
//  QVBoxLayout *box = new QVBoxLayout();
//  box->setSpacing(0);
//  box->setContentsMargins( 0, 0, 0, 0 );


//  setLayout( box );

  //
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

