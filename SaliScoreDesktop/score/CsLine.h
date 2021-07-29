#ifndef CSLINE_H
#define CSLINE_H

#include "CsConfig.h"
#include "CsRemark.h"
#include "CsChordKit.h"
#include "CsNoteKit.h"
#include "CsLyric.h"

#include <QList>

class CsLine
  {
    CsRemark              mRemark;
    CsChordKit            mChordKit;
    CsNoteKit             mNoteKit;
    CsLyricList           mLyricList;
    QMap<QString,QString> mTranslation;
  public:
    CsLine();

    QJsonObject write() const;

    void        read( const QJsonObject obj );
  };

using CsLinePtr = CsLine*;

using CsLinePtrList = QList<CsLinePtr>;

using CsLineList = QList<CsLine>;

#endif // CSLINE_H
