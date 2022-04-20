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
    SdLib/SdTime2x.cpp \
    SvJson/SvJsonIO.cpp \
    audioOut/CsSoundBufferIODevice.cpp \
    main.cpp \
    repo/CsRepoClient.cpp \
    score/CsChord.cpp \
    score/CsChordKit.cpp \
    score/CsChordLine.cpp \
    score/CsComposition.cpp \
    score/CsCompositionHeader.cpp \
    score/CsCompositionSettings.cpp \
    score/CsCursor.cpp \
    score/CsCursorPosition.cpp \
    score/CsDefinition.cpp \
    score/CsDescrSong.cpp \
    score/CsLine.cpp \
    score/CsLyricSymbol.cpp \
    score/CsNote.cpp \
    score/CsNoteKit.cpp \
    score/CsNoteLine.cpp \
    score/CsPlayList.cpp \
    score/CsPlayPart.cpp \
    score/CsPosition.cpp \
    score/CsReference.cpp \
    score/CsRemark.cpp \
    score/CsSongLocalRepo.cpp \
    score/CsSynthSettings.cpp \
    score/CsTrainInterval.cpp \
    synth/CsSynthMeandr.cpp \
    visual/CsAndroidWinMain.cpp \
    visual/CsCellCursor.cpp \
    visual/CsDlgRegistration.cpp \
    visual/CsPainter.cpp \
    visual/CsPainterSettings.cpp \
    visual/CsPainterTrain.cpp \
    visual/CsVisualAbstractList.cpp \
    visual/CsVisualAbstractListView.cpp \
    visual/CsVisualPartList.cpp \
    visual/CsVisualPlayList.cpp \
    visual/CsVisualRegularList.cpp \
    visual/CsVisualScore.cpp \
    visual/CsVisualScoreTrain.cpp \
    visual/CsWinScoreMode.cpp \
    visual/CsWinScoreView.cpp \
    visual/CsWinTrain.cpp

HEADERS += \
    SdLib/SdTime2x.h \
    SvJson/SvJsonIO.h \
    audioOut/CsSoundBufferIODevice.h \
    audioOut/CsSoundSample.h \
    audioOut/CsSoundSource.h \
    config.h \
    repo/CsRepoClient.h \
    score/CsChord.h \
    score/CsChordKit.h \
    score/CsChordLine.h \
    score/CsComposition.h \
    score/CsCompositionHeader.h \
    score/CsCompositionSettings.h \
    score/CsCursor.h \
    score/CsCursorPosition.h \
    score/CsDefinition.h \
    score/CsDescrSong.h \
    score/CsJsonIO.h \
    score/CsLine.h \
    score/CsLyricSymbol.h \
    score/CsNote.h \
    score/CsNoteChord.h \
    score/CsNoteKit.h \
    score/CsNoteLine.h \
    score/CsPlayList.h \
    score/CsPlayPart.h \
    score/CsPosition.h \
    score/CsReference.h \
    score/CsRemark.h \
    score/CsSongLocalRepo.h \
    score/CsSynthSettings.h \
    score/CsTrainInterval.h \
    synth/CsSynthMeandr.h \
    visual/CsAndroidWinMain.h \
    visual/CsCellCursor.h \
    visual/CsDlgRegistration.h \
    visual/CsPainter.h \
    visual/CsPainterSettings.h \
    visual/CsPainterTrain.h \
    visual/CsVisualAbstractList.h \
    visual/CsVisualAbstractListView.h \
    visual/CsVisualPartList.h \
    visual/CsVisualPlayList.h \
    visual/CsVisualRegularList.h \
    visual/CsVisualScore.h \
    visual/CsVisualScoreTrain.h \
    visual/CsWinScoreMode.h \
    visual/CsWinScoreView.h \
    visual/CsWinTrain.h

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
