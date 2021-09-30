QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CsBug.cpp \
    CsMain.cpp \
    SdLib/SdTime2x.cpp \
    midi/CsMidiSequencer.cpp \
    SvJson/SvJsonIO.cpp \
    import/CsImport.cpp \
    import/CsImportManager.cpp \
    import/saliScore/CsImportSaliScore.cpp \
    import/text/CsImportText.cpp \
    play/CsPlay.cpp \
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
    score/CsSynthSettings.cpp \
    score/CsTrainInterval.cpp \
    undo/CsUndo.cpp \
    undo/CsUndoItem.cpp \
    windows/CsCellCursor.cpp \
    windows/CsCursorEdit.cpp \
    windows/CsCursorEditChord.cpp \
    windows/CsCursorEditLyric.cpp \
    windows/CsCursorEditNote.cpp \
    windows/CsCursorEditProperty.cpp \
    windows/CsCursorEditRemark.cpp \
    windows/CsCursorEditString.cpp \
    windows/CsCursorEditTranslation.cpp \
    windows/CsDlgDefChord.cpp \
    windows/CsDlgDefNote.cpp \
    windows/CsDlgDefRemark.cpp \
    windows/CsDlgDefTranslation.cpp \
    windows/CsDlgDefinition.cpp \
    windows/CsDlgRegistration.cpp \
    windows/CsDlgScoreSettings.cpp \
    windows/CsKaraokePage.cpp \
    windows/CsPaintStyle.cpp \
    windows/CsPainter.cpp \
    windows/CsPainterEditor.cpp \
    windows/CsPainterSettings.cpp \
    windows/CsWinEditor.cpp \
    windows/CsWinHelp.cpp \
    windows/CsWinIntro.cpp \
    windows/CsWinKaraoke.cpp \
    windows/CsWinMain.cpp \
    windows/CsWinPlayList.cpp \
    windows/CsWinRemote.cpp \
    windows/CsWinScoreMode.cpp \
    windows/CsWinScoreView.cpp \
    windows/CsWinTrain.cpp

HEADERS += \
    CsConfig.h \
    SdLib/SdTime2x.h \
    midi/CsMidiSequencer.h \
    SvJson/SvJsonIO.h \
    import/CsImport.h \
    import/CsImportManager.h \
    import/saliScore/CsImportSaliScore.h \
    import/text/CsImportText.h \
    play/CsPlay.h \
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
    score/CsSynthSettings.h \
    score/CsTrainInterval.h \
    undo/CsUndo.h \
    undo/CsUndoItem.h \
    windows/CsCellCursor.h \
    windows/CsCursorEdit.h \
    windows/CsCursorEditChord.h \
    windows/CsCursorEditLyric.h \
    windows/CsCursorEditNote.h \
    windows/CsCursorEditProperty.h \
    windows/CsCursorEditRemark.h \
    windows/CsCursorEditString.h \
    windows/CsCursorEditTranslation.h \
    windows/CsDlgDefChord.h \
    windows/CsDlgDefNote.h \
    windows/CsDlgDefRemark.h \
    windows/CsDlgDefTranslation.h \
    windows/CsDlgDefinition.h \
    windows/CsDlgRegistration.h \
    windows/CsDlgScoreSettings.h \
    windows/CsKaraokePage.h \
    windows/CsPaintStyle.h \
    windows/CsPainter.h \
    windows/CsPainterEditor.h \
    windows/CsPainterSettings.h \
    windows/CsWinEditor.h \
    windows/CsWinHelp.h \
    windows/CsWinIntro.h \
    windows/CsWinKaraoke.h \
    windows/CsWinMain.h \
    windows/CsWinPlayList.h \
    windows/CsWinRemote.h \
    windows/CsWinScoreMode.h \
    windows/CsWinScoreView.h \
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
