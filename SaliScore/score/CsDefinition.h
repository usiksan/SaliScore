#ifndef CSDEFINITION_H
#define CSDEFINITION_H

#include "CsConfig.h"
#include "CsNoteChord.h"
#include "CsClass.h"
#include "SvLib/SvJsonIO.h"

#include <QString>
#include <QList>

struct CsDefinition
  {
  private:
    int     mClass;
  public:
    QString mName;
    QString mInstrument;
    QString mDescription;
    int     mClef;
    bool    mVisible;
    bool    mSynthes;      //!< Do synthes of this instrument
    bool    mTrain;        //!< Perform teaching of this instrument part

    CsDefinition( int csClass = cccRemark, const QString nm = QString{}, const QString descr = QString{}, const QString inst = QString{}, int mClef = noteC );

    int  classGet() const { return mClass; }

    bool visibleToggle() { mVisible = !mVisible; return mVisible; }

    int  channel() const { return 0; }

    void json( SvJsonWriter &js ) const;

    void json( SvJsonReader &js );

  };

using CsDefList = QList<CsDefinition>;

#endif // CSDEFINITION_H
