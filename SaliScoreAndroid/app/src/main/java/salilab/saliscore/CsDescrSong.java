package salilab.saliscore;

import android.content.Context;
import android.os.Environment;

import org.json.JSONObject;

public class CsDescrSong extends CsJsonWritable {
  private String  mAuthor;         //!< Author who creates composition score
  private String  mSinger;         //!< Singer is common singer of composition
  private String  mName;           //!< Composition name
  private Integer mAuthorSongId;   //!< Unical id of composition for this author
  private Integer mVersion;        //!< Version of composition score (time of last edit)
  private Boolean mIsPublic;       //!< When true, then author make it available for public, otherway its private for author

  CsDescrSong() {
    mAuthorSongId = 0;
    mVersion = 0;
    mIsPublic = false;
  }

  CsDescrSong( CsDescrSong src ) {
    mAuthor = src.mAuthor;
    mSinger = src.mSinger;
    mName   = src.mName;
    mAuthorSongId = src.mAuthorSongId;
    mVersion      = src.mVersion;
    mIsPublic     = src.mIsPublic;
  }

  String songId() { return mAuthor + "_" + Integer.toString( mAuthorSongId, 16 ); }

  String directory() {
    String s = Integer.toString(mAuthorSongId,32);
    return s.substring( s.length() - 1 );
  }

  String fileName() {
    return songId() + ".saliscore";
  }


  String  author() { return mAuthor; }
  void    authorSet( String auth ) {
    mAuthor = auth;
    mAuthorSongId = mVersion = SdTime2x.current();
  }


  String  singer() { return mSinger; }
  void    singerSet( String sing ) { mSinger = sing; }

  String  name() { return mName; }
  void    nameSet( String nam ) { mName = nam; }

  int     version() { return mVersion; }
  void    versionUpdate() {
    mVersion = SdTime2x.current();
  }

  boolean isPublic() { return mIsPublic; }
  void    isPublicSet( boolean pub ) { mIsPublic = pub; }

  int     versionFromFile( Context ctx ) {
    JSONObject obj = SvJsonFile.read( ctx, fileName() );
    //int version = obj.value(QStringLiteral(CS_BASE_VERSION_KEY)).toInt();
    //"BaseVersion"
    SvJsonReader js = new SvJsonReader( obj );
    CsDescrSong comp = new CsDescrSong();
    try {
      comp.jsonRead( js );
    } catch (Exception e) {
    }
    return comp.version();
  }


  void clear() {
    mAuthor = new String();
    mSinger = new String();
    mName   = new String();
    mAuthorSongId = mVersion = 0;
    mIsPublic = false;
  }

  //=================================================================
  //         Settings JSON io

  @Override
  void   jsonWrite( SvJsonWriter js ) throws Exception {
    js.jsonString( "Author", mAuthor );
    js.jsonString( "Singer", mSinger );
    js.jsonString( "Name",   mName );
    js.jsonInt( "AuthorSongId", mAuthorSongId );
    js.jsonInt( "Version",      mVersion );
    js.jsonBool( "IsPublic",    mIsPublic );
  }

  @Override
  void   jsonRead( SvJsonReader js ) throws Exception {
    js.jsonString( "Author", mAuthor );
    js.jsonString( "Singer", mSinger );
    js.jsonString( "Name",   mName );
    js.jsonInt( "AuthorSongId", mAuthorSongId );
    js.jsonInt( "Version",      mVersion );
    js.jsonBool( "IsPublic",    mIsPublic );
  }

  @Override
  CsJsonWritable copy() {
    return new CsDescrSong( this );
  }
}
