#ifndef CSTRACKSTRING_H
#define CSTRACKSTRING_H

#include "CsConfig.h"
#include "CsTrack.h"
#include "CsClass.h"

template <int typeValue>
class CsTrackString : public CsTrack
  {
    QString mText;
  public:
    CsTrackString() {}

    // CsTrack interface
  public:

    virtual int     type() const override { return typeValue; }


    virtual QString textGet() const override { return mText; }

    virtual void    textSet(const QString &t) override { mText = t; }


    virtual void json(SvJsonWriter &js) const override
      {
      CsTrack::json(js);
      js.jsonString( "text", mText );
      }

    virtual void json(SvJsonReader &js) override
      {
      CsTrack::json(js);
      js.jsonString( "text", mText );
      }

  };


using CsTrackStringRemark      = CsTrackString<cccRemark>;
using CsTrackStringTranslation = CsTrackString<cccTranslation>;


#endif // CSTRACKSTRING_H
