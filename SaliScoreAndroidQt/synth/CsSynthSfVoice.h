/*
 Project "SaliScore Score music edit, view and tutorial program"
 Author
   Sibilev Alexander S.
 Web
   SaliLab.com
 Description
   Single voice for synth. Voice is single or set of instruments acts as the whole.
   Voice sounds when you press a key (midi event from keyboard or midi file or accompaniment)
*/
#ifndef CSSYNTHSFVOICE_H
#define CSSYNTHSFVOICE_H

#include "CsSynthSfNote.h"

#include <QSharedPointer>

class CsSoundFont;
using CsSoundFontPtr = QSharedPointer<CsSoundFont>;

class CsSynthSfVoice
  {
    QString        mVoiceName;    //!< Voice name
    CsSynthSfNote  mNotes[128];   //!< Builded notes for preset
    CsSoundFontPtr mSoundFontPtr; //!< Sound font synth base on
    int            mVoiceId;      //!< Id of current voice
    int            mVolume;       //!< Volume of hole voice
  public:
    CsSynthSfVoice( int voiceId, const QString voiceName, CsSoundFontPtr soundFont, int preset );

    QString        voiceName() const { return mVoiceName; }

    int            voiceId() const { return mVoiceId; }

    CsSoundFontPtr soundFontPtr() const { return mSoundFontPtr; }

    //!
    //! \brief setVolume Setup master volume for all notes of preset
    //! \param volume    Volume value which need to be installed
    //!
    void           volumeSet( int volume ) { mVolume = volume; }

    int            volumeGet() const { return mVolume; }

    void           playNote( int note, int velo );
  };

#endif // CSSYNTHSFVOICE_H
