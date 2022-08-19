#include "CsConfig.h"
#include "CsSynthSfManager.h"
#include "CsSynthSfVoice.h"
#include "soundFont2/CsSoundFont.h"
#include "score/CsSongLocalRepo.h"

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
    voiceList.append( CsSynthSfVoiceInfo( soundFontName, i, soundFont.presetName(i), soundFont.presetBank(i), i & 0x7f ) );
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




void CsSynthSfManager::load(QWidget *parent)
  {
  QFile file( CsSongLocalRepo::repo()->repoHomeDir( CS_DIR_SOUND_FONT ) + CS_VOICE_LIST );
  if( file.open( QIODevice::ReadOnly ) ) {
    QProgressDialog progress( tr("Sound font loading"), tr("Cancel"), 0, 3, parent );
    progress.setWindowModality( Qt::WindowModal );

    progress.setValue( 1 );
    auto obj = svJsonObjectFromByteArray(file.readAll());
    SvJsonReader js( obj );
    js.jsonList( "voiceList", mInfoList );
    progress.setValue( 2 );

    for( int i = 0; i < 16; i++ )
      mChannels[i] = -1;

    //Fill map
    mVoiceIdMap.clear();
    mVoiceMap.clear();
    for( int i = 0; i < mInfoList.count() - 1; i++ ) {
      mVoiceIdMap.insert( mInfoList.at(i).voiceId(), i );
      mVoiceMap.insert( mInfoList.at(i).name(), i );
      }

    if( mInfoList.count() )
      voiceSelect( 0, mInfoList.at(0).voiceId() );

    progress.setValue( 3 );
    }
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
