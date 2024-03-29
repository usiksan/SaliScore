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

#include "CsConfig.h"
#include "CsRemark.h"
#include "CsChordKit.h"
#include "CsNoteKit.h"
#include "CsLyricSymbol.h"

#include <QList>

class CsLine
  {
    CsRemark              mRemark;      //!< Any remark text
    CsChordKit            mChordKit;    //!< Chord for different parts (variants)
    CsNoteKit             mNoteKit;     //!< Notes for different parts (variants)
    CsLyricLine           mLyricLine;   //!< Lyric line
    QMap<QString,QString> mTranslation; //!< Translations of lyric on other languages
    int                   mTickPerTakt; //!< Tick per takt
    int                   mTaktCount;   //!< Takt count per line
  public:
    CsLine();

    CsLine( const QString &lang, const QString &rem );

    //========================================================
    //    Information
    auto &remarkConst() const { return mRemark; }
    auto &chordKitConst() const { return mChordKit; }
    auto &noteKitConst() const { return mNoteKit; }
    auto &lyricLineConst() const { return mLyricLine; }
    auto &translationConst() const { return mTranslation; }

    bool  isRemark() const { return !mRemark.isEmpty(); }

    //========================================================
    //    Remark part
    QString remarkGet( const QString &lang ) const { return mRemark.remarkGet(lang); }

    void    remarkSet( const QString &lang, const QString &rem ) { mRemark.remarkSet(lang,rem); }

    void    remarkRename( const QString &prevLang, const QString &newLang ) { mRemark.remarkRename( prevLang, newLang ); }

    void    remarkRemove( const QString &lang ) { mRemark.remarkRemove( lang ); }

    //========================================================
    //    Chord part
    auto    chordListGet( const QString &part ) const { return mChordKit.chordListGet(part); }

    void    chordListSet( const QString &part, const CsChordList &line ) { mChordKit.chordListSet( part, line ); }

    void    chordRename( const QString &prevPart, const QString &newPart ) { mChordKit.chordRename( prevPart, newPart ); }

    void    chordRemove( const QString &part ) { mChordKit.chordRemove( part ); }


    //========================================================
    //    Chord part
    auto    noteListGet( const QString &part ) const { return mNoteKit.noteListGet( part ); }

    void    noteListSet( const QString &part, const CsNoteList &list ) { mNoteKit.noteListSet( part, list ); }

    void    noteRename( const QString &prevPart, const QString &newPart ) { mNoteKit.noteRename( prevPart, newPart ); }

    void    noteRemove( const QString &part ) { mNoteKit.noteRemove( part ); }


    //========================================================
    //    Lyric part
    auto    lyricGet() const { return mLyricLine; }

    void    lyricSet( const CsLyricLine &lyricLine ) { mLyricLine = lyricLine; }


    //========================================================
    //    Translation part
    QString translationGet( const QString &lang ) const { return mTranslation.value( lang ); }

    void    translationSet( const QString &lang, const QString &tran ) { mTranslation.insert( lang, tran ); }

    void    translationRename( const QString &prevLang, const QString &newLang );

    void    translationRemove( const QString &lang ) { mTranslation.remove( lang ); }

    //========================================================
    //   Tick and Takt

    int     tickPerTakt() const { return mTickPerTakt; }

    void    tickPerTaktSet( int tpt ) { mTickPerTakt = tpt; }

    int     taktCount() const { return mTaktCount; }

    void    taktCountSet( int tc ) { mTaktCount = tc; }

    //========================================================
    //    json part
    void    jsonWrite( CsJsonWriter &js ) const;

    void    jsonRead( CsJsonReader &js );

  private:
    QString lyricToString() const;

    void    stringToLyric( const QString line );
  };

using CsLinePtr = CsLine*;

using CsLinePtrList = QList<CsLinePtr>;

using CsLineList = QList<CsLine>;

#endif // CSLINE_H
