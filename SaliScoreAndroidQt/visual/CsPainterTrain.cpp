#include "CsPainterTrain.h"

CsPainterTrain::CsPainterTrain(QPainter *painter, const QString &keyViewSettings, const CsComposition &comp, const CsCursorPosition *player, int offsetX, QSize size, int fragmentIndex) :
  CsPainter( painter, keyViewSettings, comp, player, offsetX, size, nullptr ),
  mShowFragment( fragmentIndex > 0 )
  {
  if( mShowFragment )
    mFragment = comp.trainGet( fragmentIndex - 1 );
  }


void CsPainterTrain::drawLineBackground(int lineHeight)
  {
  if( mShowFragment ) {
    if( mLineIndex < mFragment.mStart.lineIndex() || mLineIndex > mFragment.mStop.lineIndex() ) {
      //Downlight full line
      mPainter->setBrush( Qt::lightGray );
      mPainter->setPen( Qt::lightGray );
      mPainter->drawRect( mSettings.mLeftMenuSize - mOffsetX, mCurY, 2048 * mSettings.mPixelPerTakt >> 8, lineHeight );
      }
    else {
      if( mLineIndex == mFragment.mStart.lineIndex() ) {
        //Fragment started at current line
        mPainter->setBrush( Qt::lightGray );
        mPainter->setPen( Qt::lightGray );
        int x = mSettings.mLeftMenuSize - mOffsetX;
        mPainter->drawRect( x, mCurY, mLeftGap - x + (mFragment.mStart.linePosition() * mSettings.mPixelPerTakt >> 8), lineHeight );
        }
      if( mLineIndex == mFragment.mStop.lineIndex() ) {
        //Fragment finished at current line
        mPainter->setBrush( Qt::lightGray );
        mPainter->setPen( Qt::lightGray );
        mPainter->drawRect( visualX(mLeftGap,mFragment.mStop.linePosition()), mCurY, (2048 - mFragment.mStop.linePosition()) * mSettings.mPixelPerTakt >> 8, lineHeight );
        }
      }
    }
  }
