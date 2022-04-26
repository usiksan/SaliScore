#include "CsVisualWinMain.h"
#include "score/CsChordToNote.h"

CsVisualWinMain::CsVisualWinMain(QWidget *parent) :
  QMainWindow{parent}
  ,mTickCount(0)     //! Current time tick value
  ,mTickStep(0)      //! Current time tick step for playback
  ,mTrainIndex(-1)    //!< Current train fragment index
  ,mLineIndex(0)     //!< Current train line index
  ,mLinePosition(0)  //!< Current position in line
  ,mLineTickCount(0)
  ,mIsRun(false)         //!< If true then player in run state
  ,mIsPaused(false)      //!< If true then player paused
  ,mIsTraining(false)    //!< true if teaching process actived
  {
  //Timer for periodic tick generation
  connect( &mTimer, &QTimer::timeout, this, &CsVisualWinMain::periodic );
  mTimer.start( TICK_PERIOD_MS );
  }




void CsVisualWinMain::setTempo(int tempo)
  {
  tempo = qBound( 16, tempo, 480 );
  //time of tick in us 4 * 60000000 / bpm / 256
  quint64 usPerTick = (4 * 60'000'000 / tempo) >> 8;
  quint64 temp = TICK_PERIOD_US * 256;
  mTickStep = temp / usPerTick;
  mTickCount = 0;
  qDebug() << "mTickStep" << mTickStep;
  }



void CsVisualWinMain::cmPlayRun()
  {
  mIsPaused = false;
  mIsTraining = false;
  mIsRun = true;
  emit playSetRun( mIsRun );
  prepareRun();
  }



void CsVisualWinMain::cmPlayTrain()
  {
  mIsPaused = false;
  mIsTraining = true;
  mIsRun = true;
  emit playSetRun( mIsRun );
  prepareRun();
  }




void CsVisualWinMain::cmPlayPause()
  {
  mIsPaused = !mIsPaused;
  emit playSetPause(mIsPaused);
  }




void CsVisualWinMain::cmPlayStop()
  {
  mIsRun = false;
  emit playSetRun( mIsRun );
  mIsPaused = false;
  emit playSetPause(mIsPaused);
  }



void CsVisualWinMain::midiNoteOn(int pitch)
  {
  int keyEvent = csKeyEvent( 0, pitch );
  if( mKeyEventFromPlayer.contains(keyEvent) ) {
    //Player is waiting for this note
    mKeyEventFromPlayer.remove(keyEvent);
    }
  else {
    mKeyEventFromKeyboard.insert(keyEvent);
    }
  }



void CsVisualWinMain::midiNoteOff(int pitch)
  {
  int keyEvent = csKeyEvent( 0, pitch );
  if( mKeyEventFromKeyboard.contains(keyEvent) )
    mKeyEventFromKeyboard.remove(keyEvent);
  }



void CsVisualWinMain::periodic()
  {
  mTickCount += mTickStep;
  tick( mTickCount >> 8 );
  mTickCount &= 0xff;

  static int divider = 0;
  if( ++divider > 9 ) {
    divider = 0;
    playUpdate();
    }
  }



void CsVisualWinMain::tick(int tickOffset)
  {
  if( tickOffset > 0 ) {
    //Process all playing notes
    for( int i = mEventList.count() - 1; i >= 0; i-- )
      if( mEventList[i].tick( tickOffset ) ) {
        //Stop note playing
        emit noteOff( mEventList.at(i).channel(), mEventList.at(i).pitch() );
        //Remove current event from playing list
        mEventList.removeAt(i);
        }

    //Process all waiting notes
    bool fail = false;
    if( mIsTraining ) {
      for( int i = mWaitingList.count() - 1; i >= 0; i-- )
        if( mWaitingList[i].tick( tickOffset ) ) {
          //Time for press note ended
          if( mKeyEventFromPlayer.contains( mWaitingList.at(i).keyEvent() )  )
            //Note still waiting for pressing
            fail = true;
          else mWaitingList.removeAt(i);
          }
      }

    if( !fail && mIsRun && !mIsPaused ) {
      //Extract next notes
      int interval = qMin( tickOffset, mLineTickCount - mLinePosition );
      extractLine( mLinePosition + interval );
      mLinePosition += interval;
      if( mLinePosition >= mLineTickCount ) {
        //We reach end of line and must find next line to play
        mLinePosition = 0;
        //Find next line
        findNextLine();
        interval = tickOffset - interval;
        extractLine( mLinePosition + interval );
        mLinePosition += interval;
        }

      //Check if we reach end of fragment
      if( mComposition.trainGet( mTrainIndex ).mStop.isLess( mLineIndex, mLinePosition ) ) {
        mIsRun = false;
        emit playSetRun( mIsRun );
        }
      }
    }
  }



void CsVisualWinMain::extractLine(int lastPosition)
  {
  const CsLine &line = mComposition.line( mLineIndex );

  //Chord processing
  const CsDefList chordDefList( mComposition.chordDefList() );
  for( auto const &def : qAsConst(chordDefList) )
    extractChords( line, def, lastPosition );


  //Note processing
  const CsDefList noteDefList( mComposition.noteDefList() );
  for( auto const &def : qAsConst(noteDefList) )
    extractNote( line, def, lastPosition );
  }





void CsVisualWinMain::extractChords(const CsLine &line, const CsDefinition &def, int lastPosition)
  {
  if( def.mSynthes || def.mTrain ) {
    //Perform extraction
    const auto &list = line.chordListGet( def.mName );
    for( auto &chord : list ) {
      if( chord.isBeginInside( mLinePosition, lastPosition ) ) {
        //This chord hit in current interval - append chord to array
        if( def.mSynthes ) {
          //Append chord to synthes array
          CsChordToNote chordToNote( chord.note(), chord.chord() );
          for( int i = 0; i < chordToNote.count(); i++ )
            mEventList.append( CsPlayerEvent( def.channel(), chordToNote.pitch(i), chord.duration() ) );
          }
        if( def.mTrain ) {
          //Append chord to teach
          }
        }
      }
    }
  }





void CsVisualWinMain::extractNote(const CsLine &line, const CsDefinition &def, int lastPosition)
  {
  if( def.mSynthes || def.mTrain ) {
    //Perform extraction
    const auto &list = line.noteListGet( def.mName );
    for( auto &note : list ) {
      if( note.isBeginInside( mLinePosition, lastPosition ) ) {
        //This chord hit in current interval - append chord to array
        CsPlayerEvent noteEvent( def.channel(), note.pitch(), note.duration() );
        if( def.mSynthes ) {
          //Append chord to synthes array
          }
        if( def.mTrain ) {
          //Append chord to teach
          int keyEvent = noteEvent.keyEvent();
          if( mKeyEventFromKeyboard.contains(keyEvent) )
            mKeyEventFromKeyboard.remove( keyEvent );
          else {
            mWaitingList.append( noteEvent );
            mKeyEventFromPlayer.insert( keyEvent );
            }
          }
        else {
          //Append note to event list
          mEventList.append( noteEvent );
          emit noteOn( noteEvent.channel(), noteEvent.pitch() );
          }
        }
      }
    }
  }



void CsVisualWinMain::findNextLine()
  {
  for( int index = mLineIndex + 1; index < mComposition.lineCount(); index++ ) {
    const auto &line = mComposition.line( index );
    if( !line.isRemark() ) {
      //Next song line found
      qDebug() << "CsPlay:new line" << index;
      mLineIndex = index;
      mLineTickCount = line.taktCount() * mComposition.tickPerTakt();
      return;
      }
    }
  //Next line not found, composition is finished
  mIsRun = false;
  emit playSetRun( mIsRun );
  }




void CsVisualWinMain::prepareRun()
  {
  CsTrainInterval ti = mComposition.trainGet( mTrainIndex );
  mLineIndex = ti.mStart.lineIndex() - 1;
  findNextLine();
  if( mIsRun ) {
    setTempo( mComposition.attributeGet(CS_ATTR_TEMPO).toInt() );
    mLinePosition = ti.mStart.linePosition();
    mWaitingList.clear();
    mKeyEventFromKeyboard.clear();
    mKeyEventFromPlayer.clear();
    }
  }
