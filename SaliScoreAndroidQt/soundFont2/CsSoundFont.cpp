/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "CsSoundFont.h"

#include <QDebug>

CsSoundFont::CsSoundFont()
  {

  }

CsSoundFont::~CsSoundFont()
  {

  }



void CsSoundFont::clear()
  {
  //qDeleteAll(mSamples);
  mSamples.clear();
  mSampleHeaders.clear();
  }




//Preset name list
QStringList CsSoundFont::presetList() const
  {
  //Prepare list
  QStringList list;

  //Fill preset name list from preset list
  //Exclude last preset because it is unused
  for( int i = 0; i < mPresets.count() - 1; i++ )
    list.append( QString::fromLatin1( mPresets.at(i).achPresetName ) );

  //Return preset name list
  return list;
  }




//Preset name by index
QString CsSoundFont::presetName(int preset) const
  {
  if( preset >= 0 && preset < mPresets.count() )
    return QString::fromLatin1( mPresets.at(preset).achPresetName );
  return QString{};
  }



bool CsSoundFont::read(const QString fname)
  {
  IffReader reader(fname,true);
  if( !reader.isValid() )
    return false;

  if( reader.readChunk( "RIFF", [this] ( IffReader &reader ) -> bool { return readRiff(reader); } ) ) {
    mFileName = fname;
    return true;
    }
  return false;
  }




bool CsSoundFont::buildPreset(int preset, std::function<void( quint16 *generator, const SfSample &sam, qint16 *samples )> addTracks )
  {
  //Create default generator
  quint16 generator[61];
  for( int i = 0; i < 61; i++ )
    generator[i] = 0;
  generator[sfpiInitialFilterFc] = 13500;
  generator[sfpiDelayModLfo] = static_cast<quint16>(-12000);
  generator[sfpiDelayVibLfo] = static_cast<quint16>(-12000);

  generator[sfpiDelayModEnv] = static_cast<quint16>(-12000);
  generator[sfpiAttackModEnv] = static_cast<quint16>(-12000);
  generator[sfpiHoldModEnv] = static_cast<quint16>(-12000);
  generator[sfpiDecayModEnv] = static_cast<quint16>(-12000);
  generator[sfpiReleaseModEnv] = static_cast<quint16>(-12000);

  generator[sfpiDelayVolEnv] = static_cast<quint16>(-12000);
  generator[sfpiAttackVolEnv] = static_cast<quint16>(-12000);
  generator[sfpiHoldVolEnv] = static_cast<quint16>(-12000);
  generator[sfpiDecayVolEnv] = static_cast<quint16>(-12000);
  generator[sfpiReleaseVolEnv] = static_cast<quint16>(-12000);

  generator[sfpiKeyRange] = 127 << 8;
  generator[sfpiVelRange] = 127 << 8;
  generator[sfpiKeyNum] = static_cast<quint16>(-1);
  generator[sfpiVelocity] = static_cast<quint16>(-1);

  generator[sfpiScaleTuning] = static_cast<quint16>(100);
  generator[sfpiOverridingRootKey] = static_cast<quint16>(-1);

  //Check available preset
  if( preset < 0 || preset >= mPresets.count() - 1 )
    return false;

  int startBag = mPresets.at(preset).wPresetBagIndex;
  int endBag   = mPresets.at(preset + 1).wPresetBagIndex;
  //qDebug() << "selected" << preset << "preset";
  //qDebug() << mPresets.at(preset).achPresetName << mPresets.at(preset).wBank << mPresets.at(preset).wPreset;
  //qDebug() << "Bags:" << startBag << endBag;
  bool globalZone = true;
  while( startBag < endBag ) {
    int startGen = mPresetBags.at(startBag).mGeneratorStartIndex;
    int endGen = mPresetBags.at(startBag + 1).mGeneratorStartIndex;
    //qDebug() << startGen << endGen << mPresetBags.at(startBag).mModulatorStartIndex;

    quint16 zoneGenerator[61];
    memcpy( zoneGenerator, generator, sizeof(quint16) * 61 );

    while( startGen < endGen ) {
      int paramIndex = mPresetGenerators.at(startGen).mParamIndex;
      if( paramIndex < 0 || paramIndex >= 61 ) return false;
      //qDebug() << "param" << paramIndex << mPresetGenerators.at(startGen).mParamValue;
      zoneGenerator[paramIndex] = mPresetGenerators.at(startGen).mParamValue;
      if( paramIndex == sfpiInstrument ) globalZone = false;
      startGen++;
      }

    if( globalZone )
      memcpy( generator, zoneGenerator, sizeof(quint16) * 61 );
    else
      buildInstrument( zoneGenerator, addTracks );
    startBag++;
    }
  return true;
  }




bool CsSoundFont::buildInstrument(quint16 *generator, std::function<void( quint16 *generator, const SfSample &sam, qint16 *samples )> addTracks )
  {
  int instrument = generator[sfpiInstrument];
  //qDebug() << "selected instrument" << instrument;
  if( instrument < 0 || instrument >= mInstruments.count() - 1 )
    return false;

  int startBag = mInstruments.at(instrument).wInstumentBagIndex;
  int endBag   = mInstruments.at(instrument + 1).wInstumentBagIndex;
  //qDebug() << "instrument name" << mInstruments.at(instrument).achInstrumentName << startBag << endBag;

  //qDebug() << "Bags:";
  bool globalZone = true;
  while( startBag < endBag ) {
    int startGen = mInstrumentBags.at(startBag).mGeneratorStartIndex;
    int endGen = mInstrumentBags.at(startBag + 1).mGeneratorStartIndex;
    //qDebug() << startGen << endGen << mInstrumentBags.at(startBag).mModulatorStartIndex;

    quint16 zoneGenerator[61];
    memcpy( zoneGenerator, generator, sizeof(quint16) * 61 );

    while( startGen < endGen ) {
      int paramIndex = mInstrumentGenerators.at(startGen).mParamIndex;
      if( paramIndex < 0 || paramIndex >= 61 ) return false;
      //qDebug() << "param" << paramIndex << mInstrumentGenerators.at(startGen).mParamValue;
      zoneGenerator[paramIndex] = mInstrumentGenerators.at(startGen).mParamValue;
      if( paramIndex == sfpiSampleId ) globalZone = false;
      startGen++;
      }

    if( globalZone )
      memcpy( generator, zoneGenerator, sizeof(quint16) * 61 );
    else {
      int sampleId = zoneGenerator[sfpiSampleId];
      if( sampleId >= mSampleHeaders.count() - 1 )
        return false;
      addTracks( zoneGenerator, mSampleHeaders.at(sampleId), mSamples.data() );
      }
    startBag++;
    }
  return true;
  }









bool CsSoundFont::readRiff(IffReader &reader)
  {
  return reader.needFour( "sfbk" ) && readSfbk(reader);
  }



bool CsSoundFont::readSfbk(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    IffReader chunk = reader.getChunk();
    if( !chunk.compareChunkName("LIST") )
      return false;

    switch( chunk.getUint32be() ) {
      case IFF_FOUR('I','N','F','O') :
        if( !readInfo( chunk ) ) return false;
        break;
      case IFF_FOUR('s','d','t','a') :
        if( !readSdta( chunk ) ) return false;
        break;
      case IFF_FOUR('p','d','t','a') :
        if( !readPdta( chunk ) ) return false;
        break;
      }
    }
  return true;
  }



bool CsSoundFont::readInfo(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    IffReader chunk = reader.getChunk();
    //qDebug() << "INFO" << chunk.name();
    switch( chunk.chunkFour() ) {
//      case IFF_FOUR('i','f','i','l') :
//        //Version
//        qDebug() << "ifil" << chunk.getUint16le() << '.' << chunk.getUint16le();
//        break;
//      case IFF_FOUR('i','s','n','g') :
//        qDebug() << "isng";
//        break;
//      case IFF_FOUR('I','N','A','M') :
//        qDebug() << "INAM";
//        break;
//      case IFF_FOUR('i','r','o','m') :
//        qDebug() << "INAM";
//        break;
//      case IFF_FOUR('i','v','e','r') :
//        break;
//      case IFF_FOUR('I','C','R','D') :
//        break;
//      case IFF_FOUR('I','E','N','G') :
//        break;
//      case IFF_FOUR('I','P','R','D') :
//        break;
//      case IFF_FOUR('I','C','O','P') :
//        break;
//      case IFF_FOUR('I','C','M','T') :
//        break;
//      case IFF_FOUR('I','S','F','T') :
//        break;
      }
    }
  return true;
  }



bool CsSoundFont::readSdta(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    IffReader chunk = reader.getChunk();
    //qDebug() << "sdta" << chunk.name();
    switch( chunk.chunkFour() ) {
      case IFF_FOUR('s','m','p','l') :
        if( !readSmpl(chunk) ) return false;
        break;
//      case IFF_FOUR('s','m','2','4') :
//        break;
      }
    }
  return true;
  }


bool CsSoundFont::readPdta(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    IffReader chunk = reader.getChunk();
    //qDebug() << "pdta" << chunk.name();
    switch( chunk.chunkFour() ) {
      case IFF_FOUR('p','h','d','r') :
        if( !readPhdr(chunk) ) return false;
        break;
      case IFF_FOUR('p','b','a','g') :
        if( !readPbag(chunk) ) return false;
        break;
      case IFF_FOUR('p','m','o','d') :
        if( !readPmod(chunk) ) return false;
        break;
      case IFF_FOUR('p','g','e','n') :
        if( !readPgen( chunk ) ) return false;
        break;
      case IFF_FOUR('i','n','s','t') :
        if( !readInst( chunk ) ) return false;
        break;
      case IFF_FOUR('i','b','a','g') :
        if( !readIbag( chunk ) ) return false;
        break;
      case IFF_FOUR('i','m','o','d') :
        if( !readImod( chunk ) ) return false;
        break;
      case IFF_FOUR('i','g','e','n') :
        if( !readIgen( chunk ) ) return false;
        break;
      case IFF_FOUR('s','h','d','r') :
        if( !readShdr( chunk ) ) return false;
        break;
      }
    }
  return true;
  }



bool CsSoundFont::readSmpl(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    qint16 sample = reader.getInt16le();
    mSamples.append(sample);
    }
  return true;
  }



bool CsSoundFont::readShdr(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfSample sm;
    reader.readChars( sm.achSampleName, 20 );
    sm.achSampleName[20] = 0;
    sm.dwStart           = reader.getUint32le();
    sm.dwEnd             = reader.getUint32le();
    sm.dwStartloop       = reader.getUint32le();
    sm.dwEndloop         = reader.getUint32le();
    sm.dwSampleRate      = reader.getUint32le();
    sm.byOriginalPitch   = reader.getUint8();
    sm.chPitchCorrection = reader.getInt8();
    sm.wSampleLink       = reader.getUint16le();
    sm.sfSampleType      = reader.getUint16le();
    //qDebug() << "header" << sm.achSampleName;
    mSampleHeaders.append( sm );
    }
  return true;
  }




bool CsSoundFont::readInst(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfInstrument inst;
    reader.readChars( inst.achInstrumentName, 20 );
    inst.achInstrumentName[20] = 0;
    inst.wInstumentBagIndex = reader.getUint16le();
    //qDebug() << "inst" << inst.achInstrumentName << inst.wInstumentBagIndex;
    if( mInstruments.count() )
      mInstruments.last().mLastBagIndex = inst.wInstumentBagIndex - 1;
    mInstruments.append( inst );
    }
  return true;
  }




bool CsSoundFont::readIbag(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfBag bag;
    bag.mGeneratorStartIndex = reader.getUint16le();
    bag.mModulatorStartIndex = reader.getUint16le();
    //qDebug() << "bag" << bag.mGeneratorStartIndex << bag.mModulatorStartIndex;
    mInstrumentBags.append( bag );
    }
  return true;
  }



bool CsSoundFont::readImod(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfModulator mod;
    mod.mModSrcOper = reader.getUint16le();
    mod.mModDstOper = reader.getUint16le();
    mod.mModAmount = reader.getInt16le();
    mod.mModAmtSrcOper = reader.getUint16le();
    mod.mModTransOper = reader.getUint16le();
    //qDebug() << "mod" << mod.mModSrcOper << mod.mModDstOper << mod.mModAmount << mod.mModAmtSrcOper << mod.mModTransOper;
    mInstrumentModulators.append( mod );
    }
  return true;
  }




bool CsSoundFont::readIgen(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfGeneratorParam gen;
    gen.mParamIndex = reader.getUint16le();
    gen.mParamValue = reader.getUint16le();
    //qDebug() << "gen" << gen.mParamIndex << gen.mParamValue;
    mInstrumentGenerators.append( gen );
    }
  return true;
  }




bool CsSoundFont::readPhdr(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfPreset hdr;
    reader.readChars( hdr.achPresetName, 20 );
    hdr.achPresetName[20] = 0;
    hdr.wPreset         = reader.getUint16le();           //Unique preset and bank combination
    hdr.wBank           = reader.getUint16le();
    hdr.wPresetBagIndex = reader.getUint16le();
    //Reserved for feature
    hdr.dwLibrary      = reader.getUint32le();
    hdr.dwGentre       = reader.getUint32le();
    hdr.dwMorphology   = reader.getUint32le();
    //qDebug() << "Preset" << hdr.achPresetName << hdr.wPreset << hdr.wBank << hdr.wPresetBagIndex;
    mPresets.append( hdr );
    }
  return true;
  }



bool CsSoundFont::readPbag(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfBag bag;
    bag.mGeneratorStartIndex = reader.getUint16le();
    bag.mModulatorStartIndex = reader.getUint16le();
    //qDebug() << "bag" << bag.mGeneratorStartIndex << bag.mModulatorStartIndex;
    mPresetBags.append( bag );
    }
  return true;
  }




bool CsSoundFont::readPgen(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfGeneratorParam gen;
    gen.mParamIndex = reader.getUint16le();
    gen.mParamValue = reader.getUint16le();
    //qDebug() << "gen" << gen.mParamIndex << gen.mParamValue;
    mPresetGenerators.append( gen );
    }
  return true;
  }




bool CsSoundFont::readPmod(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfModulator mod;
    mod.mModSrcOper = reader.getUint16le();
    mod.mModDstOper = reader.getUint16le();
    mod.mModAmount = reader.getInt16le();
    mod.mModAmtSrcOper = reader.getUint16le();
    mod.mModTransOper = reader.getUint16le();
    //qDebug() << "mod" << mod.mModSrcOper << mod.mModDstOper << mod.mModAmount << mod.mModAmtSrcOper << mod.mModTransOper;
    mPresetModulators.append( mod );
    }
  return true;
  }


