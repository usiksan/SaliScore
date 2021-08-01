#ifndef CSPAINTSTYLE_H
#define CSPAINTSTYLE_H

#include <QString>

class CsPaintStyle
  {
  public:
    QString mFontName;
    int     mRemarkFontSize;

    CsPaintStyle();

    QString fontName() const { return mFontName; }
  };

#endif // CSPAINTSTYLE_H
