/*
  Project "SaliScore Score music edit, view and tutorial program"

  Author
    Sibilev Alexander S.
  Web
    www.saliLab.com
    www.saliLab.ru

  Description
    Sound buffer device - interface device with sound card of computer.
    Main idea: sound synthesizes by notes. Simultaneously supported a lot of notes.
    Each of note synthesizes sound separately and SoundBufferIODevice mixes them into
    single stream. When we need to play some note we append it SoundBufferIODevice
    notes play list. When note end playing we remove it from notes play list.
*/
#ifndef CSSOUNDBUFFERIODEVICE_H
#define CSSOUNDBUFFERIODEVICE_H

#include "CsConfig.h"
#include "CsSoundSource.h"

#include <QIODevice>

class QAudioOutput;

class CsSoundBufferIODevice : public QIODevice
  {
    Q_OBJECT

    QList<CsSoundSourcePtr>       mActiveSounds; //!< List of current actived sounds

    static CsSoundBufferIODevice *mSoundBuffer;  //!< Single audio buffer
    static QAudioOutput          *mAudioOutput;  //!< Audio output

    CsSoundBufferIODevice();
  public:

    //!
    //! \brief soundBuffer Returns single sound buffer
    //! \return            Single sound buffer
    //!
    static CsSoundBufferIODevice *soundBuffer();
  public slots:
    //!
    //! \brief addNote Append sound to active sound list
    //! \param notePtr Note to append
    //!
    void addSound( CsSoundSourcePtr soundPtr );

    // QIODevice interface
  public:
    virtual bool   isSequential() const override;
    virtual qint64 bytesAvailable() const override;

  protected:
    //With this sound card queries next portion af data to play
    virtual qint64 readData(char *data, qint64 maxlen) override;

    //Not used
    virtual qint64 writeData(const char *data, qint64 len) override;
  };

#endif // CSSOUNDBUFFERIODEVICE_H
