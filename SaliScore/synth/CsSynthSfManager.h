#ifndef CSSYNTHSFMANAGER_H
#define CSSYNTHSFMANAGER_H

#include "CsSynthSfVoiceInfo.h"

#include <QObject>
#include <QList>
#include <QWidget>


class CsSynthSfManager : public QObject
  {
    Q_OBJECT

    QList<CsSynthSfVoiceInfo> mInfoList;
    QMap<int,int>             mVoiceIdMap;
    QMap<QString,int>         mVoiceMap;
    int                       mChannels[16];
    //QMap<int,int>             mChannelMap;
  public:
    explicit CsSynthSfManager(QObject *parent = nullptr);

    void build( QWidget *parent, const QString soundFontPath );

    void load( QWidget *parent );
  signals:

  public slots:
    void playNote( int channel, int pitch, int velo );
    void voiceSelect( int channel, int voiceId );
  };

#endif // CSSYNTHSFMANAGER_H
