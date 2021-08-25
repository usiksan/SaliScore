#include "CsConfig.h"
#include "midi/CsMidiSequencer.h"
#include "score/CsPlayList.h"
#include "windows/CsWinMain.h"
#include "windows/CsPainterSettings.h"
#include "windows/CsDlgRegistration.h"
#include "repo/CsRepoClient.h"

#include <QApplication>
#include <QSettings>
#include <QLocale>
#include <QTranslator>
#include <QThread>

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


  //Default editor settings
//  if( !s.contains(KEY_TRAIN_SETTINGS) ) {
//    //Train windows settings not assigned
//    CsPainterSettings ps;
//    ps.write( QStringLiteral(KEY_TRAIN_SETTINGS) );
//    }

  CsPlayList playList;
  playList.load();

  //Repo client
  repoClient = new CsRepoClient( playList );

  if( !repoClient->isRegistered() ) {
    CsDlgRegistration dlg(nullptr);
    if( !dlg.exec() )
      return 0;
    }

  //=============================================================================
  //        MIDI keyboard setup
  //Start midi keyboard
  QThread *midiThread = new QThread();
  CsMidiSequencer *midiSequencer = new CsMidiSequencer( midiThread );

  CsWinMain w( playList, midiSequencer );
  w.show();

  midiThread->start();
  int res = a.exec();

  midiThread->quit();
  midiThread->wait();

  playList.save();

  return res;
  }
