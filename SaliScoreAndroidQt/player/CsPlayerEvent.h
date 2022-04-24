#ifndef CSPLAYEREVENT_H
#define CSPLAYEREVENT_H

#include <QList>

using CsKeyEvent = int;

inline CsKeyEvent csKeyEvent( int channel, int note ) { return (channel << 16) | note; }

struct CsPlayerEvent
  {
    int mChannel;    //!< Channel to which need to be sended this event
    int mNote;       //!< Note of event
    int mTickRemain; //!< Count of ticks remain to end of event

    CsPlayerEvent() {}
    CsPlayerEvent( int channel, int note, int duration ) : mChannel(channel), mNote(note), mTickRemain(duration) {}

    bool tick( int tick )
      {
      mTickRemain -= tick;
      return mTickRemain <= 0;
      }

    CsKeyEvent keyEvent() const { return csKeyEvent( mChannel, mNote ); }
  };


using CsPlayerEventList = QList<CsPlayerEvent>;

#endif // CSPLAYEREVENT_H
