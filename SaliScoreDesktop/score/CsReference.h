#ifndef CSREFERENCE_H
#define CSREFERENCE_H

#include <QRect>
#include <QString>
#include <QList>

enum CsRefType {
  crtTitle,
  crtSinger,
  crtComposer,
  crtLyricist,

  crtVoice,
  crtVoiceDual,
  crtVoiceRight,
  crtStyle,
  crtTempo,

  crtRemark,
  crtChordLine,
  crtChord,
  crtChordPlacehold,
  crtClef,
  crtNumerator,
  crtDenomenator,
  crtNoteLine,
  crtNote,
  crtNotePlacehold,
  crtLyricLine,
  crtLyric,
  crtLyricPlacehold,
  crtTranslation
  };


class CsReference
  {
    QRect   mRect;
    int     mType;
    int     mLine;
    QString mPart;
    int     mIndex;
  public:
    CsReference( QRect r, int type, int line, const QString &part, int index );

    bool    isHit( QPoint p ) const { return mRect.contains(p); }

    int     type() const { return mType; }

    int     line() const { return mLine; }

    QString part() const { return mPart; }

    int     index() const { return mIndex; }

  };


using CsReferenceList = QList<CsReference>;

#endif // CSREFERENCE_H
