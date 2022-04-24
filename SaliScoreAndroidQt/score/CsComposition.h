/*
Project "SaliScore Score music edit, view and tutorial programm"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Hole composition definition. Composition is a single song
*/
#ifndef CSCOMPOSITION_H
#define CSCOMPOSITION_H

#include "CsConfig.h"
#include "CsLine.h"
#include "CsDefinition.h"
#include "CsTrainInterval.h"
#include "CsSongLocalRepo.h"

#include <QStringList>
#include <QJsonObject>

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

#define CS_STATUS_PUBLIC  QStringLiteral("Public")
#define CS_STATUS_PRIVATE QStringLiteral("Private")


class CsComposition
  {
    CsKitOfString       mAttributes;

    int                 mLineStartOffset;        //!< Offset of line start in tick
    int                 mTickPerPart;            //!< Part duration in tick
    int                 mPartPerTakt;            //!< Part count per takt

    int                 mStepChord;
    int                 mStepNote;
    int                 mStepLyric;

    CsDefList           mRemarkList;             //!< List of remark parts. Each part may be in different language
    CsDefList           mChordList;              //!< List of chord parts. Each part may be for different accompanement or difficulties
    CsDefList           mNoteList;               //!< List of note parts. Each part may be for different instrument or difficulties
    CsDefList           mTranslationList;        //!< List of translation parts. Each part is translation of lyric to single language

    CsLineList          mLineList;               //!< Line list of song

    CsTrainList         mTrainList;              //!< List of train interval for training

    bool                mDirty;
    //Settings
  public:
    CsComposition();

    bool        isDirty() const { return mDirty; }


    //=================================================================
    //         Attributes part
    QString     attributeGet( const QString &key ) const { return mAttributes.get( key ); }
    void        attributeSet( const QString &key, const QString &v ) { if( mAttributes.contains(key) ) { mAttributes.set( key, v ); dirtySet(); } }

    QString     songId() const { return attributeGet(CS_ATTR_SONG_ID); }

    QString     version() const { return attributeGet(CS_ATTR_VERSION); }

    bool        isPublic() const { return attributeGet(CS_ATTR_STATUS) == CS_STATUS_PUBLIC; }

    int         isMelodyPresent() const { return mNoteList.count(); }



    //=================================================================
    //         Remark part

    CsDefList   remarkDefList() const { return mRemarkList; }

    QStringList remarkVisible() const { return visibleList(mRemarkList); }

    QString     remarkPrevVisible( const QString &key ) const { return prevVisible( mRemarkList, key ); }

    QString     remarkNextVisible( const QString &key ) const { return nextVisible( mRemarkList, key ); }

    int         remarkIndex( const QString &lang ) const { return defListIndex( mRemarkList, lang ); }

    void        remarkAppend( const QString &lang, const QString &descr );

    void        remarkRename( int index, const QString &lang );

    void        remarkRemove( int index );

    bool        remarkToggle( int index ) { dirtySet(); return mRemarkList[index].visibleToggle(); }

    auto        remarkGet( int line, const QString &lang ) const { return mLineList.at(line).remarkGet(lang); }

    void        remarkSet( int line, const QString &lang, const QString &rem ) { mLineList[line].remarkSet(lang,rem); dirtySet(); }


    //=================================================================
    //         Chord part

    CsDefList   chordDefList() const { return mChordList; }

    QStringList chordVisible() const { return visibleList(mChordList); }

    QString     chordPrevVisible( const QString &key ) const { return prevVisible( mChordList, key ); }

    QString     chordNextVisible( const QString &key ) const { return nextVisible( mChordList, key ); }

    int         chordIndex( const QString &part ) const { return defListIndex( mChordList, part );}

    void        chordAppend( const QString &part, const QString &descr );

    void        chordRename( int index, const QString &part );

    void        chordRemove( int index );

    bool        chordToggle( int index ) { dirtySet(); return mChordList[index].visibleToggle(); }

    auto        chordListGet( int line, const QString &part ) const { return mLineList.at(line).chordListGet(part); }

    void        chordListSet( int line, const QString &part, const CsChordList &list ) { mLineList[line].chordListSet(part,list); dirtySet(); }



    //=================================================================
    //         Note part

    CsDefList   noteDefList() const { return mNoteList; }

    QStringList noteVisible() const { return visibleList(mNoteList); }

    QString     notePrevVisible( const QString &key ) const { return prevVisible( mNoteList, key ); }

    QString     noteNextVisible( const QString &key ) const { return nextVisible( mNoteList, key ); }

    int         noteClefGet( const QString &part ) const { return mNoteList.at(noteIndex(part)).mClef; }

    void        noteClefSet( const QString &part, int clef ) { mNoteList[noteIndex(part)].mClef = clef; dirtySet(); }

    int         noteIndex( const QString &part ) const { return defListIndex( mNoteList, part ); }

    void        noteAppend( const QString &part, const QString &descr, int clef = noteG );

    void        noteRename(int index, const QString &part );

    void        noteRemove( int index );

    bool        noteToggle( int index ) { dirtySet(); return mNoteList[index].visibleToggle(); }

    auto        noteListGet( int line, const QString &part ) const { return mLineList.at(line).noteListGet(part); }

    void        noteListSet( int line, const QString &part, const CsNoteList &list ) { mLineList[line].noteListSet(part,list); dirtySet(); }


    //=================================================================
    //         Note part
    auto        lyricGet( int line ) const { return mLineList.at(line).lyricGet(); }

    void        lyricSet( int line, const CsLyricLine &lyricLine ) { mLineList[line].lyricSet(lyricLine); dirtySet(); }


    //=================================================================
    //         Translation part

    CsDefList   translationDefList() const { return mTranslationList; }

    QStringList translationVisible() const { return visibleList(mTranslationList); }

    QString     translationPrevVisible( const QString &key ) const { return prevVisible( mTranslationList, key ); }

    QString     translationNextVisible( const QString &key ) const { return nextVisible( mTranslationList, key ); }

    int         translationIndex( const QString &lang ) const { return defListIndex( mTranslationList, lang ); }

    void        translationAppend( const QString &lang, const QString &descr );

    void        translationRename( int index, const QString &lang );

    void        translationRemove( int index );

    bool        translationToggle( int index ) { dirtySet(); return mTranslationList[index].visibleToggle(); }

    auto        translationGet( int line, const QString &lang ) const { return mLineList.at(line).translationGet(lang); }

    void        translationSet( int line, const QString &lang, const QString &tran ) { mLineList[line].translationSet( lang, tran ); dirtySet(); }


    //=================================================================
    //         Lines

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

    int         tickCount() const;

    int         tickPerPart() const { return qBound( 1, mTickPerPart, 256 ); }

    void        tickPerPartSet( int tpp ) { mTickPerPart = tpp; dirtySet(); }

    int         denominator() const { return 256 / tickPerPart(); }

    void        denominatorSet( int deno ) { tickPerPartSet( 256 / qBound(1, deno, 32) ); }

    int         partPerTakt() const { return mPartPerTakt; }

    void        partPerTaktSet( int ppt ) { mPartPerTakt = ppt; dirtySet(); }

    int         tickPerTakt() const { return mTickPerPart * mPartPerTakt; }


    //=================================================================
    //         Step (in ticks)

    int         stepChord() const { return mStepChord; }

    int         stepNote() const { return mStepNote; }

    int         stepLyric() const { return mStepLyric; }


    //=================================================================
    //         Train fragments
    auto        trainGet( int index ) const { return mTrainList.at(index); }

    void        trainSet( int index, const CsTrainInterval &t ) { mTrainList[index] = t; dirtySet(); }

    int         trainCount() const { return mTrainList.count(); }

    int         trainAppend() { mTrainList.append( CsTrainInterval{} ); dirtySet(); return mTrainList.count() - 1; }


    //=================================================================
    //         Full composition JSON io

    void        jsonWrite( CsJsonWriter &js ) const;

    void        jsonRead( CsJsonReader &js );

    void        dirtyReset() { mDirty = false; }

    void        dirtySet() { mDirty = true; }

    void        clear();

    QByteArray  toByteArray() const;

    bool        fromByteArray(const QByteArray &ar , bool stat);

    bool        fileSave();

    bool        fileCopySave();

    bool        fileLoad( const QString &songId );

  private:
    static QStringList     visibleList( const CsDefList &src );

    static QString         prevVisible( const CsDefList &list, const QString &key );

    static QString         nextVisible( const CsDefList &list, const QString &key );

    static int             defListIndex( const CsDefList &list, const QString &key );
  };

#endif // CSCOMPOSITION_H
