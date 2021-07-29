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

History
  28.07.2021 v0.1  Begin creation
             v0.2  Begin creation composition structure
             v0.3  Begin main menu creation
             v0.4  Append import manager system to load sources
  29.07.2021 v0.5  Append json reading and writing
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
#define CS_VERSION_MINOR             5

//Some defaults
#define CS_DEFAULT_WEB               "www.SaliLAB.com"
#define CS_DEFAULT_REPO              "repo.salicad.com/"
#define CS_UPGRADE_WEB               "http:/salicad.salilab.com/downloads/"


//Project naming definition
#define CS_BASE_EXTENSION            ".saliscore"
#define CS_DEFAULT_FILE_NAME         "default"

//SaliScore database version
//Must be compatible with previous versions
#define CS_BASE_VERSION_1            1

//Current SaliScore database version
#define CS_BASE_VERSION              CS_BASE_VERSION_1
#define CS_BASE_VERSION_KEY          "Version"

#define CS_BASE_TYPE                 "SaliLAB SaliScore file"
#define CS_BASE_TYPE_KEY             "FileType"

//Previous file count in menu
#define CS_PREVIOUS_FILES_COUNT      10

//Undo history count
#define CS_UNDO_HISTORY_SIZE         1000

//Clipboard objects format
#define CS_CLIP_FORMAT_SELECTOR      "bin/saliscore-CsSelector"

//Size of picture to draw fragment to clipboard
#define CS_CLIP_IMAGE_WIDTH          1920
#define CS_CLIP_IMAGE_HEIGHT         1280

//Max default delivered limit elements
#define CS_DEFAULT_DELIVERED_LIMIT   100

//Max object in find list of GetObject dialog
#define CS_GET_OBJECT_MAX_FIND_LIST  300

//Settings key names
#define SDK_WMAIN_MAX                "WMainMax"
#define SDK_LANGUAGE                 "Language"
#define SDK_MAIN_SPLITTER            "MainSplitter"
#define SDK_PREVIOUS_FILES           "PreviousFiles"
#define SDK_GLOBAL_AUTHOR            "Author global id"
#define SDK_GLOBAL_PASSWORD          "Author global password"
#define SDK_LOCAL_SYNC               "Local sync"
#define SDK_REMOTE_SYNC              "Remote sync"
#define SDK_REMOTE_REMAIN            "Remote remain"
#define SDK_SERVER_REPO              "SaliCAD repository"
#define SDK_ENVIR_VERSION            "Envir version"
#define SDK_ENVIR                    "Envir"
#define SDK_HELP_PATH                "HelpPath"

#define MODE_HELP                    //Prefix to mode help files


#endif // SDCONFIG

