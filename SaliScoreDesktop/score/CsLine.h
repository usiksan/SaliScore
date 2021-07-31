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

    CsLine( const QString &lang, const QString &rem );

    //========================================================
    //    Information
    auto &remarkConst() const { return mRemark; }
    auto &chordKitConst() const { return mChordKit; }
    auto &noteKitConst() const { return mNoteKit; }
    auto &lyricListConst() const { return mLyricList; }
    auto &translationConst() const { return mTranslation; }

    bool  isRemark() const { return !mRemark.isEmpty(); }

    //========================================================
    //    Remark part
    QString remarkGet( const QString &lang ) const { return mRemark.remarkGet(lang); }

    void    remarkSet( const QString &lang, const QString &rem ) { mRemark.remarkSet(lang,rem); }

    //========================================================
    //    Chord part
    auto    chordListGet( const QString &part ) const { return mChordKit.chordListGet(part); }

    void    chordListSet( const QString &part, const CsChordList &line ) { mChordKit.chordListSet( part, line ); }


    //========================================================
    //    Chord part
    auto    noteListGet( const QString &part ) const { return mNoteKit.noteListGet( part ); }

    void    noteListSet( const QString &part, const CsNoteList &list ) { mNoteKit.noteListSet( part, list ); }

    //========================================================
    //    Lyric part
    auto    lyricGet() const { return mLyricList; }

    void    lyricSet( const CsLyricList &lyricList ) { mLyricList = lyricList; }

    //========================================================
    //    Translation part
    QString translationGet( const QString &lang ) const { return mTranslation.value( lang ); }

    void    translationSet( const QString &lang, const QString &tran ) { mTranslation.insert( lang, tran ); }

    //========================================================
    //    json part
    void jsonWrite( CsJsonWriter &js ) const;

    void jsonRead( CsJsonReader &js );
  };

using CsLinePtr = CsLine*;

using CsLinePtrList = QList<CsLinePtr>;

using CsLineList = QList<CsLine>;

#endif // CSLINE_H
