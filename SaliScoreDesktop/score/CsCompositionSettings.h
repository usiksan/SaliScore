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

    //=================================================================
    //         Settings JSON io

    virtual void jsonWrite( CsJsonWriter &js ) const override;

    virtual void jsonRead( CsJsonReader &js ) override;
  };

#endif // CSCOMPOSITIONSETTINGS_H
