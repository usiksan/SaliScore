#include "CsDlgDefNote.h"

CsDlgDefNote::CsDlgDefNote(CsComposition &comp, QWidget *parent) :
  CsDlgDefinition( parent ),
  mComposition(comp)
  {

  }



//!
//! \brief defList Returns definition list
//! \return        Definition list
//!
CsDefList CsDlgDefNote::defList() const
  {
  return mComposition.noteDefList();
  }



//!
//! \brief defAppend   Append new definition
//! \param title       Title (name) of definition
//! \param description Description of definition
//!
void CsDlgDefNote::defAppend(const QString &title, const QString &description)
  {
  mComposition.noteAppend( title, description );
  }




//!
//! \brief defRename Rename definition
//! \param index     Index of renamed definition
//! \param title     New title (name) of definition
//!
void CsDlgDefNote::defRename(int index, const QString &title)
  {
  mComposition.noteRename( index, title );
  }




//!
//! \brief defDelete Deletes definition
//! \param index     Index of deleted definition
//!
void CsDlgDefNote::defDelete(int index)
  {
  mComposition.noteRemove( index );
  }




//!
//! \brief defToggle Toggle visibility of definition
//! \param index     Index of toggled definition
//! \return          New state of visibility
//!
bool CsDlgDefNote::defToggle(int index)
  {
  return mComposition.noteToggle( index );
  }
