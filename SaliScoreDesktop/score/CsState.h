#ifndef CSSTATE_H
#define CSSTATE_H

#include "CsDefinition.h"

#include <QStringList>



class CsState
  {
    CsDefList mRemarkList;
    CsDefList mChordList;
    CsDefList mNoteList;
    CsDefList mTranslateList;
  public:
    CsState();

    void        remarkAppend( const QString name, const QString descr );
    void        chordAppend( const QString name, const QString descr );
    void        noteAppend( const QString name, const QString descr );

    QStringList visibleRemark() const { return visibleList(mRemarkList); }

    QStringList visibleChord() const { return visibleList(mChordList); }

    QStringList visibleNote() const { return visibleList(mNoteList); }

    QStringList visibleTranslation() const { return visibleList(mTranslateList); }

    void jsonWrite( CsJsonWriter &js ) const;

    void jsonRead( CsJsonReader &js );

    static QStringList visibleList( const CsDefList &src );
  };

#endif // CSSTATE_H
