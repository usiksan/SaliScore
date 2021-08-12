#include "CsCursorEdit.h"
#include "CsCursorEditProperty.h"
#include "CsCursorEditRemark.h"
#include "CsCursorEditChord.h"
#include "CsCursorEditLyric.h"
#include "CsCursorEditTranslation.h"

CsCursorEdit::CsCursorEdit(CsComposition &comp) :
  CsCursor(),
  mComposition(comp),
  mShift(false),
  mControl(false)
  {

  }

void CsCursorEdit::keyPress(int key, QChar ch, CsCursorEdit::CsCursorEditPtr &ptr)
  {
  Q_UNUSED(ch)
  switch( key ) {
    case Qt::Key_Escape :
      cancel();
      delete ptr;
      ptr = nullptr;
      break;

    case Qt::Key_Return :
    case Qt::Key_Enter :
      apply();
      delete ptr;
      ptr = nullptr;
      break;

    case Qt::Key_Shift :
      mShift = true;
      break;

    case Qt::Key_Control :
      mControl = true;
      break;
    }

  }

void CsCursorEdit::keyRelease(int key, QChar ch)
  {
  Q_UNUSED(ch)
  switch( key ) {

    case Qt::Key_Shift :
      mShift = false;
      break;

    case Qt::Key_Control :
      mControl = false;
      break;
    }
  }




CsCursorEdit *CsCursorEdit::build(CsCursor &src, CsComposition &comp)
  {
  switch( src.cellClass() ) {
    case cccTitle :
    case cccVoice :
    case cccSinger :

    case cccVoiceDual :
    case cccComposer :

    case cccVoiceRight :
    case cccLyricist :

    case cccStyle :
    case cccAuthor :

    case cccTempo :
      return new CsCursorEditProperty( src.cellClass(), comp );

    case cccRemark :
      return new CsCursorEditRemark( src.lineIndex(), src.partName(), comp );

    case cccChord :
      return new CsCursorEditChord( src.lineIndex(), src.position(), src.partName(), comp );

    case cccLyric :
      return new CsCursorEditLyric( src.lineIndex(), src.position(), comp );

    case cccTranslation :
      return new CsCursorEditTranslation( src.lineIndex(), src.partName(), comp );

    default:
      return nullptr;
    }
  return nullptr;
  }
