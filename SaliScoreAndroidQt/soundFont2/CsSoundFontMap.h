#ifndef CSSOUNDFONTMAP_H
#define CSSOUNDFONTMAP_H

#include <QMap>
#include <QSharedPointer>

class CsSoundFont;

using CsSoundFontPtr = QSharedPointer<CsSoundFont>;

class CsSoundFontMap
  {
    QMap<QString,CsSoundFontPtr> mSoundFonts;
    CsSoundFontMap();
  public:
    static CsSoundFontPtr font( const QString fontName );
  };

#endif // CSSOUNDFONTMAP_H
