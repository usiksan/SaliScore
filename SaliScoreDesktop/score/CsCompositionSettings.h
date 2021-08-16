#ifndef CSCOMPOSITIONSETTINGS_H
#define CSCOMPOSITIONSETTINGS_H

#include "CsCompositionHeader.h"
#include "CsComposition.h"

#include <QStringList>

class CsCompositionSettings : public CsCompositionHeader
  {
    QStringList mRemarkList;
    QStringList mChordList;
    QStringList mNoteList;
    QStringList mTranslationList;
  public:
    CsCompositionSettings();
    CsCompositionSettings( const CsComposition &comp );

    QStringList remarkList() const { return mRemarkList; }

    QStringList chordList() const { return mChordList; }

    QStringList noteList() const { return mNoteList; }

    QStringList translationList() const { return mTranslationList; }
  };

#endif // CSCOMPOSITIONSETTINGS_H
