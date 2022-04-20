#include "CsCursor.h"

CsCursor::CsCursor() :
  mClass(cccInvalid) //!< Class of cursor, one of CsClass
  {

  }





//!
//! \brief isMatchClass Returns true if cursor's class match to any of classes in vector
//! \param v            Vector of classes
//! \return             true if cursor's class match to any of classes in vector
//!
bool CsCursor::isMatchClass(const QVector<int> &v) const
  {
  for( auto val : v )
    if( val == mClass ) return true;
  return false;
  }




//!
//! \brief isMatch   Check if cursor match to theClass, lineIndex and partName
//! \param theClass  Class which need to be checked
//! \param lineIndex Line index which need to be checked
//! \param partName  Part name which need to be checked
//! \return          true if theClass, lineIndex and partName exactly equals to cursor's one
//!
bool CsCursor::isMatch(int theClass, int lineIndex, const QString &partName) const
  {
  return theClass == mClass && lineIndex == mLineIndex && partName == mPartName;
  }





//!
//! \brief isMatch   Check if cursor match to theClass, position, lineIndex and partName
//! \param theClass  Class which need to be checked
//! \param position  Position which need to be checked
//! \param lineIndex Line index which need to be checked
//! \param partName  Part name which need to be checked
//! \return          true if theClass, position, lineIndex and partName exactly equals to cursor's one
//!
bool CsCursor::isMatch(int theClass, int position, int lineIndex, const QString &partName) const
  {
  return theClass == mClass && position == mLinePosition && lineIndex == mLineIndex && partName == mPartName;
  }




//!
//! \brief isMatch   Check if cursor match to theClass, position and lineIndex
//! \param theClass  Class which need to be checked
//! \param position  Position which need to be checked
//! \param lineIndex Line index which need to be checked
//! \return          true if theClass, position and lineIndex exactly equals to cursor's one
//!
bool CsCursor::isMatch(int theClass, int position, int lineIndex) const
  {
  return theClass == mClass && position == mLinePosition && lineIndex == mLineIndex;
  }
