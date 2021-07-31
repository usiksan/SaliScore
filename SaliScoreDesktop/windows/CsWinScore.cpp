#include "CsWinScore.h"

#include <QFileInfo>
#include <QJsonDocument>
#include <QVBoxLayout>

CsWinScore::CsWinScore(const QString path, CsComposition &src, QWidget *parent) :
  QWidget(parent),
  mPath(path),
  mComposition(src),
  mDirty(false)
  {
  setPath(path);

  CsLine line;
  CsChordList chordList;
  chordList.append( CsChord(128,noteC,chordMin) );
  chordList.append( CsChord(256,noteD,chordMin) );
  line.chordListSet( QStringLiteral("soft"), chordList );

  mState.remarkAppend( QStringLiteral("ru"), QStringLiteral("Russian") );
  mState.chordAppend( QStringLiteral("soft"), QStringLiteral("Soft variant") );

  mComposition.lineAppend( CsLine( QStringLiteral("ru"), QStringLiteral("Проигрыш")) );
  mComposition.lineAppend( CsLine( QStringLiteral("ru"), QString{} ) );
  mComposition.lineAppend( CsLine( QStringLiteral("ru"), QStringLiteral("Куплет")) );
  mComposition.lineAppend( line );
  mComposition.lineAppend( CsLine( QStringLiteral("ru"), QString{} ) );
  mComposition.lineAppend( CsLine( QStringLiteral("ru"), QStringLiteral("Припев")) );

  //Layout
  QVBoxLayout *box = new QVBoxLayout();
  box->setSpacing(0);
  box->setContentsMargins( 0, 0, 0, 0 );

  box->addWidget( mWinTrain = new CsWinTrain( mComposition, mState ) );

  setLayout( box );

  //
  }


//!
//! \brief setPath Setup new path for composition
//! \param thePath New path for composition
//!
void CsWinScore::setPath(const QString thePath)
  {
  QFileInfo info(thePath);
  mPath = info.absolutePath() + QChar('/') + info.completeBaseName() + QStringLiteral(CS_BASE_EXTENSION);
  }



//!
//! \brief name Returns current file name (without path)
//! \return     Current file name
//!
QString CsWinScore::name() const
  {
  QFileInfo info(mPath);
  return info.completeBaseName();
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
