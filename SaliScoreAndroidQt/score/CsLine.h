/*
Project "SaliScore Score music edit, view and tutorial programm"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  CsLine - is one line of partiture
  Line may be a remark or song

  One line of song contains chord line, note line, lyric line and translation line
  Each of them lines may contains multiple line for different variant of representation, but all
  of them apply to single song line

  Line contains exactly count of takts
*/
#ifndef CSLINE_H
#define CSLINE_H

#include "config.h"
#include "CsChord.h"
#include "CsNote.h"
#include "CsLyricSymbol.h"
#include "CsKit.h"

#include <QList>


using CsChordKit  = CsKitOfList<CsChord>;
using CsNoteKit   = CsKitOfList<CsNote>;

class CsLine
  {
    CsKitOfString         mRemark;      //!< Any remark text
    CsChordKit            mChordKit;    //!< Chord for different parts (variants)
    CsNoteKit             mNoteKit;     //!< Notes for different parts (variants)
    CsLyricLine           mLyricLine;   //!< Lyric line
    CsKitOfString         mTranslate;   //!< Lyric translations
    int                   mTaktCount;   //!< Takt count per line
  public:
    CsLine();

    CsLine( const QString &lang, const QString &rem );

    //========================================================
    //    Information
    bool    isRemark() const { return !mRemark.isEmpty(); }

    //========================================================
    //    Remark part
    QString remarkGet( const QString &lang ) const { return mRemark.get(lang); }

    void    remarkSet( const QString &lang, const QString &rem ) { mRemark.set(lang,rem); }

    void    remarkRename( const QString &prevLang, const QString &newLang ) { mRemark.rename( prevLang, newLang ); }

    void    remarkRemove( const QString &lang ) { mRemark.remove( lang ); }


    //========================================================
    //    Chord part
    auto    chordListGet( const QString &part ) const { return mChordKit.get(part); }

    void    chordListSet( const QString &part, const CsChordList &line ) { mChordKit.set( part, line ); }

    void    chordRename( const QString &prevPart, const QString &newPart ) { mChordKit.rename( prevPart, newPart ); }

    void    chordRemove( const QString &part ) { mChordKit.remove( part ); }


    //========================================================
    //    Note part
    auto    noteListGet( const QString &part ) const { return mNoteKit.get( part ); }

    void    noteListSet( const QString &part, const CsNoteList &list ) { mNoteKit.set( part, list ); }

    void    noteRename( const QString &prevPart, const QString &newPart ) { mNoteKit.rename( prevPart, newPart ); }

    void    noteRemove( const QString &part ) { mNoteKit.remove( part ); }


    //========================================================
    //    Lyric part
    auto    lyricGet() const { return mLyricLine; }

    void    lyricSet( const CsLyricLine &lyricLine ) { mLyricLine = lyricLine; }


    //========================================================
    //    Translate part
    QString translationGet( const QString &lang ) const { return mTranslate.get(lang); }

    void    translationSet( const QString &lang, const QString &rem ) { mTranslate.set(lang,rem); }

    void    translationRename( const QString &prevLang, const QString &newLang ) { mTranslate.rename( prevLang, newLang ); }

    void    translationRemove( const QString &lang ) { mTranslate.remove( lang ); }


    //========================================================
    //   Tick and Takt
    int     taktCount() const { return mTaktCount; }

    void    taktCountSet( int tc ) { mTaktCount = tc; }

    //========================================================
    //    json part
    void    jsonWrite( CsJsonWriter &js ) const;

    void    jsonRead( CsJsonReader &js );

  };

using CsLinePtr = CsLine*;

using CsLinePtrList = QList<CsLinePtr>;

using CsLineList = QList<CsLine>;

#endif // CSLINE_H
