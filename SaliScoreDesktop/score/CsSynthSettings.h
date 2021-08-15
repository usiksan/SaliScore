#ifndef CSSYNTHSETTINGS_H
#define CSSYNTHSETTINGS_H

#include <QString>

class CsSynthSettings
  {
    int  mVoice;
    int  mVoiceDual;
    int  mVoiceRight;
    int  mStyle;
    int  mTempo;
  public:
    CsSynthSettings();

    QString     voice() const { return QString::number( mVoice ); }
    void        voiceSet( const QString &voi ) { mVoice = voi.toInt(); }

    QString     voiceDual() const { return QString::number( mVoiceDual); }
    void        voiceDualSet( const QString &voi ) { mVoiceDual = voi.toInt(); }

    QString     voiceRight() const { return QString::number( mVoiceRight ); }
    void        voiceRightSet( const QString &voi ) { mVoiceRight = voi.toInt(); }

    QString     style() const { return QString::number( mStyle ); }
    void        styleSet( const QString &stl ) { mStyle = stl.toInt(); }

    QString     tempo() const { return QString::number( mTempo ); }
    void        tempoSet( const QString &tmp ) { mTempo = tmp.toInt(); }


  };

#endif // CSSYNTHSETTINGS_H
