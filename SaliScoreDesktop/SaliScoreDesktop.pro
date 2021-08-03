QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CsBug.cpp \
    CsMain.cpp \
    SvJson/SvJsonIO.cpp \
    import/CsImport.cpp \
    import/CsImportManager.cpp \
    import/saliScore/CsImportSaliScore.cpp \
    import/text/CsImportText.cpp \
    play/CsPlay.cpp \
    score/CsChord.cpp \
    score/CsChordKit.cpp \
    score/CsChordLine.cpp \
    score/CsComposition.cpp \
    score/CsDefinition.cpp \
    score/CsDuration.cpp \
    score/CsLine.cpp \
    score/CsLyric.cpp \
    score/CsNote.cpp \
    score/CsNoteKit.cpp \
    score/CsNoteLine.cpp \
    score/CsPosition.cpp \
    score/CsReference.cpp \
    score/CsRemark.cpp \
    windows/CsDlgDefChord.cpp \
    windows/CsDlgDefNote.cpp \
    windows/CsDlgDefRemark.cpp \
    windows/CsDlgDefTranslation.cpp \
    windows/CsDlgDefinition.cpp \
    windows/CsPaintStyle.cpp \
    windows/CsPainter.cpp \
    windows/CsPainterSettings.cpp \
    windows/CsWinEditor.cpp \
    windows/CsWinHelp.cpp \
    windows/CsWinKaraoke.cpp \
    windows/CsWinMain.cpp \
    windows/CsWinPage.cpp \
    windows/CsWinScore.cpp \
    windows/CsWinScoreMode.cpp \
    windows/CsWinTrain.cpp

HEADERS += \
    CsConfig.h \
    SvJson/SvJsonIO.h \
    import/CsImport.h \
    import/CsImportManager.h \
    import/saliScore/CsImportSaliScore.h \
    import/text/CsImportText.h \
    play/CsPlay.h \
    score/CsChord.h \
    score/CsChordKit.h \
    score/CsChordLine.h \
    score/CsComposition.h \
    score/CsDefinition.h \
    score/CsDuration.h \
    score/CsJsonIO.h \
    score/CsLine.h \
    score/CsLyric.h \
    score/CsNote.h \
    score/CsNoteChord.h \
    score/CsNoteKit.h \
    score/CsNoteLine.h \
    score/CsPosition.h \
    score/CsReference.h \
    score/CsRemark.h \
    windows/CsDlgDefChord.h \
    windows/CsDlgDefNote.h \
    windows/CsDlgDefRemark.h \
    windows/CsDlgDefTranslation.h \
    windows/CsDlgDefinition.h \
    windows/CsPaintStyle.h \
    windows/CsPainter.h \
    windows/CsPainterSettings.h \
    windows/CsWinEditor.h \
    windows/CsWinHelp.h \
    windows/CsWinKaraoke.h \
    windows/CsWinMain.h \
    windows/CsWinPage.h \
    windows/CsWinScore.h \
    windows/CsWinScoreMode.h \
    windows/CsWinTrain.h

TRANSLATIONS += \
    SaliScoreDesktop_en_001.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
  SaliScore.qrc
