#ifndef CSMIDIKEYBOARD_H
#define CSMIDIKEYBOARD_H

#include <QObject>

class CsMidiKeyboard : public QObject
  {
    Q_OBJECT

    int     mBank;       //!< Full voice bank MSB bank:LSB bank
  public:
    explicit CsMidiKeyboard(QObject *parent = nullptr);

    virtual void init() = 0;

    virtual bool isLink() const = 0;

    virtual void setTempo( int tempo ) = 0;

  signals:
    void tick( int count );

    void midiNote( int note, int velo );

    void midiRun( bool run );

    void midiLink( bool on );

    void midiVoice( int voice );

  public slots:
    virtual void playRun( bool run ) = 0;
    virtual void playNote( int note, int velo ) = 0;
    virtual void playTempo( int tempo ) = 0;
    virtual void playVoice( int voice ) = 0;

  protected:
    void midiSignal( quint8 control, quint8 data0, quint8 data1 );
  };

#endif // CSMIDIKEYBOARD_H
