#include "CsWinScore.h"

#include <QFileInfo>

CsWinScore::CsWinScore(const QString path, CsComposition &src, QWidget *parent) :
  QWidget(parent),
  mPath(path),
  mComposition(src),
  mDirty(false)
  {

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
