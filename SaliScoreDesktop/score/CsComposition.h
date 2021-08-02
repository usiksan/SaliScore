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

#include <QStringList>
#include <QJsonObject>

using CsClefMap = QMap<QString,int>;

class CsComposition
  {
    QString      mTitle;
    QString      mSinger;
    QString      mComposer;
    QString      mLyricist;

    int          mVoice;
    int          mVoiceDual;
    int          mVoiceRight;
    int          mStyle;
    int          mTempo;

    CsDefList    mRemarkList;
    CsDefList    mChordList;
    CsDefList    mNoteList;
    CsDefList    mTranslationList;
    CsClefMap    mClefMap;

    CsLineList   mLineList;

    mutable bool mDirty;

    //Settings
  public:
    CsComposition();

    bool        isDirty() const { return mDirty; }

    //=================================================================
    //         Remark part

    CsDefList   remarkDefList() const { return mRemarkList; }

    QStringList remarkVisible() const { return visibleList(mRemarkList); }

    int         remarkIndex( const QString &lang ) const { return defListIndex( mRemarkList, lang ); }

    void        remarkAppend( const QString &lang, const QString &descr );

    void        remarkRename( int index, const QString &lang );

    void        remarkRemove( int index );

    bool        remarkToggle( int index ) { return mRemarkList[index].visibleToggle(); }

    auto        remarkGet( int line, const QString &lang ) const { return mLineList.at(line).remarkGet(lang); }

    void        remarkSet( int line, const QString &lang, const QString &rem ) { mDirty = true; mLineList[line].remarkSet(lang,rem); }


    //=================================================================
    //         Chord part

    CsDefList   chordDefList() const { return mChordList; }

    QStringList chordVisible() const { return visibleList(mChordList); }

    int         chordIndex( const QString &part ) const { return defListIndex( mChordList, part );}

    void        chordAppend( const QString &part, const QString &descr );

    void        chordRename( int index, const QString &part );

    void        chordRemove( int index );

    bool        chordToggle( int index ) { return mChordList[index].visibleToggle(); }

    auto        chordListGet( int line, const QString &part ) const { return mLineList.at(line).chordListGet(part); }

    void        chordListSet( int line, const QString &part, const CsChordList &list ) { mDirty = true; mLineList[line].chordListSet(part,list); }



    //=================================================================
    //         Note part

    QStringList noteVisible() const { return visibleList(mNoteList); }

    CsClefMap   noteClefMap() const { return mClefMap; }

    int         noteClefGet( const QString &part ) const { return mClefMap.value(part); }

    void        noteClefSet( const QString &part, int clef ) { mDirty = true; mClefMap.insert( part, clef ); }

    int         noteIndex( const QString &part ) const { return defListIndex( mNoteList, part ); }

    void        noteAppend( const QString &part, const QString &descr, int clef = noteG );

    void        noteRename( int index, const QString &lang );

    void        noteRemove( int index );

    void        noteToggle( int index, bool on ) { mNoteList[index].mVisible = on; }

    auto        noteListGet( int line, const QString &part ) const { return mLineList.at(line).noteListGet(part); }

    void        noteListSet( int line, const QString &part, const CsNoteList &list ) { mDirty = true; mLineList[line].noteListSet(part,list); }


    //=================================================================
    //         Note part
    auto        lyricGet( int line ) const { return mLineList.at(line).lyricGet(); }

    void        lyricSet( int line, const CsLyricList &list ) { mDirty = true; mLineList[line].lyricSet(list); }


    //=================================================================
    //         Translation part

    QStringList translationVisible() const { return visibleList(mTranslationList); }

    int         translationIndex( const QString &lang ) const { return defListIndex( mTranslationList, lang ); }

    void        translationAppend( const QString &lang, const QString &descr );

    void        translationRename( int index, const QString &lang );

    void        translationRemove( int index );

    void        translationToggle( int index, bool on ) { mTranslationList[index].mVisible = on; }

    auto        translationGet( int line, const QString &lang ) const { return mLineList.at(line).translationGet(lang); }

    void        translationSet( int line, const QString &lang, const QString &tran ) { mDirty = true; mLineList[line].translationSet( lang, tran ); }


    //=================================================================
    //         Lines

    int         lineInsert( int index, bool rem );

    int         lineAppend( bool rem ) { return lineInsert( -1, rem ); }

    auto       &line( int index ) const { return mLineList.at(index); }

    int         lineCount() const { return mLineList.count(); }





    //=================================================================
    //         Full composition JSON io

    void        jsonWrite( CsJsonWriter &js ) const;

    void        jsonRead( CsJsonReader &js );

    void        dirtyReset() { mDirty = false; }

    void        dirtySet() { mDirty = true; }



    //=================================================================
    //         Settings JSON io

    void        settingsWrite( CsJsonWriter &js ) const;

    void        settingsRead( CsJsonReader &js );

  private:
    static QStringList visibleList( const CsDefList &src );

    static int         defListIndex( const CsDefList &list, const QString &key );
  };

#endif // CSCOMPOSITION_H
