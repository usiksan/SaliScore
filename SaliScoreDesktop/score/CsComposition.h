#ifndef CSCOMPOSITION_H
#define CSCOMPOSITION_H

#include "CsConfig.h"
#include "CsLine.h"

class CsComposition
  {
    QString    mTitle;
    QString    mSinger;
    QString    mComposer;
    QString    mLyricist;
    CsLineList mLineList;
  public:
    CsComposition();
  };

#endif // CSCOMPOSITION_H
