#ifndef CSPAINTER_H
#define CSPAINTER_H

#include "score/CsRemark.h"
#include "score/CsChordLine.h"
#include "score/CsNoteLine.h"
#include "score/CsLyric.h"
#include "score/CsState.h"
#include "score/CsLine.h"

#include <QPainter>
#include <QStringList>

class CsPainter
  {
    QPainter *mPainter;

    QStringList mVisibleRemark;
    QStringList mVisibleChord;
    QStringList mVisibleNote;
    QStringList mVisibleTranslate;

    int       mCurY;
    int       mPixPerX;
  public:
    CsPainter( QPainter *painter, const CsState &st );

    void drawLine( const CsLine &line );


    //void drawLyric( int x, int y, const CsLyric &lyric );

  private:
    void drawRemark( const QMap<QString,QString> &remarkMap );

    void drawChord(const QMap<QString, CsChordLine> &chordMap );

    void drawNote(const QMap<QString,CsNoteLine> &noteMap );

    void drawLyric( const CsLyricList &lyricList );



    void drawRemarkImpl( int x, int y, const QString &rem );

    void drawChordImpl( int x, int y, const CsChordLine &chordLine );

    void drawNoteImpl( int x, int y, const CsNoteLine &noteLine );

    int  visualX( int x, int pos );
  };

#endif // CSPAINTER_H
