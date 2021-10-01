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
  28.07.2021 v0.1  Begin creation
             v0.2  Begin creation composition structure
             v0.3  Begin main menu creation
             v0.4  Append import manager system to load sources
  29.07.2021 v0.5  Append json reading and writing
  30.07.2021 v0.6  Begin to create drawing system
  31.07.2021 v0.7  Append settings as individual tuning of composition
             v0.8  Transfer settings into composition. So settings are the part of composition
                   and can be write-read separatelly
             v0.9  Complete with transfer settings into composition
                   Try drawing notes by standard font
             v0.10 Append clef info into composition
             v0.11 Preliminary finish with note drawing. All drawing completed.
  01.08.2021 v0.12 Append restoring size of main window
             v0.13 Append individual viewer settings
                   Append separated settings for different kind of information
                   Begin releasing viewer switching
             v0.14 Append intermediate page window to support playLists
             v0.15 Append recent file functionality
             v0.16 Append some help actions
                   Append view switching
             v0.17 Begin creation definition editing dialog
  02.08.2021 v0.18 Complete with definition editing dialog
                   Append chord definition dialog
             v0.19 Append note definition dialog and translation definition dialog
                   Append title and score properties paint into painter
             v0.20 Begin player
  03.08.2021 v0.21 Inject common class CsWinScoreMode as base class for all view of score
                   Append taktCount for line
                   Modify painter to accept taktCount for lines
  04.08.2021 v0.22 Create paint for karaoke mode with multypurpose view and animation
                   Realised simple single line karaoke mode
             v0.23 Append text import
             v0.24 To append scrollbar to edit and train windows make refactory of score modes.
                   Fact modes at now are views and mode represents by single class CsWinScoreMode,
                   which is QAbstractScrollBar and separate views are them viewports
             v0.25 Append scrolling to windows, made scroll for trainMode
  05.08.2021 v0.26 Append soft tick generator and prepare midi tick sync
                   Fix some errors in player with tick accept
                   Move duration field into CsPosition. So any position at now has duration
  08.08.2021 v0.27 Begin creating score edit concept
             v0.28 Append cell drawing with cursor support
  10.08.2021 v0.29 Append editing cursor. Begin creating text editing cursor. Append property title edit
  16.08.2021 v0.30 Reformat all system: At now base of programm is playList. For compositions now only one
                   window, which display active song from playList. Compositions are saved in special
                   dataPlace. There compositions are manadged.
  30.08.2021 v0.31 Append sync with remote repo. Append sync for playlist: released download and upload
  08.09.2021 v0.32 Append left-right moving chord, note and lyric
  15.09.2021 v0.33 Fix: with empty singer or title song not uploaded
             v0.34 Fix: chord like F# not detected right because F# detection resides AFTER F but must be earlear
  16.09.2021 v0.35 Ajust cell display for all cell class
                   Append mouse point for lyric symbols
  24.09.2021 v0.36 Append line offset for all composition with takt lines offset
                   Append special key Ctrl+Tab for +1/4 align
                   Append Delete key to chord edit
                   Append right key to end chord edit and move right
                   Fix: selection box for lyric
             v0.37 Append shift cursor when insert align
             v0.38 Set musical size as global to hole composition
                   Refactor current position display
                   Append display and edit synth settings (voice, style and so on)
                   Append autoscroll by player
                   Append hide player position when stop
  29.09.2021 v0.39 Block edit possibilities when mode other than editor
             v0.40 Append different edit menu in edit mode and others modes
  30.09.2021 v0.41 Append score settings dialog
                   Append numerator and denominator display
             v0.42 Union paint algorithm for edit mode and training mode
             v0.43 Refactor view windows from common scroll widget to concrete view widget
  01.10.2021 v0.44 Appended fragment support in train mode
             v0.45 When edit lyric exclude lyric intersection
             v0.46 Append score clipboard operations
*/

#ifndef CSCONFIG
#define CSCONFIG

//Do'nt change this names!
#define CS_AUTHOR                    "Alexander Sibilev"
#define CS_ORGANIZATION_NAME         "SaliLAB"
#define CS_ORGANIZATION_DOMAIN       "http://SaliLab.com/"
#define CS_APPLICATION_NAME          "SaliScoreDesktop"

//Version definition
#define CS_VERSION_MAJOR             0
#define CS_VERSION_MINOR             46

//Some defaults
#define CS_DEFAULT_WEB               "www.SaliLAB.com"
#define CS_DEFAULT_REPO              "score.salilab.ru/"
#define CS_UPGRADE_WEB               "http:/salicad.salilab.com/downloads/"


//Project naming definition
#define CS_BASE_EXTENSION            ".saliscore"
#define CS_DATA_DIRECTORY            "/SaliScore/"
#define CS_DEFAULT_FILE_NAME         "default"

//SaliScore database version
//Must be compatible with previous versions
#define CS_BASE_VERSION_1            1

//Current SaliScore database version
#define CS_BASE_VERSION              CS_BASE_VERSION_1
#define CS_BASE_VERSION_KEY          "BaseVersion"

#define CS_BASE_TYPE                 "SaliLAB SaliScore file"
#define CS_BASE_TYPE_KEY             "BaseType"

//Previous file count in menu
#define CS_PREVIOUS_FILES_COUNT      10

//Undo history count
#define CS_UNDO_HISTORY_SIZE         1000

//Clipboard objects format
#define CS_CLIP_FORMAT_SELECTOR      "bin/saliscore-CsSelector"

//Size of picture to draw fragment to clipboard
//#define CS_CLIP_IMAGE_WIDTH          1920
//#define CS_CLIP_IMAGE_HEIGHT         1280

//Max default delivered limit elements
//#define CS_DEFAULT_DELIVERED_LIMIT   100

//Max object in find list of GetObject dialog
//#define CS_GET_OBJECT_MAX_FIND_LIST  300

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

//#define SDK_PREVIOUS_FILES           "PreviousFiles"
//#define SDK_GLOBAL_AUTHOR            "Author global id"
//#define SDK_GLOBAL_PASSWORD          "Author global password"
//#define SDK_LOCAL_SYNC               "Local sync"
//#define SDK_REMOTE_SYNC              "Remote sync"
//#define SDK_REMOTE_REMAIN            "Remote remain"
//#define SDK_SERVER_REPO              "SaliCAD repository"
//#define SDK_ENVIR_VERSION            "Envir version"
//#define SDK_ENVIR                    "Envir"
//#define SDK_HELP_PATH                "HelpPath"

//#define MODE_HELP                    //Prefix to mode help files


#endif // SDCONFIG

