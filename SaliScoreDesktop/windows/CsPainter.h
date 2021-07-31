#ifndef CSPAINTER_H
#define CSPAINTER_H

#include "score/CsRemark.h"
#include "score/CsChordLine.h"
#include "score/CsNoteLine.h"
#include "score/CsLyric.h"
#include "score/CsLine.h"
#include "score/CsComposition.h"

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
    int       mScoreLineDistance;
  public:
    CsPainter( QPainter *painter, const CsComposition &comp );

    void drawLine( const CsLine &line );


    //void drawLyric( int x, int y, const CsLyric &lyric );

  private:
    void drawRemark( const QMap<QString,QString> &remarkMap );

    void drawChord(const QMap<QString, CsChordLine> &chordMap );

    void drawNote(const QMap<QString,CsNoteLine> &noteMap );

    void drawLyric( const CsLyricList &lyricList );

    void drawTranslation( const QMap<QString,QString> &translationMap );



    void drawRemarkImpl( int x, int y, const QString &rem );

    void drawChordImpl( int x, int y, const CsChordLine &chordLine );

    void drawNoteImpl( int x, int y, const CsNoteLine &noteLine );

    void drawTranslationImpl( int x, int y, const QString &tran );

    int  visualX( int x, int pos );
  };

#endif // CSPAINTER_H
