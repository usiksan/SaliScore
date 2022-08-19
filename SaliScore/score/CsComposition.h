#ifndef CSCOMPOSITION_H
#define CSCOMPOSITION_H

#include "CsConfig.h"
#include "CsLine.h"
#include "CsDefinition.h"

class CsComposition
  {
    QMap<QString,QString> mAttributes;

    int                   mLineStartOffset; //!< Offset of line start in tick
    int                   mTickPerPart;     //!< Part duration in tick
    int                   mPartPerTakt;     //!< Part count per takt

    CsDefList             mDefList;         //!< List of parts (tracks).

    CsLineList            mLineList;        //!< Line list of song

    CsTrainList         mTrainList;              //!< List of train interval for training

    bool                mDirty;
  public:
    CsComposition();
  };

#endif // CSCOMPOSITION_H
