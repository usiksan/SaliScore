#ifndef CSCOMPOSITIONINFO_H
#define CSCOMPOSITIONINFO_H

#include "CsComposition.h"

class CsCompositionInfo
  {
    QString mName;
    QString mAuthor;
    QString mSinger;
    QString mVersion;
  public:
    CsCompositionInfo();

    CsCompositionInfo( const CsComposition &comp ) :
      mName(comp.attributeGet( CS_ATTR_NAME )),
      mAuthor(comp.attributeGet( CS_ATTR_AUTHOR )),
      mSinger(comp.attributeGet( CS_ATTR_SINGER )),
      mVersion(comp.attributeGet( CS_ATTR_VERSION ))
      {
      }

    QString name() const { return mName; }

    QString author() const { return mAuthor; }

    QString singer() const { return mSinger; }

    QString version() const { return mVersion; }

    void jsonWrite( CsJsonWriter &js ) const
      {
      js.jsonString( "Name", mName );
      js.jsonString( "Author", mAuthor );
      js.jsonString( "Singer", mSinger );
      js.jsonString( "Version", mVersion );
      }

    void jsonRead( CsJsonReader &js )
      {
      js.jsonString( "Name", mName );
      js.jsonString( "Author", mAuthor );
      js.jsonString( "Singer", mSinger );
      js.jsonString( "Version", mVersion );
      }
  };

#endif // CSCOMPOSITIONINFO_H
