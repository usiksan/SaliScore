#ifndef CSPAINTER_H
#define CSPAINTER_H

#include "CsConfig.h"
#include "CsPainterSettings.h"
#include "score/CsRemark.h"
#include "score/CsChordLine.h"
#include "score/CsNoteLine.h"
#include "score/CsLyric.h"
#include "score/CsLine.h"
#include "score/CsComposition.h"
#include "score/CsReference.h"

#include <QPainter>
#include <QStringList>

class CsPainter
  {
    QPainter         *mPainter;

    //Composition settings
    QStringList       mVisibleRemark;
    QStringList       mVisibleChord;
    QStringList       mVisibleNote;
    QStringList       mVisibleTranslate;
    CsClefMap         mClefMap;

    //Loadable view settings
    CsPainterSettings mSettings;

    //Operative values
    int               mCurY;
    int               mRemarkTextHeight;
    int               mChordTextHeight;
    int               mLyricTextHeight;
    int               mTranslationTextHeight;

    CsReferenceList   mReferenceList;
    int               mLineIndex;
  public:
    CsPainter( QPainter *painter, const QString &keyViewSettings, const CsComposition &comp );

    QColor backgroundColor() const { return mSettings.mColorBackground; }

    int    drawLine( int y, int lineIndex, const CsLine &line );

  private:
    void   drawRemark( const QMap<QString,QString> &remarkMap );

    void   drawChord(const QMap<QString, CsChordLine> &chordMap );

    void   drawNote(const QMap<QString,CsNoteLine> &noteMap );

    void   drawLyric( const CsLyricList &lyricList );

    void   drawTranslation( const QMap<QString,QString> &translationMap );



    void   drawRemarkImpl( int x, int y, const QString &rem );

    void   drawChordImpl( int x, int y, const CsChordLine &chordLine );

    void   drawNoteImpl(int x, int y, int clef, const CsNoteLine &noteLine );

    void   drawTranslationImpl( int x, int y, const QString &tran );

    int    visualX( int x, int pos );

    int    fontHeight( int fontSize ) const;
  };

#endif // CSPAINTER_H
