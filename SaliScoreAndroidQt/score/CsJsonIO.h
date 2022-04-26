#ifndef CSJSONIO_H
#define CSJSONIO_H

#include "CsConfig.h"
#include "SvLib/SvJsonIO.h"

using CsJsonWriter = SvJsonWriter;

struct CsJsonAttr
  {
    QString mType;    //!< Data-base type
    int     mVersion; //!< Data-base version
    bool    mStatus;  //!< True when status reading

    CsJsonAttr( bool stat ) : mType( QStringLiteral(CS_BASE_TYPE) ), mVersion(CS_BASE_VERSION), mStatus(stat) {}
    CsJsonAttr() : mType( QStringLiteral(CS_PLAY_LIST_TYPE) ), mVersion(CS_PLAY_LIST_VERSION), mStatus(false) {}

    bool isComposition() const { return mType == QStringLiteral(CS_BASE_TYPE); }

    bool isPlayList() const { return mType == QStringLiteral(CS_PLAY_LIST_TYPE); }

    void jsonWrite( SvJsonWriter &js ) const
      {
      js.jsonString( CS_BASE_TYPE_KEY, mType );
      js.jsonInt( CS_BASE_VERSION_KEY, mVersion );
      }

    void jsonRead( SvJsonReader &js )
      {
      js.jsonString( CS_BASE_TYPE_KEY, mType );
      js.jsonInt( CS_BASE_VERSION_KEY, mVersion );
      }
  };

using CsJsonReader = SvJsonReaderExt<CsJsonAttr>;

#endif // CSJSONIO_H
