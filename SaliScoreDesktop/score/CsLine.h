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
    auto    lyricGet() const { return mLyricList; }

    void    lyricSet( const CsLyricList &lyricList ) { mLyricList = lyricList; }

    //========================================================
    //    Translation part
    QString translationGet( const QString &lang ) const { return mTranslation.value( lang ); }

    void    translationSet( const QString &lang, const QString &tran ) { mTranslation.insert( lang, tran ); }

    void    translationRename( const QString &prevLang, const QString &newLang );

    void    translationRemove( const QString &lang ) { mTranslation.remove( lang ); }

    //========================================================
    //    json part
    void jsonWrite( CsJsonWriter &js ) const;

    void jsonRead( CsJsonReader &js );
  };

using CsLinePtr = CsLine*;

using CsLinePtrList = QList<CsLinePtr>;

using CsLineList = QList<CsLine>;

#endif // CSLINE_H
