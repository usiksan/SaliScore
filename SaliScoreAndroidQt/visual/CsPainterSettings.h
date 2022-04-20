#ifndef CSPAINTERSETTINGS_H
#define CSPAINTERSETTINGS_H

#include "config.h"

#include <QString>
#include <QColor>

struct CsPainterSettings
  {
    QString     mFontName;

    QColor      mColorBackground;
    QColor      mColorRemark;
    QColor      mColorChord;
    QColor      mColorChordHighlight;
    QColor      mColorNote;
    QColor      mColorNoteHighlight;
    QColor      mColorLyric;
    QColor      mColorLyricHighlight;
    QColor      mColorTranslation;
    QColor      mColorTakt;

    int         mPixelPerTakt;

    int         mScoreLineDistance;

    int         mRemarkFontSize;
    int         mChordFontSize;
    int         mLyricFontSize;
    int         mTranslationFontSize;
    int         mTitleFontSize;
    int         mPropertiesFontSize;

    int         mTextGap;
    int         mLineGap;
    int         mLeftMenuSize;

    int         mPlayMarkWidth;
    QColor      mPlayMarkColor;

    QColor      mColorGrid;
    QColor      mColorCellCurrent;
    QColor      mColorCellSelected;
    QColor      mColorFontSelected;
    CsPainterSettings();

    void setDefault();

    void write( QString key );

    void read( QString key );
  };

#endif // CSPAINTERSETTINGS_H
