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

class CsComposition
  {
    QString    mTitle;
    QString    mSinger;
    QString    mComposer;
    QString    mLyricist;

    CsDefList  mRemarkList;
    CsDefList  mChordList;
    CsDefList  mNoteList;
    CsDefList  mTranslateList;

    CsLineList mLineList;

    //Settings
  public:
    CsComposition();

    //=================================================================
    //         Remark part

    QStringList remarkVisible() const { return visibleList(mRemarkList); }

    int         remarkIndex( const QString &lang ) const { return defListIndex( mRemarkList, lang ); }

    void        remarkAppend( const QString &lang, const QString &descr );

    void        remarkRemove( int index );

    void        remarkToggle( int index, bool on ) { mRemarkList[index].mVisible = on; }



    //=================================================================
    //         Chord part

    QStringList chordVisible() const { return visibleList(mChordList); }

    int         chordIndex( const QString &part ) const { return defListIndex( mChordList, part );}

    void        chordAppend( const QString &part, const QString &descr );

    void        chordRemove( int index );

    void        chordToggle( int index, bool on ) { mChordList[index].mVisible = on; }



    //=================================================================
    //         Note part

    QStringList noteVisible() const { return visibleList(mNoteList); }

    int         noteIndex( const QString &part ) const { return defListIndex( mNoteList, part ); }

    void        noteAppend( const QString &part, const QString &descr );

    void        noteRemove( int index );

    void        noteToggle( int index, bool on ) { mNoteList[index].mVisible = on; }



    //=================================================================
    //         Translation part

    QStringList translationVisible() const { return visibleList(mTranslateList); }

    int         translationIndex( const QString &lang ) const { return defListIndex( mTranslateList, lang ); }

    void        translationAppend( const QString &lang, const QString &descr );

    void        translationRemove( int index );

    void        translateToggle( int index, bool on ) { mTranslateList[index].mVisible = on; }



    //=================================================================
    //         Lines

    void        lineInsert( int index, const CsLine &line );

    void        lineAppend( const CsLine &line ) { lineInsert( -1, line ); }

    auto       &lineList() const { return mLineList; }




    //=================================================================
    //         Full composition JSON io

    void        jsonWrite( CsJsonWriter &js ) const;

    void        jsonRead( CsJsonReader &js );



    //=================================================================
    //         Settings JSON io

    void        settingsWrite( CsJsonWriter &js ) const;

    void        settingsRead( CsJsonReader &js );

  private:
    static QStringList visibleList( const CsDefList &src );

    static int         defListIndex( const CsDefList &list, const QString &key );
  };

#endif // CSCOMPOSITION_H
