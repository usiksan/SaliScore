#include "CsComposition.h"

CsComposition::CsComposition()
  {

  }


int CsComposition::isMelodyPresent() const
  {
  for( const auto &def : qAsConst(mDefList) )
    if( def.mClass == cccNote )
      return true;
  return false;
  }


QStringList CsComposition::defVisibleList() const
  {
  QStringList list;
  for( const auto &def : qAsConst(mDefList) )
    if( def.mVisible )
      list.append( def.mName );
  return list;
  }




QString CsComposition::defPrevVisible( const QString &key, bool rem ) const
  {
  int i = key.isEmpty() ? mDefList.count() - 1 : defIndex( key ) - 1;
  if( i < 0 )
    return QString{};
  while( i >= 0 )
    if( mDefList.at(i).mVisible && mDefList.at(i).isRemark() == rem )
      return mDefList.at(i).mName;
    else i--;
  return QString{};
  }




QString CsComposition::defNextVisible( const QString &key, bool rem ) const
  {
  int i = key.isEmpty() ? 0 : defIndex( key ) + 1;
  if( i < 0 )
    return QString{};
  while( i < mDefList.count() )
    if( mDefList.at(i).mVisible && mDefList.at(i).isRemark() == rem )
      return mDefList.at(i).mName;
    else i++;
  return QString{};
  }




int CsComposition::defIndex(const QString &key) const
  {
  //Prepare index for finding
  int i = 0;

  //Scan list and test for equal name
  for( const auto &def : qAsConst(mDefList) )
    if( def.mName == key )
      return i;
    else
      i++;

  //Key not found in list
  return -1;
  }



void CsComposition::defAppend(CsClass csClass, const QString &part, const QString &descr)
  {
  if( defIndex(part) < 0 ) {
    CsDefinition df( csClass, part, descr );
    //Find insertion point
    int i;
    for( i = 0; i < mDefList.count(); i++ )
      if( mDefList.at(i).classGet() > csClass ) {
        mDefList.insert( i, df );
        break;
        }
    if( i >= mDefList.count() )
      mDefList.append( df );

    //For all lines append part
    bool isRemark = csClass == cccRemark;
    for( auto &line : mLineList )
      if( line.isRemark() == isRemark )
        line.textSet( part, QString{} );
    }
  }




void CsComposition::defRename(int index, const QString &part)
  {
  if( index >= 0 ) {
    QString prevPart = mDefList.at(index).mName;
    //Replace name in definition
    mDefList[index].mName = part;
    //Replace part name in all lines
    for( auto &line : mLineList )
      line.partRename( prevPart, part );
    }
  }




void CsComposition::defRemove(int index)
  {
  if( index >= 0 ) {
    QString part = mDefList.at(index).mName;
    //Remove from definition
    mDefList.removeAt(index);
    //Remove part in all lines
    for( auto &line : mLineList )
      line.partRemove( part );
    }
  }


//!
//! \brief lineInsert Insert new line into composition
//! \param index      Index at wich line will be inserted, if index = -1 then line appended to end of composition
//! \param rem        If true then remark line inserted else music line inserted
//! \return           index of new inserted line
//!
int CsComposition::lineInsert( int index, bool rem )
  {
  dirtySet();
  //Create new line
  CsLine line;
  if( rem ) {
    //We need to create remark line
    //Append all remark languages
    for( auto const &def : qAsConst(mDefList) )
      if( def.classGet() == cccRemark )
        line.textSet( def.mName, QString{} );
    }
  else {
    //We build song line
    //Append all except remark
    for( auto const &def : qAsConst(mDefList) )
      if( def.classGet() != cccRemark )
        line.textSet( def.mName, QString{} );
    }

  //Line created, insert
  if( index < 0 ) {
    mLineList.append( line );
    return mLineList.count() - 1;
    }
  mLineList.insert( index, line );
  return index;
  }


void CsComposition::lineRemove( int index )
  {
  dirtySet();
  mLineList.removeAt( index );
  }



void CsComposition::json(SvJsonWriter &js) const
  {
  js.jsonValue( "attributes", mAttributes );
  js.jsonInt( "lineStartOffset", mLineStartOffset );
  js.jsonInt( "tickPerPart", mTickPerPart );
  js.jsonInt( "partPerTakt", mPartPerTakt );

  js.jsonList( js, "definitionList", mDefList );

  js.jsonList( js, "lineList", mLineList );

  js.jsonList( js, "trainList", mTrainList );
  }



void CsComposition::json(SvJsonReader &js)
  {
  js.jsonValue( "attributes", mAttributes );
  js.jsonInt( "lineStartOffset", mLineStartOffset );
  js.jsonInt( "tickPerPart", mTickPerPart );
  js.jsonInt( "partPerTakt", mPartPerTakt );

  js.jsonList( js, "definitionList", mDefList );

  js.jsonList( js, "lineList", mLineList );

  js.jsonList( js, "trainList", mTrainList );

  mDirty = false;
  }
