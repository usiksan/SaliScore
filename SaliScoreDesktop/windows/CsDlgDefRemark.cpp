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
  }




void CsDlgDefRemark::defRename(int index, const QString &title)
  {
  mComposition.remarkRename( index, title );
  }




void CsDlgDefRemark::defDelete(int index)
  {
  mComposition.remarkRemove(index);
  }




bool CsDlgDefRemark::defToggle(int index)
  {
  return mComposition.remarkToggle( index );
  }

