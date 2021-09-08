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
#include "CsCompositionHeader.h"
#include "CsCursor.h"

#include <QStringList>
#include <QJsonObject>

using CsClefMap = QMap<QString,int>;

class CsCompositionSettings;

class CsComposition
  {
    CsCompositionHeader mHeader;
    QString             mComposer;
    QString             mLyricist;

    int                 mStepChord;
    int                 mStepNote;
    int                 mStepLyric;

    CsDefList           mRemarkList;
    CsDefList           mChordList;
    CsDefList           mNoteList;
    CsDefList           mTranslationList;
    CsClefMap           mClefMap;

    CsLineList          mLineList;

    mutable bool        mDirty;
    mutable bool        mStateDirty;
    //Settings
  public:
    CsComposition();

    bool        isDirty() const { return mDirty || mStateDirty; }

    //=================================================================
    //         Header part

    const CsCompositionHeader &header() const { return mHeader; }

    QString     title() const { return mHeader.name(); }
    void        titleSet( const QString &tit ) { mHeader.nameSet( tit ); dirtySet(); }

    QString     singer() const { return mHeader.singer(); }
    void        singerSet( const QString &sing ) { mHeader.singerSet( sing ); dirtySet(); }

    QString     composer() const { return mComposer; }
    void        composerSet( const QString &compos ) { mComposer = compos; dirtySet(); }

    QString     lyricist() const { return mLyricist; }
    void        lyricistSet( const QString &lyr ) { mLyricist = lyr; dirtySet(); }

    QString     author() const { return mHeader.author(); }

    QString     voice() const { return mHeader.mSettings.voice(); }
    void        voiceSet( const QString &voi ) { mHeader.mSettings.voiceSet( voi ); stateDirtySet(); }

    QString     voiceDual() const { return mHeader.mSettings.voiceDual(); }
    void        voiceDualSet( const QString &voi ) { mHeader.mSettings.voiceDualSet( voi ); stateDirtySet(); }

    QString     voiceRight() const { return mHeader.mSettings.voiceRight(); }
    void        voiceRightSet( const QString &voi ) { mHeader.mSettings.voiceRightSet( voi ); stateDirtySet(); }

    QString     style() const { return mHeader.mSettings.style(); }
    void        styleSet( const QString &stl ) { mHeader.mSettings.styleSet( stl ); stateDirtySet(); }

    QString     tempo() const { return mHeader.mSettings.tempo(); }
    void        tempoSet( const QString &tmp ) { mHeader.mSettings.tempoSet( tmp ); stateDirtySet(); }

    int         version() const { return mHeader.version(); }
    void        versionUpdate() { mHeader.versionUpdate(); dirtySet(); }

    void        makeCopy();

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

    bool        remarkToggle( int index ) { stateDirtySet(); return mRemarkList[index].visibleToggle(); }

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

    bool        chordToggle( int index ) { stateDirtySet(); return mChordList[index].visibleToggle(); }

    auto        chordListGet( int line, const QString &part ) const { return mLineList.at(line).chordListGet(part); }

    void        chordListSet( int line, const QString &part, const CsChordList &list ) { mLineList[line].chordListSet(part,list); dirtySet(); }



    //=================================================================
    //         Note part

    CsDefList   noteDefList() const { return mNoteList; }

    QStringList noteVisible() const { return visibleList(mNoteList); }

    QString     notePrevVisible( const QString &key ) const { return prevVisible( mNoteList, key ); }

    QString     noteNextVisible( const QString &key ) const { return nextVisible( mNoteList, key ); }

    CsClefMap   noteClefMap() const { return mClefMap; }

    int         noteClefGet( const QString &part ) const { return mClefMap.value(part); }

    void        noteClefSet( const QString &part, int clef ) { mClefMap.insert( part, clef ); dirtySet(); }

    int         noteIndex( const QString &part ) const { return defListIndex( mNoteList, part ); }

    void        noteAppend( const QString &part, const QString &descr, int clef = noteG );

    void        noteRename(int index, const QString &part );

    void        noteRemove( int index );

    bool        noteToggle( int index ) { stateDirtySet(); return mNoteList[index].visibleToggle(); }

    auto        noteListGet( int line, const QString &part ) const { return mLineList.at(line).noteListGet(part); }

    void        noteListSet( int line, const QString &part, const CsNoteList &list ) { mLineList[line].noteListSet(part,list); dirtySet(); }


    //=================================================================
    //         Note part
    auto        lyricGet( int line ) const { return mLineList.at(line).lyricGet(); }

    void        lyricSet( int line, const CsLyricList &list ) { mLineList[line].lyricSet(list); dirtySet(); }


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

    bool        translationToggle( int index ) { stateDirtySet(); return mTranslationList[index].visibleToggle(); }

    auto        translationGet( int line, const QString &lang ) const { return mLineList.at(line).translationGet(lang); }

    void        translationSet( int line, const QString &lang, const QString &tran ) { mLineList[line].translationSet( lang, tran ); dirtySet(); }


    //=================================================================
    //         Lines

    int         lineInsert( int index, bool rem );

    int         lineAppend( bool rem ) { return lineInsert( -1, rem ); }

    auto       &line( int index ) const { return mLineList.at(index); }

    int         lineCount() const { return mLineList.count(); }

    int         lineTaktCount( int index ) const { return mLineList.at(index).taktCount(); }

    void        lineTaktCountSet( int index, int taktCount ) { mLineList[index].taktCountSet(taktCount); }

    int         lineTickCount( int index ) const { return lineTaktCount(index) * 256; }

    //=================================================================
    //         Tick

    int         tickCount() const;


    //=================================================================
    //         Step (in ticks)

    int         stepChord() const { return mStepChord; }

    int         stepNote() const { return mStepNote; }

    int         stepLyric() const { return mStepLyric; }


    //=================================================================
    //         Full composition JSON io

    void        jsonWrite( CsJsonWriter &js ) const;

    void        jsonRead( CsJsonReader &js );

    void        dirtyReset() { mDirty = false; }

    void        dirtySet() { mStateDirty = mDirty = true; }

    void        stateDirtyReset() { mStateDirty = false; }

    void        stateDirtySet() { mStateDirty = true; }

    void        settingsRead( const CsCompositionSettings &settings );

    void        clear();

    QByteArray  toByteArray() const;

    void        fromByteArray( const QByteArray &ar );

    void        fileSave() const;

  private:
    static QStringList visibleList( const CsDefList &src );

    static QString     prevVisible( const CsDefList &list, const QString &key );

    static QString     nextVisible( const CsDefList &list, const QString &key );

    static int         defListIndex( const CsDefList &list, const QString &key );

    static void        defListUpdate( CsDefList &list, const QStringList &visibleList );
  };

#endif // CSCOMPOSITION_H
