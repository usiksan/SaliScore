#include "CsCursorEdit.h"
#include "CsCursorEditProperty.h"
#include "CsCursorEditRemark.h"
#include "CsCursorEditChord.h"
#include "CsCursorEditNote.h"
#include "CsCursorEditLyric.h"
#include "CsCursorEditTranslation.h"

CsCursorEdit::CsCursorEdit(CsComposition &comp) :
  CsCursor(),
  mComposition(comp),
  mShift(false),
  mControl(false),
  mIsEdit(false)
  {

  }

void CsCursorEdit::setPitchDuration(int pitch, int duration)
  {
  Q_UNUSED(pitch)
  Q_UNUSED(duration)
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




CsCursorEdit *CsCursorEdit::build(CsCursor &src, CsComposition &comp )
  {
  return build( src.cellClass(), src.lineIndex(), src.linePosition(), src.partName(), comp );
  }




CsCursorEdit *CsCursorEdit::build( CsClass cellClass, int lineIndex, int position, const QString partName, CsComposition &comp)
  {
  switch( cellClass ) {
    case cccAttribute :
      return new CsCursorEditProperty( partName, comp );

    case cccRemark :
      return new CsCursorEditRemark( lineIndex, partName, comp );

    case cccChord :
      return new CsCursorEditChord( lineIndex, position, partName, comp );

    case cccNote :
      return new CsCursorEditNote( lineIndex, position, partName, comp );

    case cccLyric :
      return new CsCursorEditLyric( lineIndex, comp, -1 );

    case cccLyricSymbol :
      return new CsCursorEditLyric( lineIndex, comp, position );

    case cccTranslation :
      return new CsCursorEditTranslation( lineIndex, partName, comp );

    default:
      return nullptr;
    }
  return nullptr;
  }
