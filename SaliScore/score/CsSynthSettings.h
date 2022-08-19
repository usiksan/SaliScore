#ifndef CSSYNTHSETTINGS_H
#define CSSYNTHSETTINGS_H

#include "CsJsonIO.h"

#include <QString>

class CsSynthSettings
  {
    int  mVoice;
    int  mVoiceDual;
    int  mVoiceLeft;
    int  mStyle;
    int  mTempo;
  public:
    CsSynthSettings();

    QString voice() const { return QString::number( mVoice ); }
    void    voiceSet( const QString &voi ) { mVoice = voi.toInt(); }

    QString voiceDual() const { return QString::number( mVoiceDual); }
    void    voiceDualSet( const QString &voi ) { mVoiceDual = voi.toInt(); }

    QString voiceLeft() const { return QString::number( mVoiceLeft ); }
    void    voiceLeftSet( const QString &voi ) { mVoiceLeft = voi.toInt(); }

    QString style() const { return QString::number( mStyle ); }
    void    styleSet( const QString &stl ) { mStyle = stl.toInt(); }

    QString tempo() const { return QString::number( mTempo ); }
    void    tempoSet( const QString &tmp ) { mTempo = tmp.toInt(); }

    void    clear();

    //=================================================================
    //         Settings JSON io

    void    jsonWrite( CsJsonWriter &js ) const;

    void    jsonRead( CsJsonReader &js );

  };

#endif // CSSYNTHSETTINGS_H
