#include "CsConfig.h"
#include "CsSoundFontMap.h"
#include "CsSoundFont.h"
#include "score/CsSongLocalRepo.h"

#include <QFile>

CsSoundFontMap::CsSoundFontMap()
  {

  }

CsSoundFontPtr CsSoundFontMap::font(const QString fontName)
  {
  //Local storage for readed sound fonts
  static CsSoundFontMap soundFontMap;

  if( fontName.isEmpty() )
    return CsSoundFontPtr{};

  //If sound font already readed, then simple return it
  if( soundFontMap.mSoundFonts.contains(fontName) )
    return soundFontMap.mSoundFonts.value(fontName);

  //try load font
  QString fontFilePath = CsSongLocalRepo::repo()->repoHomeDir( CS_DIR_SOUND_FONT ) + fontName;
  if( !QFile::exists( fontFilePath ) )
    return CsSoundFontPtr{};

  CsSoundFontPtr soundFontPtr( new CsSoundFont{} );
  if( soundFontPtr->read(fontFilePath) ) {
    //Sound font readed successfully
    soundFontMap.mSoundFonts.insert( fontName, soundFontPtr );
    return soundFontPtr;
    }

  return CsSoundFontPtr{};
  }
