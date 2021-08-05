#ifndef CSTICKGENERATOR_H
#define CSTICKGENERATOR_H

#include <QObject>
#include <QTimer>

class CsTickGenerator : public QObject
  {
    Q_OBJECT

    QTimer mLocalGenerator;
    bool   mLink;
    bool   mRun;
  public:
    explicit CsTickGenerator(QObject *parent = nullptr);

    bool isLink() const { return mLink; }

    bool isRun() const { return mRun; }
    void setRun( bool on );

  signals:
    void tick( int count );

    void midiLink( bool on );

  public slots:
    void setTempo( int tempo );
  };

#endif // CSTICKGENERATOR_H
