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
    int               mPreset;       //!< Preset index in sound font file
    QString           mName;         //!< Visual name of voice
    QString           mTembrClass;   //!< Tembr class name
    int               mMidiBank;     //!< MIDI bank index
    int               mMidiProgram;  //!< MIDI program index
  public:
    CsSynthSfVoiceInfo();
    CsSynthSfVoiceInfo( const QString &fontName, int preset, const QString &name, int bank ) :
      mFontName(fontName), mPreset(preset), mName(name), mMidiBank(bank), mMidiProgram(preset) {}

    CsSynthSfVoice *voice() const { return mVoice.data(); }

    bool isValid() const { return !mVoice.isNull(); }

    QString name() const { return mName; }

    QString tembrClass() const { return mTembrClass; }

    void jsonWrite( SvJsonWriter &js ) const;

    void jsonRead( SvJsonReader &js );
  };

#endif // CSSYNTHSFVOICEINFO_H
