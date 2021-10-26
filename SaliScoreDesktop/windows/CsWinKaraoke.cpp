#include "CsWinKaraoke.h"
#include "CsPainter.h"

#include <QPainter>
#include <QSettings>

CsWinKaraoke::CsWinKaraoke(CsComposition &comp, CsPlay &play, QWidget *parent) :
  CsWinScoreView( comp, play, parent )
  {

  }

void CsWinKaraoke::activate()
  {
  mPageMap.clear();
  mPlayer.reset();
  }




void CsWinKaraoke::paint()
  {
  QPainter painter(this);
  CsPainter cp( &painter, QStringLiteral(KEY_KARAOKE_SETTINGS), mComposition, mPlayer, 0, size() );

  if( mPageMap.isEmpty() ) {
    //Build page map
    QSettings s;
    int animationMode = camTwinLine; // s.value( QStringLiteral(KEY_KARAOKE_ANIMATION), 0 ).toInt();
    switch( animationMode ) {
      case camSingleLine : buildSingleLine( cp ); break;
      case camDoubleLine : buildDoubleLine( cp ); break;
      case camTwinLine   : buildTwinLine( cp ); break;
      }
    }

  //Fill background by background color
  cp.fillBackground();

  //How much tick elapsed from begin of current line
  int lineTickElapsed = mPlayer.lineTickElapsed();

  //Paint current page
  int currentLine = mPlayer.lineIndex();
  if( mPageMap.contains( currentLine ) )
    mPageMap.value( currentLine ).paint( lineTickElapsed, cp, mComposition );
  }




//!
//! \brief compositionChanged Called when composition changed outside
//!
void CsWinKaraoke::compositionChanged()
  {
  CsWinScoreView::compositionChanged();
  mPageMap.clear();
  update();
  }







void CsWinKaraoke::buildSingleLine( CsPainter &cp )
  {
  //Line height
  int lineHeight = cp.lineSongHeight();
  int posY = (size().height() - lineHeight) / 2;
  for( int lineIndex = 0; lineIndex < mComposition.lineCount(); lineIndex++ ) {
    const auto &line = mComposition.line(lineIndex);
    if( !line.isRemark() ) {
      //Bild line list
      CsKaraokeLineList lineList;
      CsKaraokeLine karaokeLine( posY, lineIndex );
      lineList.append( karaokeLine );

      //From list build page
      CsKaraokePage page( cklmSimple, lineList );

      //Insert page to map
      mPageMap.insert( lineIndex, page );
      }
    }
  }




void CsWinKaraoke::buildDoubleLine(CsPainter &cp)
  {
  //Line height
  int lineHeight = cp.lineSongHeight();
  int posY = (size().height() - lineHeight * 2) / 2;
  CsKaraokeLineList lineList;
  for( int lineIndex = 0; lineIndex < mComposition.lineCount(); lineIndex++ ) {
    const auto &line = mComposition.line(lineIndex);
    if( !line.isRemark() ) {
      //Bild line list
      CsKaraokeLine karaokeLine( posY + lineList.count() * lineHeight, lineIndex );
      lineList.append( karaokeLine );

      if( lineList.count() == 2 ) {
        //From list build page
        CsKaraokePage page( cklmSimple, lineList );

        //Insert page to map
        mPageMap.insert( lineList.at(0).mNewLine, page );
        mPageMap.insert( lineList.at(1).mNewLine, page );

        lineList.clear();
        }
      }
    }

  if( lineList.count() ) {
    //From list build page
    CsKaraokePage page( cklmSimple, lineList );

    //Insert page to map
    mPageMap.insert( lineList.at(0).mNewLine, page );
    }
  }



void CsWinKaraoke::buildTwinLine(CsPainter &cp)
  {
  //Line height
  int lineHeight = cp.lineSongHeight();
  int posY = (size().height() - lineHeight * 2) / 2;
  CsKaraokeLineList lineList;
  for( int lineIndex = 0; lineIndex < mComposition.lineCount(); lineIndex++ ) {
    const auto &line = mComposition.line(lineIndex);
    if( !line.isRemark() ) {
      //Bild line list
      CsKaraokeLine karaokeLine( posY + lineList.count() * lineHeight, lineIndex );
      lineList.append( karaokeLine );

      if( lineList.count() == 2 ) {
        //From list build page
        CsKaraokePage page( cklmSimple, lineList );

        //Insert page to map
        mPageMap.insert( lineList.at(0).mNewLine, page );

        lineList.clear();

        //Current line which is second line we set as first line for next page
        CsKaraokeLine karaokeLine( posY + lineList.count() * lineHeight, lineIndex );
        lineList.append( karaokeLine );
        }
      }
    }

  if( lineList.count() ) {
    //From list build page
    CsKaraokePage page( cklmSimple, lineList );

    //Insert page to map
    mPageMap.insert( lineList.at(0).mNewLine, page );
    }
  }
