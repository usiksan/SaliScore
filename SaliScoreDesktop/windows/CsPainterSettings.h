#ifndef CSPAINTERSETTINGS_H
#define CSPAINTERSETTINGS_H

#include "CsConfig.h"

#include <QString>
#include <QColor>

struct CsPainterSettings
  {
    QString     mFontName;
    QColor      mColorBackground;
    int         mPixPerX;
    int         mScoreLineDistance;
    int         mRemarkFontSize;
    int         mChordFontSize;
    int         mLyricFontSize;
    int         mTranslationFontSize;
    int         mTextGap;
    int         mLineGap;

    CsPainterSettings();

    void write( QString key );

    void read( QString key );
  };

#endif // CSPAINTERSETTINGS_H
