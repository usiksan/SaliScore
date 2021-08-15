#ifndef CSPLAYCOMPOSITION_H
#define CSPLAYCOMPOSITION_H

#include "SvJson/SvJsonIO.h"
#include "CsDescrSong.h"
#include "CsSynthSettings.h"

#include <QString>
#include <QList>

class CsPlayComposition : public CsDescrSong
  {
    CsSynthSettings mSettings;
  public:
    CsPlayComposition();

    const CsSynthSettings &settings() const { return mSettings; }

    void jsonWrite( SvJsonWriter &writer ) const;

    void jsonRead( SvJsonReaderExtInt &reader );
  };


using CsPlayCompositionList = QList<CsPlayComposition>;

#endif // CSPLAYCOMPOSITION_H
