#include "CsConfig.h"
#include "CsSynthSfManager.h"
#include "CsSynthSfVoice.h"
#include "soundFont2/CsSoundFont.h"

#include <QProgressDialog>
#include <QFileInfo>
#include <QDir>

CsSynthSfManager::CsSynthSfManager(QObject *parent)
  : QObject{parent}
  {
  for( int i = 0; i < 16; i++ )
    mChannels[i] = -1;
  }



void CsSynthSfManager::build(QWidget *parent, const QString soundFontPath)
  {
  QProgressDialog progress( tr("Sound font loading"), tr("Cancel"), 0, 100, parent );
  progress.setWindowModality( Qt::WindowModal );

  QFileInfo info(soundFontPath);
  QString soundFontName = info.fileName();
  progress.setValue( 1 );

  CsSoundFont soundFont;
  soundFont.read( soundFontPath );
  int presetCount = soundFont.presetCount();
  progress.setMaximum( presetCount + 10 );
  progress.setValue( 10 );
  QList<CsSynthSfVoiceInfo> voiceList;
  for( int i = 0; i < presetCount; i++ ) {
    voiceList.append( CsSynthSfVoiceInfo( soundFontName, i, soundFont.presetName(i), soundFont.presetBank(i) ) );
    progress.setValue( 10 + i );
    }

  //Save list
  QString infoFileName = info.dir().absoluteFilePath( CS_VOICE_LIST );
  QFile file(infoFileName);
  if( file.open( QIODevice::WriteOnly ) ) {
    SvJsonWriter js;
    js.jsonList( "voiceList", voiceList );
    file.write( svJsonObjectToByteArray( js.object() ) );
    }

  progress.setValue( 10 + presetCount );
  }



void CsSynthSfManager::playNote(int channel, int pitch, int velo)
  {
  if( mChannels[channel] >= 0 )
    mInfoList.at( mChannels[channel] ).voice()->playNote( pitch, velo );
  }




void CsSynthSfManager::voiceSelect(int channel, int voiceId)
  {
  mChannels[channel] = mVoiceIdMap.contains(voiceId) ? mVoiceIdMap.value(voiceId) : -1;
  }
