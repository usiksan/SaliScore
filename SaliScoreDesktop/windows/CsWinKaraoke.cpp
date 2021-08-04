#include "CsWinKaraoke.h"
#include "CsPainter.h"

#include <QPainter>
#include <QSettings>

CsWinKaraoke::CsWinKaraoke(CsComposition &comp, CsPlay &play, QWidget *parent) :
  CsWinScoreMode( comp, play, parent )
  {

  }

void CsWinKaraoke::activate()
  {
  mPageMap.clear();
  mPlayer.reset();
  }



void CsWinKaraoke::paintEvent(QPaintEvent *event)
  {
  QPainter painter(this);
  CsPainter cp( &painter, QStringLiteral(KEY_KARAOKE_SETTINGS), mComposition, mPlayer );

  if( mPageMap.isEmpty() ) {
    //Build page map
    QSettings s;
    int animationMode = s.value( QStringLiteral(KEY_KARAOKE_ANIMATION), 0 ).toInt();
    switch( animationMode ) {
      case camSingleLine : buildSingleLine( cp );
      }
    }

  //Fill background by background color
  painter.fillRect( QRect( QPoint(), size() ), cp.backgroundColor() );

  //How much tick elapsed from begin of current line
  int lineTickElapsed = mPlayer.lineTickElapsed();

  //Paint current page
  int currentLine = mPlayer.lineIndex();
  if( mPageMap.contains( currentLine ) )
    mPageMap.value( currentLine ).paint( lineTickElapsed, cp, mComposition );
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
