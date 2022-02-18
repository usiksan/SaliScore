#include "CsPainterSettings.h"

#include "../SvJson/SvJsonIO.h"
#include <QJsonDocument>
#include <QSettings>



CsPainterSettings::CsPainterSettings()
  {
  setDefault();
  }



void CsPainterSettings::setDefault()
  {
  //Defaults
  mFontName            = QStringLiteral("Serif");
  mColorBackground     = QColor(Qt::white);
  mColorRemark         = QColor(Qt::black);
  mColorChord          = QColor(Qt::black);
  mColorChordHighlight = QColor(Qt::blue);
  mColorNote           = QColor(Qt::black);
  mColorNoteHighlight  = QColor(Qt::blue);
  mColorLyric          = QColor(Qt::black);
  mColorLyricHighlight = QColor(Qt::blue);
  mColorTranslation    = QColor(Qt::black);
  mColorTakt           = QColor(Qt::lightGray);
  mPixelPerTakt        = 300;
  mScoreLineDistance   = 8;
  mRemarkFontSize      = 12;
  mChordFontSize       = 16;
  mLyricFontSize       = 14;
  mTranslationFontSize = 12;
  mTitleFontSize       = 20;
  mPropertiesFontSize  = 12;
  mTextGap             = 4;
  mLineGap             = 20;
  mLeftMenuSize        = 10;
  mPlayMarkWidth       = 2;
  mPlayMarkColor       = QColor(Qt::gray);
  mColorGrid           = QColor(Qt::green).lighter();
  mColorCellCurrent    = QColor(Qt::magenta).lighter();
  mColorCellSelected   = QColor(Qt::blue);
  mColorFontSelected   = QColor(Qt::white);
  }



void CsPainterSettings::write(QString key)
  {
  SvJsonWriter js;

  js.jsonString( "fontName", mFontName );
  js.jsonColor( "colorBackground", mColorBackground );
  js.jsonColor( "colorRemark", mColorRemark );
  js.jsonColor( "colorChord", mColorChord );
  js.jsonColor( "colorChordHighlight", mColorChordHighlight );
  js.jsonColor( "colorNote", mColorNote );
  js.jsonColor( "colorNoteHighlight", mColorNoteHighlight );
  js.jsonColor( "colorLyric", mColorLyric );
  js.jsonColor( "colorLyricHighlight", mColorLyricHighlight );
  js.jsonColor( "colorTranslation", mColorTranslation );
  js.jsonColor( "colorTakt", mColorTakt );
  js.jsonInt( "pixelPerTakt", mPixelPerTakt );
  js.jsonInt( "scoreLineDistance", mScoreLineDistance );
  js.jsonInt( "remarkFontSize", mRemarkFontSize );
  js.jsonInt( "chordFontSize", mChordFontSize );
  js.jsonInt( "lyricFontSize", mLyricFontSize );
  js.jsonInt( "translationFontSize", mTranslationFontSize );
  js.jsonInt( "titleFontSize", mTitleFontSize );
  js.jsonInt( "propertiesFontSize", mPropertiesFontSize );
  js.jsonInt( "textGap", mTextGap );
  js.jsonInt( "lineGap", mLineGap );
  js.jsonInt( "leftMenuSize", mLeftMenuSize );
  js.jsonInt( "playMarkWidth", mPlayMarkWidth );
  js.jsonColor( "playMarkColor", mPlayMarkColor );
  js.jsonColor( "colorGrid", mColorGrid );
  js.jsonColor( "colorCellCurrent", mColorCellCurrent );
  js.jsonColor( "colorCellSelected", mColorCellSelected );
  js.jsonColor( "colorFontSelected", mColorFontSelected );

  QSettings s;
  s.setValue( key, QJsonDocument(js.object()).toJson() );
  }




void CsPainterSettings::read(QString key)
  {
  QSettings s;
  if( !s.contains(key) )
    return;
  //Load view settings from json
  QByteArray ar = s.value(key).toByteArray();
  QJsonObject obj = QJsonDocument::fromJson( ar ).object();
  SvJsonReader js( obj );

  js.jsonString( "fontName", mFontName );
  js.jsonColor( "colorBackground", mColorBackground );
  js.jsonColor( "colorRemark", mColorRemark );
  js.jsonColor( "colorChord", mColorChord );
  js.jsonColor( "colorChordHighlight", mColorChordHighlight );
  js.jsonColor( "colorNote", mColorNote );
  js.jsonColor( "colorNoteHighlight", mColorNoteHighlight );
  js.jsonColor( "colorLyric", mColorLyric );
  js.jsonColor( "colorLyricHighlight", mColorLyricHighlight );
  js.jsonColor( "colorTranslation", mColorTranslation );
  js.jsonColor( "colorTakt", mColorTakt );
  js.jsonInt( "pixelPerTakt", mPixelPerTakt );
  js.jsonInt( "scoreLineDistance", mScoreLineDistance );
  js.jsonInt( "remarkFontSize", mRemarkFontSize );
  js.jsonInt( "chordFontSize", mChordFontSize );
  js.jsonInt( "lyricFontSize", mLyricFontSize );
  js.jsonInt( "translationFontSize", mTranslationFontSize );
  js.jsonInt( "titleFontSize", mTitleFontSize );
  js.jsonInt( "propertiesFontSize", mPropertiesFontSize );
  js.jsonInt( "textGap", mTextGap );
  js.jsonInt( "lineGap", mLineGap );
  js.jsonInt( "leftMenuSize", mLeftMenuSize );
  js.jsonInt( "playMarkWidth", mPlayMarkWidth );
  js.jsonColor( "playMarkColor", mPlayMarkColor );
  js.jsonColor( "colorGrid", mColorGrid );
  js.jsonColor( "colorCellCurrent", mColorCellCurrent );
  js.jsonColor( "colorCellSelected", mColorCellSelected );
  js.jsonColor( "colorFontSelected", mColorFontSelected );
  }

