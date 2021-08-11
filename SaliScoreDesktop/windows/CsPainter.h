#ifndef CSPAINTER_H
#define CSPAINTER_H

#include "CsConfig.h"
#include "CsPainterSettings.h"
#include "CsCellCursor.h"
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
  protected:
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

    int               mStepChord;
    int               mStepNote;
    int               mStepLyric;

    int               mStepPixChord;
    int               mStepPixNote;
    int               mStepPixLyric;

    CsReferenceList   mReferenceList;
    int               mLineIndex;
    CsCellCursor     *mCellCursor;

    //To support x scroll
    int               mOffsetX;
    QSize             mSize;                   //!< Drawable area size

    QRect             mCellCursorRect;         //!< Current cell rectangle
  public:
    CsPainter( QPainter *painter, const QString &keyViewSettings, const CsComposition &comp, const CsPlay &player, int offsetX, QSize size, CsCellCursor *cellCursor = nullptr );

    QPainter        *painter() { return mPainter; }

    CsReferenceList  referenceList() const { return mReferenceList; }

    QColor           backgroundColor() const { return mSettings.mColorBackground; }

    QRect            cellCursorRect() const { return mCellCursorRect; }

    int              drawTitleAndProperties( int y, const CsComposition &comp );

    int              drawLine(int y, int lineIndex, const CsLine &line , bool fullDrawing = false );

    int              lineRemarkHeight() const;

    int              lineSongHeight() const;

  protected:
    virtual bool isNotEditProperty( int propertyId, int x, int y );

    virtual bool isNotEditRemark( const QString &part, int x, int y );

    virtual bool isNotEditChord( const QString &part, int position, int x, int y );

    virtual bool isNotEditNote( const QString &part, int position, int x, int y );

    virtual bool isNotEditLyric( int position, int x, int y );

    virtual bool isNotEditTranslation( const QString &part, int x, int y );

  private:
    void   drawRemark( const QMap<QString,QString> &remarkMap );

    void   drawChord(int taktCount, const QMap<QString, CsChordLine> &chordMap );

    void   drawNote( int taktCount, const QMap<QString,CsNoteLine> &noteMap );

    void   drawLyric( const CsLyricList &lyricList );

    void   drawTranslation( const QMap<QString,QString> &translationMap );

    void   drawPlayPosition(int markHeight);



    void   drawRemarkImpl( int x, int y, const QString &rem );

    void   drawChordImpl(const QString &part, const CsChordLine &chordLine );

    void   drawNoteImpl(int clef, int taktCount, const CsNoteLine &noteLine );

    void   drawTranslationImpl( int x, int y, const QString &tran );

    void   drawPropertyImpl(int xorigin, int xtab, const QString &title, const QString &value, int propertyId );

    void   drawTaktLines( int taktCount, int y0, int y1 );

    int    visualX( int x, int pos );

    int    fontHeight( int fontSize ) const;

    bool   isPlayerOnCurrentLine() const { return mPlayer.isShow() && mPlayer.lineIndex() == mLineIndex; }

    bool   isHighlight( int position, int duration ) const;


    void   drawCellProperty(int x, int y, const QString &value, int height, int propertyId );

    void   drawCellText( int x, int y, const QString &value, int height, bool isCurrent );

    void   drawCellChord( int y , int tickCount, const QString &part );

    void   drawCellNote( int y, int tickCount, const QString &part );

    void   drawCellLyric( int y, int tickCount );

    void   drawCell( int x, int y, int width, int height, bool isCurrent );
  };

#endif // CSPAINTER_H
