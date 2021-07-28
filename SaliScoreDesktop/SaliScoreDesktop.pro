QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CsBug.cpp \
    CsMain.cpp \
    import/CsImport.cpp \
    import/text/CsImportText.cpp \
    score/CsChord.cpp \
    score/CsChordKit.cpp \
    score/CsChordLine.cpp \
    score/CsComposition.cpp \
    score/CsDuration.cpp \
    score/CsLine.cpp \
    score/CsLyric.cpp \
    score/CsNote.cpp \
    score/CsNoteKit.cpp \
    score/CsNoteLine.cpp \
    score/CsPosition.cpp \
    score/CsRemark.cpp \
    windows/CsWinCommand.cpp \
    windows/CsWinEditor.cpp \
    windows/CsWinHelp.cpp \
    windows/CsWinMain.cpp

HEADERS += \
    CsConfig.h \
    import/CsImport.h \
    import/text/CsImportText.h \
    score/CsChord.h \
    score/CsChordKit.h \
    score/CsChordLine.h \
    score/CsComposition.h \
    score/CsDuration.h \
    score/CsLine.h \
    score/CsLyric.h \
    score/CsNote.h \
    score/CsNoteKit.h \
    score/CsNoteLine.h \
    score/CsPosition.h \
    score/CsRemark.h \
    windows/CsWinCommand.h \
    windows/CsWinEditor.h \
    windows/CsWinHelp.h \
    windows/CsWinMain.h

TRANSLATIONS += \
    SaliScoreDesktop_en_001.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
