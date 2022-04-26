/*
  Project "SaliScore Score music edit, view and tutorial programm"

  Author
    Sibilev Alexander S.
  Web
    www.saliLab.com
    www.saliLab.ru

  Description
    SaliScore configuration and history

    One tick is 1/256 part of hole note

    One takt is one hole note

  History
    15.02.2022 v0.1   Append play list widget universal for desktop and android
    17.02.2022 v0.2   Append remote repository support
               v0.3   Append finding in repository
*/
#ifndef CSCONFIG_H
#define CSCONFIG_H

#define CS_AUTHOR                    "Alexander Sibilev"
#define CS_ORGANIZATION_NAME         "SaliLAB"
#define CS_ORGANIZATION_DOMAIN       "http://SaliLab.com/"
#define CS_APPLICATION_NAME          "SaliScoreAndroid"

//Version definition
#define CS_VERSION_MAJOR             0
#define CS_VERSION_MINOR             3

//Some defaults
#define CS_DEFAULT_WEB               "www.SaliLAB.com"
#define CS_DEFAULT_REPO              "score.salilab.ru/"
#define CS_UPGRADE_WEB               "http:/salicad.salilab.com/downloads/"


#define CS_SAMPLES_PER_SECOND        44100
#define CS_SAMPLES_PER_20MS          (CS_SAMPLES_PER_SECOND * 20 / 1000)

#define TICK_PERIOD_MS               20
#define TICK_PERIOD_US               (TICK_PERIOD_MS * 1000)


//Project naming definition
#define CS_BASE_EXTENSION            ".saliscore"
#define CS_DATA_DIRECTORY            "/SaliScore/"
#define CS_DEFAULT_FILE_NAME         "default"

//SaliScore database version
//Must be compatible with previous versions
#define CS_PLAY_LIST_VERSION_1       1
#define CS_BASE_VERSION_1            1

//Current SaliScore database version
#define CS_PLAY_LIST_VERSION         CS_PLAY_LIST_VERSION_1
#define CS_BASE_VERSION              CS_BASE_VERSION_1
#define CS_BASE_VERSION_KEY          "BaseVersion"

#define CS_PLAY_LIST_TYPE            "SaliLAB SaliScore play list file"
#define CS_BASE_TYPE                 "SaliLAB SaliScore file"
#define CS_BASE_TYPE_KEY             "BaseType"

//Previous file count in menu
#define CS_PREVIOUS_FILES_COUNT      10

//Undo history count
#define CS_UNDO_HISTORY_SIZE         1000

//Clipboard objects format
#define CS_CLIP_FORMAT_SELECTOR      "bin/saliscore-CsSelector"


//Settings key names
#define KEY_LANGUAGE                 "Language"
#define KEY_MAIN_SPLITTER            "MainSplitter"
#define KEY_WMAIN_MAX                "WMainMax"
#define KEY_WMAIN_SIZE               "WMainSize"
#define KEY_TRAIN_SETTINGS           "TrainSettings"
#define KEY_EDITOR_SETTINGS          "EditorSettings"
#define KEY_KARAOKE_SETTINGS         "KaraokeSettings"
#define KEY_KARAOKE_ANIMATION        "KaraokeAnimation"
#define KEY_AUTHOR                   "Author"
#define KEY_PASSWORD                 "Password"
#define KEY_WEB_REPO                 "WebRepo"
#define KEY_REMOTE_SYNC              "RemoteSync"
#define KEY_LOCAL_SYNC               "LocalSync"

#endif // CSCONFIG_H
