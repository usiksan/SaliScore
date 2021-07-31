/*
Project "SaliScore Score music edit, view and tutorial programm"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Settings are individual tunings to composition
*/
#ifndef CSSETTINGS_H
#define CSSETTINGS_H

#include "CsConfig.h"
#include "CsJsonIO.h"

#include <QStringList>

class CsComposition;

class CsSettings
  {
    int         mViewMode;     //!< Mode of composition view
    QStringList mRemarkList;
    QStringList mChordList;
    QStringList mNoteList;
    QStringList mTranslateList;
  public:
    CsSettings();

    QStringList remarkList() const { return mRemarkList; }
    void        remarkToggle( const QString &remarkId, bool on ) { stringListToggle( mRemarkList, remarkId, on ); }

    QStringList chordList() const { return mChordList; }
    void        chordToggle( const QString &chordId, bool on ) { stringListToggle( mChordList, chordId, on ); }

    QStringList noteList() const { return mNoteList; }
    void        noteToggle( const QString &noteId, bool on ) { stringListToggle( mNoteList, noteId, on ); }

    QStringList translateList() const { return mTranslateList; }
    void        translateToggle( const QString &translateId, bool on ) { stringListToggle( mTranslateList, translateId, on ); }

    void        jsonWrite( CsJsonWriter &js ) const;
    void        jsonRead( CsJsonReader &js );

    void        sync( const CsComposition &comp );
  private:
    static void stringListToggle( QStringList &list, const QString &id, bool on );
  };

#endif // CSSETTINGS_H
