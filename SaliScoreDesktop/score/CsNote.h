#ifndef CSNOTE_H
#define CSNOTE_H

#include "CsConfig.h"
#include "CsPosition.h"
#include "CsNoteChord.h"

#include <QList>

class CsNote : public CsPosition
  {
    int mPitch;    //!< Pitch from begin of one-lined octave
    int mCommand;  //!< MIDI command
  public:
    CsNote();
    CsNote( int pos, int duration, int note, int octave = octaveFirst ) : CsPosition(pos,duration), mPitch(qBound(0,note + octave,127)), mCommand(0) {}

    int  pitch() const { return mPitch; }

    int  note() const { return mPitch % 12; }

    void noteSet( int nt );

    void noteShift( bool up );

    int  octaveIndex() const { return mPitch / 12; }

    void octaveShift( bool up );

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
