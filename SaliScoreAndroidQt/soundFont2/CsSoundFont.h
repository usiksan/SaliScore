/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   SoundFont2 file
*/
#ifndef CSSOUNDFONT_H
#define CSSOUNDFONT_H

#include "iff/IffReader.h"
#include <QVector>
#include <functional>
#include <QWeakPointer>


enum SfGeneratorParamId {
  sfpiStartAddressOffset = 0,
  sfpiEndAddressOffset = 1,
  sfpiStartLoopAddressOffset = 2,
  sfpiEndLoopAddressOffset = 3,
  sfpiStartAddressCoarseOffset = 4,
  sfpiModLfoToPitch = 5,
  sfpiVibLfoToPitch = 6,
  sfpiModEnvToPitch = 7,
  sfpiInitialFilterFc = 8,
  sfpiInitialFilterQ = 9,
  sfpiModLfoToFilterFc = 10,
  sfpiModEnvToFilterFc = 11,
  sfpiEndAddressCoarseOffset = 12,
  sfpiModLfoToVolume = 13,
  sfpiChorusEffectsSend = 15,
  sfpiReverbEffectsSend = 16,
  sfpiPan = 17,
  sfpiDelayModLfo = 21,
  sfpiFreqModLfo = 22,
  sfpiDelayVibLfo = 23,
  sfpiFreqVibLfo = 24,
  sfpiDelayModEnv = 25,
  sfpiAttackModEnv = 26,
  sfpiHoldModEnv = 27,
  sfpiDecayModEnv = 28,
  sfpiSustainModEnv = 29,
  sfpiReleaseModEnv = 30,
  sfpiKeyNumToModEnvHold = 31,
  sfpiKeyNumToModEnvDecay = 32,
  sfpiDelayVolEnv = 33,
  sfpiAttackVolEnv = 34,
  sfpiHoldVolEnv = 35,
  sfpiDecayVolEnv = 36,
  sfpiSustainVolEnv = 37,
  sfpiReleaseVolEnv = 38,
  sfpiKeyNumToVolEnvHold = 39,
  sfpiKeyNumToVolEnvDecay = 40,
  sfpiInstrument = 41,
  sfpiKeyRange = 43,
  sfpiVelRange = 44,
  sfpiStartLoopCoarseOffset = 45,
  sfpiKeyNum = 46,
  sfpiVelocity = 47,
  sfpiInitialAttenuation = 48,
  sfpiEndLoopCoarseOffset = 50,
  sfpiCoarseTune = 51,
  sfpiFineTune = 52,
  sfpiSampleId = 53,
  sfpiSampleModes = 54,
  sfpiScaleTuning = 56,
  sfpiExclusiveClass = 57,
  sfpiOverridingRootKey = 58
};



struct SfSample
  {
    char    achSampleName[21];
    quint32 dwStart;
    quint32 dwEnd;
    quint32 dwStartloop;
    quint32 dwEndloop;
    quint32 dwSampleRate;
    quint8  byOriginalPitch;
    qint8   chPitchCorrection;
    quint16 wSampleLink;
    quint16 sfSampleType;
  };

using SfSampleVector = QVector<SfSample>;




struct SfPreset
  {
    char    achPresetName[21]; //Preset name
    quint16 wPreset;           //Unique preset and bank combination
    quint16 wBank;
    quint16 wPresetBagIndex;
    //Reserved for feature
    quint32 dwLibrary;
    quint32 dwGentre;
    quint32 dwMorphology;
  };

using SfPresetVector = QVector<SfPreset>;




struct SfBag
  {
    quint16 mGeneratorStartIndex; //Start index in generator param vector
    quint16 mModulatorStartIndex; //Start index in modulator param vector
  };

using SfBagVector = QVector<SfBag>;



//One generator parameter description
struct SfGeneratorParam
  {
    quint16 mParamIndex; //Param index - identify param
    quint16 mParamValue; //Param value

    quint16 getUint16() const { return mParamValue; }
    qint16  getInt16() const { return static_cast<qint16>(mParamValue); }
    quint8  getMB() const { return static_cast<quint8>(mParamValue >> 8); }
    quint8  getLB() const { return static_cast<quint8>(mParamValue & 0xff); }
  };

using SfGeneratorParamVector = QVector<SfGeneratorParam>;


//Modulator description
struct SfModulator
  {
    quint16 mModSrcOper;    //Source data of modulator
    quint16 mModDstOper;    //Destignation data of modulator
    qint16  mModAmount;     //Indicating the degree to which the source modulates the destination. A zero value indicates there is no fixed amount.
    quint16 mModAmtSrcOper; //This value indicates the degree to which the source modulates the destination is to be controlled by the specified modulation source.
    quint16 mModTransOper;  //This value indicates that a transform of the specified type will be applied to the modulation source before application to the modulator.
  };

using SfModulatorVector = QVector<SfModulator>;


//Instrument descriptor
struct SfInstrument
  {
    char    achInstrumentName[21]; //Instrument name
    quint16 wInstumentBagIndex;    //Instrument first zone
    quint16 mLastBagIndex;         //Instrument last zone
  };

using SfInstrumentVector = QVector<SfInstrument>;



class CsSoundFont
  {
    QString                mFileName;
    QVector<qint16>        mSamples;
    SfSampleVector         mSampleHeaders;
    SfPresetVector         mPresets;
    SfBagVector            mPresetBags;
    SfModulatorVector      mPresetModulators;
    SfGeneratorParamVector mPresetGenerators;
    SfInstrumentVector     mInstruments;
    SfBagVector            mInstrumentBags;
    SfModulatorVector      mInstrumentModulators;
    SfGeneratorParamVector mInstrumentGenerators;
  public:
    CsSoundFont();
    ~CsSoundFont();

    void        clear();

    QString     fileName() const { return mFileName; }

    //Preset name list
    QStringList presetList() const;

    //Preset count in sound font
    int         presetCount() const { return mPresets.count(); }

    //Preset name by index
    QString     presetName( int preset ) const;

    bool        read( const QString fname );

    bool        buildPreset( int preset, std::function<void( quint16 *generator, const SfSample &sam, qint16 *samples )> addTracks );

  private:
    bool    buildInstrument( quint16 *generator, std::function<void( quint16 *generator, const SfSample &sam, qint16 *samples )> addTracks );

    bool readRiff( IffReader &reader );

    bool readSfbk( IffReader &reader );

    bool readInfo( IffReader &reader );

    bool readSdta( IffReader &reader );

    bool readPdta( IffReader &reader );

    bool readSmpl( IffReader &reader );

    bool readShdr( IffReader &reader );

    bool readInst( IffReader &reader );

    bool readIbag( IffReader &reader );

    bool readImod( IffReader &reader );

    bool readIgen( IffReader &reader );

    bool readPhdr( IffReader &reader );

    bool readPbag( IffReader &reader );

    bool readPgen( IffReader &reader );

    bool readPmod( IffReader &reader );
  };

using SoundFontPtr = QSharedPointer<CsSoundFont>;
using SoundFontWeakPtr = QWeakPointer<CsSoundFont>;

#endif // CSSOUNDFONT_H
