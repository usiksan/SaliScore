/*
Project "SaliScore Score music edit, view and tutorial program"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Basic painter for score paint
*/
#ifndef CSPAINTER_H
#define CSPAINTER_H

#include "CsConfig.h"
#include "CsPainterSettings.h"
#include "CsCellCursor.h"
#include "score/CsLyricSymbol.h"
#include "score/CsLine.h"
#include "score/CsComposition.h"
#include "score/CsReference.h"

#include <QPainter>
#include <QStringList>

//!
//! \brief The CsLyricDisposition struct represents disposition of each symbol in lyric line
//!
struct CsLyricDisposition {
    int  mWidth;     //!< Width of symbol in pixels
    int  mPosX;      //!< Visual position of symbol in pixels
    bool mHighlight; //!< Flag defines highlighting this position

    int  after() const { return mPosX + mWidth; }
  };


struct CsTextHeight {
    int mHeight; //!< Full height of text line
    int mOffset; //!< Vertical offset text
  };


class CsPainter
  {
  protected:
    QPainter               *mPainter;
    const CsCursorPosition *mPlayer;

    //Composition settings
    const CsDefList        &mDefList;
    QString                 mNumerator;
    QString                 mDenominator;

    //Loadable view settings
    CsPainterSettings       mSettings;

    //Operative values
    int               mCurY;
    CsTextHeight      mRemarkTextHeight;
    CsTextHeight      mChordTextHeight;
    CsTextHeight      mLyricTextHeight;
    CsTextHeight      mTranslationTextHeight;
    CsTextHeight      mTitleHeight;
    CsTextHeight      mPropertiesHeight;
    int               mLeftGap;
    int               mClefPos;
    int               mDenominatorPos;

    int               mLineStartOffset;        //!< Offset of line start in tick

    int               mTickPerTakt;

    int               mStepChord;
    int               mStepNote;
    int               mStepLyric;

    int               mStepPixChord;
    int               mStepPixNote;
    int               mStepPixLyric;

    CsReferenceList   mReferenceList;    //!< Builded by painter reference list for mouse positioning on score
    int               mLineIndex;
    CsCellCursor     *mCellCursor;

    //To support x scroll
    int               mOffsetX;
    QSize             mSize;                   //!< Drawable area size

    QRect             mCellCursorRect;         //!< Current cell rectangle
    QLine             mPlayerLine;             //!< Current play position
  public:
    CsPainter( QPainter *painter, const QString &keyViewSettings, const CsComposition &comp, const CsCursorPosition *player, int offsetX, QSize size, CsCellCursor *cellCursor = nullptr );

    QPainter        *painter() { return mPainter; }

    CsReferenceList  referenceList() const { return mReferenceList; }

    virtual void     fillBackground();

    //!
    //! \brief cellCursorRect Returns current cursor cell position rect
    //! \return               Current cursor cell position rect
    //!
    QRect            cellCursorRect() const { return mCellCursorRect; }

    QLine            playerLine() const { return mPlayerLine; }

    int              drawTitleAndProperties( int y, const CsComposition &comp );

    int              drawLine(int y, int lineIndex, const CsLine &line , bool fullDrawing = false );

    virtual void     drawLineBackground( int lineHeight ) { Q_UNUSED(lineHeight) }
  protected:
    virtual bool isNotEditProperty(const QString &propertyId, int x, int y );

    virtual bool isNotEditRemark( const QString &part, int x, int y );

    virtual bool isNotEditChord( const QString &part, int position, int x, int y );

    //!
    //! \brief isNotEditNote Test if this position not edited
    //! \param part          Part of score line
    //! \param position      Time position inside line
    //! \param x             x position of note in pixels
    //! \param scoreY        y position of score line
    //! \param noteStart     base note of score line (clef)
    //! \return              true if this position not edited
    //!
    virtual bool isNotEditNote(const QString &part, int position, int x, int scoreY, int noteStart);

    virtual bool isNotEditLyric( QVector<CsLyricDisposition> &disposition );

    virtual bool isNotEditTranslation( const QString &part, int x, int y );

    //!
    //! \brief disableDrawActiveNote Disables drawing active note
    //! \return                      true drawing active note disabled
    //!
    virtual bool disableDrawActiveNote() const { return true; }

    //!
    //! \brief drawNoteSingle Draws single note
    //! \param x              x position of note in pixel
    //! \param scoreY         y position of note score line
    //! \param noteStart      note of bottom score line
    //! \param noteWhite      note white key index (black notes display with sharp)
    //! \param noteDuration   note duration
    //! \param noteDies       note dies
    //! \param opacity        opacity of display
    //! \return               rectangle of note
    //!
    QRect  drawNoteSingle(int x, int scoreY, int noteStart, int noteWhite, int noteDuration, bool noteDies , bool opacity = false);

    void   buildDisposition( QVector<CsLyricDisposition> &disposition, const CsLyricLine &lyricLine );

    int    visualX( int x, int pos );

  private:
    void         drawLine( const CsLine &line );

    void         drawRemarkLine( const CsLine &line, const CsDefinition &def );

    void         drawLyric(const CsLyricLine &lyricLine );

    void         drawPlayPosition(int markHeight);



    void         drawRemarkImpl( int x, int y, const QString &rem );

    void         drawChordImpl(const QString &part, const QList<CsChord> &chordList );

    void         drawNoteImpl(int clef, int taktCount, const QString &part, const QList<CsNote> &noteList );

    void         drawTranslationImpl( int x, int y, const QString &tran );

    void         drawPropertyImpl(int xorigin, int xtab, const QString &title, const QString &value, const QString &propertyId );

    void         drawTaktLines( int taktCount, int y0, int y1 );

    CsTextHeight fontHeight( int fontSize ) const;

    bool         isPlayerOnCurrentLine() const { return mPlayer != nullptr && mPlayer->lineIndex() == mLineIndex; }

    bool         isPlayerHighlight( int position, int duration ) const;

    bool         isPlayerHighlight( const CsPosition &p ) const { return isPlayerHighlight( p.position(), p.duration() ); }


    void         drawCellProperty(int x, int y, const QString &value, int height, const QString &propertyId );

    int          drawCellText( int x, int y, const QString &value, int height, bool isCurrent );

    void         drawCellChord( int y , int tickCount, const QString &part );

    void         drawCellNote( int y, int tickCount, const QString &part );

    void         drawCellLyric( int y, int tickCount );

    void         drawCell( int x, int y, int width, int height, bool isCurrent );
  };

#endif // CSPAINTER_H
