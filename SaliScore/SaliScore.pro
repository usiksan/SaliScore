QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SvLib/SvDir.cpp \
    SvLib/SvTime2x.cpp \
    SvLib/SvVisualAbstractList.cpp \
    SvLib/SvVisualAbstractListView.cpp \
    SvLib/SvVisualRegularList.cpp \
    audioOut/CsSoundBufferIODevice.cpp \
    iff/IffReader.cpp \
    import/CsImport.cpp \
    import/CsImportManager.cpp \
    import/saliScore/CsImportSaliScore.cpp \
    import/text/CsImportText.cpp \
    main.cpp \
    MainWindow.cpp \
    midiKeyboard/CsMidiKeyboard.cpp \
    midiKeyboard/CsMidiKeyboardLinux.cpp \
    repo/CsRepoClient.cpp \
    soundFont2/CsSoundFont.cpp \
    soundFont2/CsSoundFontMap.cpp \
    synth/CsSynthMeandr.cpp \
    synth/CsSynthSfManager.cpp \
    synth/CsSynthSfNote.cpp \
    synth/CsSynthSfTrack.cpp \
    synth/CsSynthSfVoice.cpp \
    synth/CsSynthSfVoiceInfo.cpp

HEADERS += \
    CsConfig.h \
    MainWindow.h \
    SvLib/SvDir.h \
    SvLib/SvJsonIO.h \
    SvLib/SvSingleton.h \
    SvLib/SvTime2x.h \
    SvLib/SvVisualAbstractList.h \
    SvLib/SvVisualAbstractListView.h \
    SvLib/SvVisualRegularList.h \
    audioOut/CsSoundBufferIODevice.h \
    audioOut/CsSoundSample.h \
    audioOut/CsSoundSource.h \
    iff/IffReader.h \
    import/CsImport.h \
    import/CsImportManager.h \
    import/saliScore/CsImportSaliScore.h \
    import/text/CsImportText.h \
    midiKeyboard/CsMidiKeyboard.h \
    midiKeyboard/CsMidiKeyboardLinux.h \
    repo/CsRepoClient.h \
    soundFont2/CsSoundFont.h \
    soundFont2/CsSoundFontMap.h \
    synth/CsSynthMeandr.h \
    synth/CsSynthSfManager.h \
    synth/CsSynthSfNote.h \
    synth/CsSynthSfTrack.h \
    synth/CsSynthSfVoice.h \
    synth/CsSynthSfVoiceInfo.h \
    synth/CsSynthVoiceId.h

TRANSLATIONS += \
    SaliScore_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
