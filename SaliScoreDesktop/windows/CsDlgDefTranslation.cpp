#include "CsDlgDefTranslation.h"

CsDlgDefTranslation::CsDlgDefTranslation(CsComposition &comp, QWidget *parent) :
  CsDlgDefinition( parent ),
  mComposition(comp)
  {

  }




//!
//! \brief defList Returns definition list
//! \return        Definition list
//!
CsDefList CsDlgDefTranslation::defList() const
  {
  return mComposition.translationDefList();
  }




//!
//! \brief defAppend   Append new definition
//! \param title       Title (name) of definition
//! \param description Description of definition
//!
void CsDlgDefTranslation::defAppend(const QString &title, const QString &description)
  {
  mComposition.translationAppend( title, description );
  }




//!
//! \brief defRename Rename definition
//! \param index     Index of renamed definition
//! \param title     New title (name) of definition
//!
void CsDlgDefTranslation::defRename(int index, const QString &title)
  {
  mComposition.translationRename( index, title );
  }




//!
//! \brief defDelete Deletes definition
//! \param index     Index of deleted definition
//!
void CsDlgDefTranslation::defDelete(int index)
  {
  mComposition.translationRemove( index );
  }




//!
//! \brief defToggle Toggle visibility of definition
//! \param index     Index of toggled definition
//! \return          New state of visibility
//!
bool CsDlgDefTranslation::defToggle(int index)
  {
  return mComposition.translationToggle( index );
  }
