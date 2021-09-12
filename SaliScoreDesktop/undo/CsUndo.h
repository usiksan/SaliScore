#ifndef CSUNDO_H
#define CSUNDO_H

#include "CsUndoItem.h"
#include "score/CsComposition.h"

class CsUndo
  {
    CsUndoItemPtrList mUndoList;
    CsUndoItemPtrList mRedoList;
  public:
    CsUndo();
    ~CsUndo();

    bool availableUndo() const { return mUndoList.count() != 0; }

    bool availableRedo() const { return mRedoList.count() != 0; }

    void changeTitle( const QString &t );

    void changeRemark( int lineIndex, const QString &rem );

    void insertLine( int lineIndex, const CsLine &line );

    void removeLine( int lineIndex, const CsLine &line );

    void changeChordList( int lineIndex, const QString &partName, const CsChordList &list );

    void changeNoteList( int lineIndex, const QString &partName, const CsNoteList &list );

    void changeLyricList( int lineIndex, const CsLyricLine &line );

    void changeTranslation( int lineIndex, const QString &partName, const QString &tran );

  };

#endif // CSUNDO_H
