package salilab.saliscore;

public class CsCompositionHeader extends CsDescrSong {
  public CsSynthSettings mSettings;
  public CsTrainInterval mFragments[];

  CsCompositionHeader() {
    super();
    mSettings = new CsSynthSettings();
    mFragments = new CsTrainInterval[2];
    mFragments[0] = new CsTrainInterval();
    mFragments[1] = new CsTrainInterval();
  }

  CsCompositionHeader( CsCompositionHeader hdr ) {
    super( hdr );
    mSettings = new CsSynthSettings(hdr.mSettings);
    mFragments = new CsTrainInterval[2];
    mFragments[0] = new CsTrainInterval(hdr.mFragments[0]);
    mFragments[1] = new CsTrainInterval(hdr.mFragments[1]);
  }

  @Override
  void clear() {
    super.clear();
    mSettings.clear();
  }

  @Override
  void jsonWrite( SvJsonWriter js ) throws Exception {
    super.jsonWrite( js );
    mSettings.jsonWrite( js );
    js.jsonObject( "fragment0", mFragments[0] );
    js.jsonObject( "fragment1", mFragments[1] );
  }

  @Override
  void jsonRead( SvJsonReader js ) throws Exception {
    super.jsonRead( js );
    mSettings.jsonRead( js );
    js.jsonObject( "fragment0", mFragments[0] );
    js.jsonObject( "fragment1", mFragments[1] );
  }

  @Override
  CsJsonWritable copy() {
    return new CsCompositionHeader( this );
  }

  }
