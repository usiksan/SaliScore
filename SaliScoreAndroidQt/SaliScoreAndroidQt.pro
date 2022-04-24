QT       += core gui network multimedia svg

android {
  QT       += androidextras
  }


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SvJson/SvSignal.cpp \
    import/CsImport.cpp \
    import/CsImportManager.cpp \
    import/saliScore/CsImportSaliScore.cpp \
    import/text/CsImportText.cpp \
    player/CsPlayer.cpp \
    player/CsPlayerEvent.cpp \
    visual/CsAndroidWinMain.cpp \
    visual/CsDesktopWinMain.cpp \
    SdLib/SdTime2x.cpp \
    audioOut/CsSoundBufferIODevice.cpp \
    main.cpp \
    repo/CsRepoClient.cpp \
    score/CsChord.cpp \
    score/CsComposition.cpp \
    score/CsCompositionInfo.cpp \
    score/CsCursor.cpp \
    score/CsCursorPosition.cpp \
    score/CsDefinition.cpp \
    score/CsLine.cpp \
    score/CsLyricSymbol.cpp \
    score/CsNote.cpp \
    score/CsPlayList.cpp \
    score/CsPlayPart.cpp \
    score/CsPosition.cpp \
    score/CsReference.cpp \
    score/CsSongLocalRepo.cpp \
    score/CsSynthSettings.cpp \
    score/CsTrainInterval.cpp \
    synth/CsSynthMeandr.cpp \
    visual/CsCellCursor.cpp \
    visual/CsCursorEdit.cpp \
    visual/CsCursorEditChord.cpp \
    visual/CsCursorEditLyric.cpp \
    visual/CsCursorEditNote.cpp \
    visual/CsCursorEditProperty.cpp \
    visual/CsCursorEditRemark.cpp \
    visual/CsCursorEditString.cpp \
    visual/CsCursorEditTranslation.cpp \
    visual/CsDlgRegistration.cpp \
    visual/CsPainter.cpp \
    visual/CsPainterEditor.cpp \
    visual/CsPainterSettings.cpp \
    visual/CsPainterTrain.cpp \
    visual/CsVisualAbstractList.cpp \
    visual/CsVisualAbstractListView.cpp \
    visual/CsVisualPartList.cpp \
    visual/CsVisualPlayList.cpp \
    visual/CsVisualRegularList.cpp \
    visual/CsVisualScore.cpp \
    visual/CsVisualScoreEdit.cpp \
    visual/CsVisualScoreKaraoke.cpp \
    visual/CsVisualScoreTrain.cpp \
    visual/CsVisualWinMain.cpp

HEADERS += \
    CsConfig.h \
    SdLib/SdTime2x.h \
    SvJson/SvJsonIO.h \
    SvJson/SvSignal.h \
    audioOut/CsSoundBufferIODevice.h \
    audioOut/CsSoundSample.h \
    audioOut/CsSoundSource.h \
    import/CsImport.h \
    import/CsImportManager.h \
    import/saliScore/CsImportSaliScore.h \
    import/text/CsImportText.h \
    player/CsPlayer.h \
    player/CsPlayerEvent.h \
    repo/CsRepoClient.h \
    score/CsChord.h \
    score/CsComposition.h \
    score/CsCompositionInfo.h \
    score/CsCursor.h \
    score/CsCursorPosition.h \
    score/CsDefinition.h \
    score/CsJsonIO.h \
    score/CsKit.h \
    score/CsLine.h \
    score/CsLyricSymbol.h \
    score/CsNote.h \
    score/CsNoteChord.h \
    score/CsPlayList.h \
    score/CsPlayPart.h \
    score/CsPosition.h \
    score/CsReference.h \
    score/CsSongLocalRepo.h \
    score/CsSynthSettings.h \
    score/CsTrainInterval.h \
    synth/CsSynthMeandr.h \
    visual/CsAndroidWinMain.h \
    visual/CsCellCursor.h \
    visual/CsCursorEdit.h \
    visual/CsCursorEditChord.h \
    visual/CsCursorEditLyric.h \
    visual/CsCursorEditNote.h \
    visual/CsCursorEditProperty.h \
    visual/CsCursorEditRemark.h \
    visual/CsCursorEditString.h \
    visual/CsCursorEditTranslation.h \
    visual/CsDesktopWinMain.h \
    visual/CsDlgRegistration.h \
    visual/CsPainter.h \
    visual/CsPainterEditor.h \
    visual/CsPainterSettings.h \
    visual/CsPainterTrain.h \
    visual/CsVisualAbstractList.h \
    visual/CsVisualAbstractListView.h \
    visual/CsVisualPartList.h \
    visual/CsVisualPlayList.h \
    visual/CsVisualRegularList.h \
    visual/CsVisualScore.h \
    visual/CsVisualScoreEdit.h \
    visual/CsVisualScoreKaraoke.h \
    visual/CsVisualScoreTrain.h \
    visual/CsVisualWinMain.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DISTFILES += \
  android/src/CsMidiAndroid.java \
  android/AndroidManifest.xml \
  android/build.gradle \
  android/gradle.properties \
  android/gradle/wrapper/gradle-wrapper.jar \
  android/gradle/wrapper/gradle-wrapper.properties \
  android/gradlew \
  android/gradlew.bat \
  android/res/values/libs.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

ANDROID_MIN_SDK_VERSION = 24
ANDROID_TARGET_SDK_VERSION = 25

ANDROID_FEATURES += android.software.midi

RESOURCES += \
  SaliScore.qrc
