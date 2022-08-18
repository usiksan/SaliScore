/*
Project "SaliScore Score music edit, view and tutorial program"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  CsNote - is score note

  Which subclass of CsPosition CsNote contains global position in score
  and extend with pitch of note and midi command (for future extension)
*/
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

    //!
    //! \brief pitch Returns pitch of note
    //! \return      Pitch of note
    //!
    int  pitch() const { return mPitch; }

    //!
    //! \brief pitchSet Sets new pitch of note
    //! \param ptch     New pitch of note
    //!
    void pitchSet( int ptch ) { mPitch = ptch; }

    //!
    //! \brief note Return note tone inside octave (0-11)
    //! \return     One of 12 halftone
    //!
    int  note() const { return mPitch % 12; }

    //!
    //! \brief noteSet Sets new halftone for note inside current octave
    //! \param nt      One of 12 halftone
    //!
    void noteSet( int nt );

    //!
    //! \brief noteShift Shift pitch of note up or down by one halftone
    //! \param up        When true then pitch shifted up
    //!
    void noteShift( bool up );

    //!
    //! \brief durationShift Shift duration of note by 2 times
    //! \param less          When true duration decreased by 2 times else increased by 2 times
    //!
    void durationShift( bool less );

    void durationPart( bool less );

    //!
    //! \brief octaveIndex Return note octave
    //! \return            Note octave index
    //!
    int  octaveIndex() const { return mPitch / 12; }

    //!
    //! \brief octaveShift Shift pitch of note on one octave up or down
    //! \param up          When true octave shifted up
    //!
    void octaveShift( bool up );

    //!
    //! \brief command Returns current midi command
    //! \return        Current midi command
    //!
    int  command() const { return mCommand; }

    //!
    //! \brief white Returns note tone index from NoName octave (7 notes per octave)
    //! \return      Note tone index from NoName octave (7 notes per octave)
    //!
    int  white() const;

    //!
    //! \brief isDies Return true when dies need to be drawn
    //! \return       true when dies need to be drawn
    //!
    bool isDies() const;

    // CsPosition interface
  public:
    virtual void jsonWrite(CsJsonWriter &js) const override;
    virtual void jsonRead(CsJsonReader &js) override;
  };

using CsNoteList = QList<CsNote>;

#endif // CSNOTE_H
