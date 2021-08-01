#include "CsDlgDefRemark.h"

CsDlgDefRemark::CsDlgDefRemark(CsComposition &comp, QWidget *parent) :
  CsDlgDefinition(parent),
  mComposition(comp)
  {

  }


CsDefList CsDlgDefRemark::defList() const
  {
  return mComposition.remarkDefList();
  }




void CsDlgDefRemark::defAppend(const QString &title, const QString &description)
  {
  mComposition.remarkAppend( title, description );
  if( parentWidget() != nullptr )
    parentWidget()->update();
  }




void CsDlgDefRemark::defRename(int index, const QString &title)
  {
  mComposition.remarkRename( index, title );
  if( parentWidget() != nullptr )
    parentWidget()->update();
  }




void CsDlgDefRemark::defDelet(int index)
  {
  mComposition.remarkRemove(index);
  if( parentWidget() != nullptr )
    parentWidget()->update();
  }




void CsDlgDefRemark::defToggle(int index, bool on)
  {
  mComposition.remarkToggle( index, on );
  if( parentWidget() != nullptr )
    parentWidget()->update();
  }
