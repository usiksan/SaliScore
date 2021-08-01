#include "CsWinPage.h"

#include <QFileInfo>

CsWinPage::CsWinPage(const QString path, QWidget *parent) :
  QStackedWidget(parent)
  {
  }




//!
//! \brief setPath Setup new path for composition
//! \param thePath New path for composition
//!
void CsWinPage::setPath(const QString thePath)
  {
  QFileInfo info(thePath);
  mPath = info.absolutePath() + QChar('/') + info.completeBaseName() + extension();
  }



//!
//! \brief name Returns current file name (without path)
//! \return     Current file name
//!
QString CsWinPage::name() const
  {
  QFileInfo info(mPath);
  return info.completeBaseName();
  }
