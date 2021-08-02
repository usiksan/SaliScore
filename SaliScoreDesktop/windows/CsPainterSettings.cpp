#include "CsPainterSettings.h"

#include "../SvJson/SvJsonIO.h"
#include <QJsonDocument>
#include <QSettings>

CsPainterSettings::CsPainterSettings()
  {
  //Defaults
  mFontName            = QStringLiteral("Serif");
  mColorBackground     = QColor(Qt::white);
  mPixPerX             = 15;
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
  }



void CsPainterSettings::write(QString key)
  {
  SvJsonWriter js;

  js.jsonString( "fontName", mFontName );
  js.jsonColor( "colorBackground", mColorBackground );
  js.jsonInt( "pixPerX", mPixPerX );
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
  js.jsonInt( "pixPerX", mPixPerX );
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
  }

