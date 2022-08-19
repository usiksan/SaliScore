#ifndef CSTRACKLIST_H
#define CSTRACKLIST_H

#include "CsConfig.h"
#include "CsTrack.h"
#include "CsClass.h"


template <int typeValue,typename CsItem>
class CsTrackList : public CsTrack
  {
    QList<CsItem> mList;
  public:
    CsTrackList() {}

    // CsTrack interface
  public:
    virtual int type() const override { return typeValue; }

    virtual void json(SvJsonWriter &js) const override
      {
      CsTrack::json(js);
      js.jsonList( js, "list", mList );
      }

    virtual void json(SvJsonReader &js) override
      {
      CsTrack::json(js);
      js.jsonList( js, "list", mList );
      }
  };


class CsTrackListChord : public CsTrackList
  {

  };

#endif // CSTRACKLIST_H
