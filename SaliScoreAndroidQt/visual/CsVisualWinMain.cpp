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
  emit playRun( mIsRun );
  prepareRun();
  }



void CsVisualWinMain::cmPlayTrain()
  {
  mIsPaused = false;
  mIsTraining = true;
  mIsRun = true;
  emit playRun( mIsRun );
  prepareRun();
  }




void CsVisualWinMain::cmPlayPause()
  {
  mIsPaused = !mIsPaused;
  emit playPause(mIsPaused);
  }




void CsVisualWinMain::cmPlayStop()
  {
  mIsRun = false;
  emit playRun( mIsRun );
  mIsPaused = false;
  emit playPause(mIsPaused);
  }



void CsVisualWinMain::midiNote(int pitch, int velo)
  {
  int keyEvent = csKeyEvent( 0, pitch );
  if( velo == 0 ) {
    //Note off, remove it from keyboard event set
    if( mKeyEventFromKeyboard.contains(keyEvent) )
      mKeyEventFromKeyboard.remove(keyEvent);
    }
  else {
    //Note on
    }
  if( mKeyEventFromPlayer.contains(keyEvent) ) {
    //Player is waiting for this note
    mKeyEventFromPlayer.remove(keyEvent);
    emit playHighlight( csKeyEventChannel(keyEvent), csKeyEventPitch( keyEvent ), 0 );
    }
  else {
    mKeyEventFromKeyboard.insert(keyEvent);
    }
  }



void CsVisualWinMain::midiRun(bool run)
  {
  if( run ) cmPlayRun();
  else      cmPlayStop();
  }




void CsVisualWinMain::midiVoice(int voice)
  {
  //Set selected voice to composition voice
  mComposition.attributeSet( CS_ATTR_VOICE, QStringLiteral("%1.%2").arg(voice >> 8).arg(voice & 0xff) );
  playUpdate();
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
        emit playNote( mEventList.at(i).channel(), mEventList.at(i).pitch(), 0 );
        emit playHighlight( mEventList.at(i).channel(), mEventList.at(i).pitch(), 0 );
        //Remove current event from playing list
        mEventList.removeAt(i);
        }

    //Process all waiting notes
    bool fail = false;
    for( int i = mWaitingList.count() - 1; i >= 0; i-- )
      if( mWaitingList[i].tick( tickOffset ) ) {
        //Time for press note ended
        if( mIsTraining ) {
          //When we training
          if( mKeyEventFromPlayer.contains( mWaitingList.at(i).keyEvent() )  )
            //Note still waiting for pressing
            fail = true;
          else {
            emit playHighlight( mWaitingList.at(i).channel(), mWaitingList.at(i).pitch(), 0 );
            mWaitingList.removeAt(i);
            }
          }
        else {
          //This is simple playback, so we remove event from waiting list
          mKeyEventFromPlayer.remove( mWaitingList.at(i).keyEvent() );
          emit playHighlight( mWaitingList.at(i).channel(), mWaitingList.at(i).pitch(), 0 );
          mWaitingList.removeAt(i);
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
        emit playRun( mIsRun );
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
          //Append note to teach
          int keyEvent = noteEvent.keyEvent();
          if( mKeyEventFromKeyboard.contains(keyEvent) )
            mKeyEventFromKeyboard.remove( keyEvent );
          else {
            mWaitingList.append( noteEvent );
            mKeyEventFromPlayer.insert( keyEvent );
            emit playHighlight( noteEvent.channel(), noteEvent.pitch(), 127 );
            }
          }
        else {
          //Append note to event list
          mEventList.append( noteEvent );
          emit playNote( noteEvent.channel(), noteEvent.pitch(), 127 );
          emit playHighlight( noteEvent.channel(), noteEvent.pitch(), 127 );
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
  emit playRun( mIsRun );
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
