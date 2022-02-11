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
*/
#ifndef CONFIG_H
#define CONFIG_H

#define CS_AUTHOR                    "Alexander Sibilev"
#define CS_ORGANIZATION_NAME         "SaliLAB"
#define CS_ORGANIZATION_DOMAIN       "http://SaliLab.com/"
#define CS_APPLICATION_NAME          "SaliScoreDesktop"

//Version definition
#define CS_VERSION_MAJOR             0
#define CS_VERSION_MINOR             1

//Some defaults
#define CS_DEFAULT_WEB               "www.SaliLAB.com"
#define CS_DEFAULT_REPO              "score.salilab.ru/"
#define CS_UPGRADE_WEB               "http:/salicad.salilab.com/downloads/"


#define CS_SAMPLES_PER_SECOND        44100
#define CS_SAMPLES_PER_20MS          (CS_SAMPLES_PER_SECOND * 20 / 1000)



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



#endif // CONFIG_H
