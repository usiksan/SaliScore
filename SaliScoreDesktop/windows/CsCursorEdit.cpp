#include "CsCursorEdit.h"
#include "CsCursorEditProperty.h"

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

    default:
      return nullptr;
    }
  return nullptr;
  }
