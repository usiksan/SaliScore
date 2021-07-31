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

    //========================================================
    //    Remark part
    QString remarkGet( const QString &lang ) const { return mRemark.remarkGet(lang); }

    void    remarkSet( const QString &lang, const QString &rem ) { mRemark.remarkSet(lang,rem); }

    //========================================================
    //    Chord part
    auto  chordListGet( const QString &part ) const { return mChordKit.chordListGet(part); }

    void  chordListSet( const QString &part, const CsChordList &line ) { mChordKit.chordListSet( part, line ); }


    //========================================================
    //    Lyric part
    void setLyric( const CsLyricList &lyricList ) { mLyricList = lyricList; }

    //========================================================
    //    json part
    void jsonWrite( CsJsonWriter &js ) const;

    void jsonRead( CsJsonReader &js );
  };

using CsLinePtr = CsLine*;

using CsLinePtrList = QList<CsLinePtr>;

using CsLineList = QList<CsLine>;

#endif // CSLINE_H
