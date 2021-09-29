#include "CsDlgDefChord.h"

CsDlgDefChord::CsDlgDefChord(bool editable, CsComposition &comp, QWidget *parent ) :
  CsDlgDefinition( editable, parent ),
  mComposition(comp)
  {

  }




//!
//! \brief defList Returns definition list
//! \return        Definition list
//!
CsDefList CsDlgDefChord::defList() const
  {
  return mComposition.chordDefList();
  }




//!
//! \brief defAppend   Append new definition
//! \param title       Title (name) of definition
//! \param description Description of definition
//!
void CsDlgDefChord::defAppend(const QString &title, const QString &description)
  {
  mComposition.chordAppend( title, description );
  }





//!
//! \brief defRename Rename definition
//! \param index     Index of renamed definition
//! \param title     New title (name) of definition
//!
void CsDlgDefChord::defRename(int index, const QString &title)
  {
  mComposition.chordRename( index, title );
  }





//!
//! \brief defDelete Deletes definition
//! \param index     Index of deleted definition
//!
void CsDlgDefChord::defDelete(int index)
  {
  mComposition.chordRemove( index );
  }




//!
//! \brief defToggle Toggle visibility of definition
//! \param index     Index of toggled definition
//! \return          New state of visibility
//!
bool CsDlgDefChord::defToggle(int index)
  {
  return mComposition.chordToggle( index );
  }
