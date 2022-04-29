#ifndef CSSYNTHSFVOICEINFO_H
#define CSSYNTHSFVOICEINFO_H

#include "SvLib/SvJsonIO.h"

#include <QMap>
#include <QSharedPointer>

class CsSynthSfVoice;

using CsSynthSfVoicePtr = QSharedPointer<CsSynthSfVoice>;

enum CsTembrClass {
  ctcNone,
  ctcPiano,
  ctcEPiano,
  ctcOrgan,
  ctcAccordion,
  ctcGuitar,
  ctcBass,
  ctcStrings,
  ctcChoir,
  ctcSaxophone,
  ctcTrumpet,
  ctcBrass,
  ctcFluite,
  ctcSynthLeed,
  ctcSynthPad,
  ctcPercussion,
  ctcWorld,
  ctcDrumKits
};

class CsSynthSfVoiceInfo
  {
    CsSynthSfVoicePtr mVoice;        //!< Voice synth
    QString           mFontName;     //!< Font name file for this voice
    QString           mPresetName;   //!< Preset name in font file
    QString           mName;         //!< Visual name of voice
    QString           mClass;        //!< Tembr class name
    int               mMidiBank;     //!< MIDI bank index
    int               mMidiProgram; //!< MIDI program index
  public:
    CsSynthSfVoiceInfo();

    CsSynthSfVoice *voice() { return mVoice.data(); }

    void jsonWrite( SvJsonWriter &js ) const;

    void jsonRead( SvJsonReader &js );
  };

#endif // CSSYNTHSFVOICEINFO_H
