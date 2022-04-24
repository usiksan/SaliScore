#include "CsConfig.h"
#include "audioOut/CsSoundBufferIODevice.h"
#include "synth/CsSynthMeandr.h"
#include "score/CsPlayList.h"
#include "repo/CsRepoClient.h"
#include "visual/CsDlgRegistration.h"

#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QThread>
#include <QDebug>


#ifdef Q_OS_ANDROID
#include "visual/CsAndroidWinMain.h"
#else
#include "visual/CsDesktopWinMain.h"
#endif

int main(int argc, char *argv[])
  {
  //Setups for settings
  QCoreApplication::setOrganizationName(CS_ORGANIZATION_NAME);
  QCoreApplication::setOrganizationDomain(CS_ORGANIZATION_DOMAIN);
  QCoreApplication::setApplicationName(CS_APPLICATION_NAME);


  QApplication a(argc, argv);

  QSettings s;
  if( !s.contains(KEY_LANGUAGE) ) {
    //Language is not assigned yet, assign it

    //Get system language
    QStringList ui = QLocale::system().uiLanguages();
    QString lang;
    if( ui.count() )
      lang = ui.at(0);
    else
      lang = QStringLiteral("en");

    //Assign default language
    s.setValue( KEY_LANGUAGE, lang );
    }


  //Translation system
  QTranslator appTranslator;
  if( appTranslator.load( QString( QCoreApplication::applicationDirPath() + "/lang_%1.qm").arg( s.value( KEY_LANGUAGE ).toString() ) ) )
    a.installTranslator( &appTranslator );


  //Main play list
  CsPlayList::pl()->load();

  //Local repo
  CsSongLocalRepo::repo()->repoInit();

  //Remote repo client
  repoClient = new CsRepoClient();

//  if( !repoClient->isRegistered() ) {
//    CsDlgRegistration dlg(nullptr);
//    if( !dlg.exec() )
//      return 0;
//    }

  //=============================================================================
  //        Audio output setup
  if( CsSoundBufferIODevice::soundBuffer() == nullptr )
    return 0;
  CsSoundBufferIODevice::soundBuffer()->addSound( new CsSynthMeandr() );




#ifdef Q_OS_ANDROID
  CsAndroidWinMain w();
#else

  //=============================================================================
  //        MIDI keyboard setup
  //Start midi keyboard
//  QThread *midiThread = new QThread();
//  CsMidiSequencer *midiSequencer = new CsMidiSequencer( midiThread );

  CsDesktopWinMain w;
#endif



  w.show();
  int res = a.exec();

#ifndef Q_OS_ANDROID
//  midiThread->start();
//  int res = a.exec();

//  midiThread->quit();
//  midiThread->wait();

  CsPlayList::pl()->save();
#endif

  return res;
  }
