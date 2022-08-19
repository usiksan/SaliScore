/*
 Project "SaliScore Score music edit, view and tutorial program"
 Author
   Sibilev Alexander S.
 Web
   SaliLab.com
 Description
   Single note for synth. Note is single or set of sound played simultaneously.
   For this, note consist of one or more tracks each of that is separate sound
   generator.
   Note sounds when midi events.
*/
#ifndef CSSYNTHSFNOTE_H
#define CSSYNTHSFNOTE_H

#include "audioOut/CsSoundSource.h"
#include "CsSynthSfTrack.h"

class SfSample;

//Note consist of one or more tracks
//Preset or program is 128 or less notes
class CsSynthSfNote : public CsSoundSource
  {
  public:
  protected:
    CsSynthSfTrackVector mTracks;  //!< Tracks of which consist a note
    int                  mNote;    //!< Note index
    bool                 mStopped; //!< Flag, true when no note sounds
  public:
    CsSynthSfNote();

    //Set note index
    void                  setNote( int note ) { mNote = note; }

    //Clear builded note to its default cleared nondefined state
    void                  clear();

    //!
    //! \brief sample Return next sample of sound for this note
    //! \return       Next sample of sound
    //!
    virtual CsSoundSample sample() override;

    //!
    //! \brief isStopped Return true when sound is stopped and can be remove from play list
    //! \return          True when sound is stopped and can be remove from play list
    //!
    virtual bool          isStopped() const override { return mStopped; }

    void                  noteOff();

    bool                  noteOn( quint8 pressure );

    //!
    //! \brief addTrack         Build track for note and add it to the note track vector
    //! \param generator        Current generator state
    //! \param sam              Sample descriptor for this track
    //! \param samples          Samples array from begin of which by other params we select fragment
    //! \param endSample        Index where samples fragment is ended
    //! \param startLoop        Index where loop starts in samples fragment
    //! \param endLoop          Index where loop ends in samples fragment
    //! \param delayVolEnv      Delay after key on and before sound
    //! \param attackVolEnv     Attack phase time
    //! \param holdVolEnv       Hold phase time
    //! \param decayVolEnv      Decay phase time
    //! \param releaseVolEnv    Release phase time
    //! \return                 true if track builded and added successfuly
    //!
    bool                   addTrack( quint16 *generator, const SfSample &sam, qint16 *samples, int endSample,
                                     int startLoop, int endLoop, int delayVolEnv, int attackVolEnv, int holdVolEnv,
                                     int decayVolEnv, int releaseVolEnv, int *masterVolume );

  };

#endif // CSSYNTHSFNOTE_H
