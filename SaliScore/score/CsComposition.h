#ifndef CSCOMPOSITION_H
#define CSCOMPOSITION_H

#include "CsConfig.h"
#include "CsLine.h"
#include "CsDefinition.h"
#include "CsTrainInterval.h"

//Standard attributes names
#define CS_ATTR_AUTHOR     QStringLiteral("Author")    //!< Author who creates composition score
#define CS_ATTR_SINGER     QStringLiteral("Singer")    //!< Singer is common singer of composition
#define CS_ATTR_COMPOSER   QStringLiteral("Composer")  //!< Composer of composition
#define CS_ATTR_LYRICIST   QStringLiteral("Lyricist")  //!< Lyricist of composition
#define CS_ATTR_NAME       QStringLiteral("Name")      //!< Composition name
#define CS_ATTR_SONG_ID    QStringLiteral("SongId")    //!< Unical id of composition for this author
#define CS_ATTR_VERSION    QStringLiteral("Version")   //!< Version of composition score (time of last edit)
#define CS_ATTR_STATUS     QStringLiteral("Status")    //!< When true, then author make it available for public, otherway its private for author
#define CS_ATTR_VOICE      QStringLiteral("Voice")     //!< Voice of main note part
#define CS_ATTR_VOICE_DUAL QStringLiteral("VoiceDual") //!< Dual Voice of main note part
#define CS_ATTR_VOICE_LEFT QStringLiteral("VoiceLeft") //!< Voice of left part of keyboard of main note part
#define CS_ATTR_STYLE      QStringLiteral("Style")     //!< Style of main chord part
#define CS_ATTR_TEMPO      QStringLiteral("Tempo")     //!< Default tempo

#define CS_STATUS_PUBLIC   QStringLiteral("Public")
#define CS_STATUS_PRIVATE  QStringLiteral("Private")


class CsComposition
  {
    QMap<QString,QString> mAttributes;

    int                   mLineStartOffset; //!< Offset of line start in tick
    int                   mTickPerPart;     //!< Part duration in tick
    int                   mPartPerTakt;     //!< Part count per takt

    CsDefList             mDefList;         //!< List of parts (tracks).

    CsLineList            mLineList;        //!< Line list of song

    CsTrainList           mTrainList;       //!< List of train interval for training

    bool                  mDirty;
  public:
    CsComposition();
    bool        isDirty() const { return mDirty; }


    //=================================================================
    //         Attributes part
    QString     attributeGet( const QString &key ) const { return mAttributes.value( key ); }
    void        attributeSet( const QString &key, const QString &v ) { if( mAttributes.contains(key) ) { mAttributes.insert( key, v ); dirtySet(); } }

    QString     songId() const { return attributeGet(CS_ATTR_SONG_ID); }

    QString     version() const { return attributeGet(CS_ATTR_VERSION); }

    bool        isPublic() const { return attributeGet(CS_ATTR_STATUS) == CS_STATUS_PUBLIC; }

    int         isMelodyPresent() const;



    //=================================================================
    //         Definitions
    auto       &defList() const { return mDefList; }

    QStringList defVisibleList() const;

    QString     defPrevVisible(const QString &key , bool rem) const;

    QString     defNextVisible(const QString &key , bool rem) const;

    int         defIndex( const QString &key ) const;

    void        defAppend( CsClass csClass, const QString &part, const QString &descr );

    void        defRename( int index, const QString &part );

    void        defRemove( int index );

    bool        defToggle( int index )  { dirtySet(); return mDefList[index].visibleToggle(); }

    auto       &def( int index ) const { return mDefList.at(index); }

    auto       &def( const QString &key ) const { return def( defIndex(key) ); }

    auto       &defRef( int index ) { dirtySet(); return mDefList[index]; }

    auto       &defRef( const QString &key ) { return defRef( defIndex(key) ); }


    //=================================================================
    //         Remark part

    auto        remarkGet( int line, const QString &lang ) const { return mLineList.at(line).textGet(lang); }

    void        remarkSet( int line, const QString &lang, const QString &rem ) { mLineList[line].textSet(lang,rem); dirtySet(); }


    //=================================================================
    //         Chord part

    auto        chordListGet( int line, const QString &part ) const { return mLineList.at(line).chordListGet(part); }

    void        chordListSet( int line, const QString &part, const CsChordList &list ) { mLineList[line].chordListSet(part,list); dirtySet(); }



    //=================================================================
    //         Note part

    int         noteClefGet( const QString &part ) const { return def(part).mClef; }

    void        noteClefSet( const QString &part, int clef ) { defRef( part ).mClef = clef; dirtySet(); }

    auto        noteListGet( int line, const QString &part ) const { return mLineList.at(line).noteListGet(part); }

    void        noteListSet( int line, const QString &part, const CsNoteList &list ) { mLineList[line].noteListSet(part,list); dirtySet(); }


    //=================================================================
    //         Note part
    auto        lyricGet( int line ) const { return mLineList.at(line).lyricLineGet(); }

    void        lyricSet( int line, const CsLyricLine &lyricLine ) { mLineList[line].lyricLineSet(lyricLine); dirtySet(); }


    //=================================================================
    //         Translation part

    auto        translationGet( int line, const QString &lang ) const { return mLineList.at(line).textGet(lang); }

    void        translationSet( int line, const QString &lang, const QString &tran ) { mLineList[line].textSet( lang, tran ); dirtySet(); }


    //=================================================================
    //         Lines

    //!
    //! \brief lineInsert Insert new line into composition
    //! \param index      Index at wich line will be inserted, if index = -1 then line appended to end of composition
    //! \param rem        If true then remark line inserted else music line inserted
    //! \return           index of new inserted line
    //!
    int         lineInsert( int index, bool rem );

    int         lineAppend( bool rem ) { return lineInsert( -1, rem ); }

    void        lineRemove( int index );

    auto       &line( int index ) const { return mLineList.at(index); }

    int         lineCount() const { return mLineList.count(); }

    int         lineTaktCount( int index ) const { return mLineList.at(index).taktCount(); }

    void        lineTaktCountSet( int index, int taktCount ) { mLineList[index].taktCountSet(taktCount); }

    int         lineTickCount( int index ) const { return lineTaktCount(index) * tickPerTakt(); }

    int         lineStartOffset() const { return mLineStartOffset; }

    void        lineStartOffsetSet( int offset ) { mLineStartOffset = offset; dirtySet(); }

    //=================================================================
    //         Tick

    int         tickPerPart() const { return qBound( 1, mTickPerPart, 256 ); }

    void        tickPerPartSet( int tpp ) { mTickPerPart = tpp; dirtySet(); }

    int         denominator() const { return 256 / tickPerPart(); }

    void        denominatorSet( int deno ) { tickPerPartSet( 256 / qBound(1, deno, 32) ); }

    int         partPerTakt() const { return mPartPerTakt; }

    void        partPerTaktSet( int ppt ) { mPartPerTakt = ppt; dirtySet(); }

    int         tickPerTakt() const { return mTickPerPart * mPartPerTakt; }


    //=================================================================
    //         Train fragments
    auto        trainGet( int index ) const { return index < 0 ? CsTrainInterval{} : mTrainList.at(index); }

    void        trainSet( int index, const CsTrainInterval &t ) { mTrainList[index] = t; dirtySet(); }

    int         trainCount() const { return mTrainList.count(); }

    int         trainAppend() { mTrainList.append( CsTrainInterval{} ); dirtySet(); return mTrainList.count() - 1; }


    //=================================================================
    //         Full composition JSON io

    void        json( SvJsonWriter &js ) const;

    void        json( SvJsonReader &js );

    void        dirtyReset() { mDirty = false; }

    void        dirtySet() { mDirty = true; }

    void        clear();

    QByteArray  toByteArray() const;

    bool        fromByteArray(const QByteArray &ar , bool stat);

    bool        fileSave();

    bool        fileCopySave();

    bool        fileLoad( const QString &songId );

  };

#endif // CSCOMPOSITION_H
