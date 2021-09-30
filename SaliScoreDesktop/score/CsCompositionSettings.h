/*
Project "SaliScore Score music edit, view and tutorial programm"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Composition settings is composition header with visual represent flags and other local (for user) information
*/
#ifndef CSCOMPOSITIONSETTINGS_H
#define CSCOMPOSITIONSETTINGS_H

#include "CsCompositionHeader.h"
#include "CsComposition.h"

#include <QStringList>

class CsCompositionSettings : public CsCompositionHeader
  {
    //Visible list is list of part which are visible on screen. Others parts are hide
    QStringList mRemarkList;      //!< Visible remark list
    QStringList mChordList;       //!< Visible chord list
    QStringList mNoteList;        //!< Visible note list
    QStringList mTranslationList; //!< Visible translation list
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
