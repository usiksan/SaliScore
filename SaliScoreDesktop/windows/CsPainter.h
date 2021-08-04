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
#include "play/CsPlay.h"

#include <QPainter>
#include <QStringList>

class CsPainter
  {
    QPainter         *mPainter;
    const CsPlay     &mPlayer;

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
    int               mTitleHeight;
    int               mPropertiesHeight;
    int               mLeftGap;
    int               mClefPos;
    int               mDenominatorPos;

    CsReferenceList   mReferenceList;
    int               mLineIndex;
  public:
    CsPainter( QPainter *painter, const QString &keyViewSettings, const CsComposition &comp, const CsPlay &player );

    QPainter *painter() { return mPainter; }

    QColor    backgroundColor() const { return mSettings.mColorBackground; }

    int       drawTitleAndProperties( int y, QSize size, const CsComposition &comp );

    int       drawLine( int y, int lineIndex, const CsLine &line );

    int       lineRemarkHeight() const;

    int       lineSongHeight() const;

  private:
    void   drawRemark( const QMap<QString,QString> &remarkMap );

    void   drawChord(int taktCount, const QMap<QString, CsChordLine> &chordMap );

    void   drawNote( int taktCount, const QMap<QString,CsNoteLine> &noteMap );

    void   drawLyric( const CsLyricList &lyricList );

    void   drawTranslation( const QMap<QString,QString> &translationMap );



    void   drawRemarkImpl( int x, int y, const QString &rem );

    void   drawChordImpl(const CsChordLine &chordLine );

    void   drawNoteImpl(int clef, int taktCount, const CsNoteLine &noteLine );

    void   drawTranslationImpl( int x, int y, const QString &tran );

    void   drawPropertyImpl( int xtab, const QString &title, const QString &value );

    void   drawTaktLines( int taktCount, int y0, int y1 );

    int    visualX( int x, int pos );

    int    fontHeight( int fontSize ) const;

    bool   isPlayerOnCurrentLine() const { return mPlayer.isShow() && mPlayer.lineIndex() == mLineIndex; }

    bool   isHighlight( int position, int duration ) const;
  };

#endif // CSPAINTER_H
