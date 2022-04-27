#ifndef CSVISUALPIANO_H
#define CSVISUALPIANO_H

#include <QWidget>
#include <QSet>
#include <QMap>

class CsVisualPiano : public QWidget
  {
    Q_OBJECT

    int mNoteStart;
    int mNoteStop;

    QSet<int>     mHighlightKeys;
    QSet<int>     mPressedKeys;
    QList<QRect>  mWhiteKeys;
    QMap<int,int> mWhiteKeysMap;
    QList<QRect>  mBlackKeys;
    QMap<int,int> mBlackKeysMap;
  public:
    explicit CsVisualPiano(QWidget *parent = nullptr);

  signals:
    void midiNote( int note, int velo );

  public slots:
    void playNote( int note, int velo );
    void playHighlight( int channel, int note, int velo );
    void settings();

    // QWidget interface
  protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

  private:
    void clear();
    int  findKey( QPoint pos );

  };

#endif // CSVISUALPIANO_H
