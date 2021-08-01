#ifndef CSNOTE_H
#define CSNOTE_H

#include "CsConfig.h"
#include "CsPosition.h"
#include "CsNoteChord.h"

#include <QList>

#define duraBreve                  512 // 2
#define duraHole                   256 // 1
#define duraHalf                   128 // 1/2
#define duraQuarter                 64 // 1/4
#define duraEighth                  32 // 1/8
#define duraSixteenth               16 // 1/16
#define duraThirtySecond             8 // 1/32
#define duraSixtyFourth              4 // 1/64
#define duraOneHundredTwentyEighth   2 // 1/128

class CsNote : public CsPosition
  {
    int mDuration; //!< Note duration, ticks
    int mPitch;    //!< Pitch from begin of one-lined octave
    int mCommand;  //!< MIDI command
  public:
    CsNote();
    CsNote( int pos, int duration, int note, int octave = octaveFirst ) : CsPosition(pos), mDuration(duration), mPitch(qBound(0,note + octave,127)), mCommand(0) {}


    int  duration() const { return mDuration; }

    int  pitch() const { return mPitch; }

    int  command() const { return mCommand; }

    int  white() const;

    bool isDies() const;

    // CsPosition interface
  public:
    virtual void jsonWrite(CsJsonWriter &js) const override;
    virtual void jsonRead(CsJsonReader &js) override;
  };

using CsNoteList = QList<CsNote>;

#endif // CSNOTE_H
