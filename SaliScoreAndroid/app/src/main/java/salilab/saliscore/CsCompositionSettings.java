package salilab.saliscore;

import java.util.ArrayList;

public class CsCompositionSettings extends CsCompositionHeader {
  //Visible list is list of part which are visible on screen. Others parts are hide
  private ArrayList<String> mRemarkList;      //!< Visible remark list
  private ArrayList<String> mChordList;       //!< Visible chord list
  private ArrayList<String> mNoteList;        //!< Visible note list
  private ArrayList<String> mTranslationList; //!< Visible translation list

  CsCompositionSettings() {
    super();
    mRemarkList = new ArrayList<>();
    mChordList  = new ArrayList<>();
    mNoteList   = new ArrayList<>();
    mTranslationList = new ArrayList<>();
  }

//  CsCompositionSettings( CsComposition comp ) {
//
//  }

  ArrayList<String> remarkList() { return mRemarkList; }

  ArrayList<String> chordList()  { return mChordList; }

  ArrayList<String> noteList()   { return mNoteList; }

  ArrayList<String> translationList() { return mTranslationList; }


  //=================================================================
  //         Settings JSON io

  @Override
  void jsonWrite( SvJsonWriter js ) throws Exception {
    super.jsonWrite( js );
    js.jsonListString( "remarkVisible", mRemarkList );
    js.jsonListString( "chordVisible", mChordList );
    js.jsonListString( "noteVisible", mNoteList );
    js.jsonListString( "translationVisible", mTranslationList );
  }

  @Override
  void jsonRead( SvJsonReader js ) throws Exception {
    super.jsonRead( js );
    js.jsonListString( "remarkVisible", mRemarkList );
    js.jsonListString( "chordVisible", mChordList );
    js.jsonListString( "noteVisible", mNoteList );
    js.jsonListString( "translationVisible", mTranslationList );
  }

}
