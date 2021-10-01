#include "CsImportText.h"
#include "score/CsNoteChord.h"
#include "score/CsChord.h"

#include <QObject>

//inline int packChord( int note, int chord ) { return (note << 8) + chord;}

//inline int unpackNote( int pk ) { return (pk >> 8); }
//inline int unpackChord( int pk ) { return (pk & 0xff); }

//bool chordParser( const QString name, int &note, int &chord )
//  {

//  }


CsImportText::CsImportText() :
  CsImport( {QString(".txt")}, QObject::tr("Simple text aligned") )
  {

  //Build chord map
  mChordMap.insert( QStringLiteral("maj6"), chordMaj6 ); //  "maj6",   // chordMaj6,     Maj6,    //0,4,7,9
  mChordMap.insert( QStringLiteral("6"), chordMaj6 ); //  "maj6",   // chordMaj6,     Maj6,    //0,4,7,9
  mChordMap.insert( QStringLiteral("maj7"), chordMaj7 ); //  "maj7",   // chordMaj7,     Maj7,     //0,4,7,11
  mChordMap.insert( QStringLiteral("7maj"), chordMaj7 ); //  "maj7",   // chordMaj7,     Maj7,     //0,4,7,11
  mChordMap.insert( QStringLiteral("M7"), chordMaj7 ); //  "maj7",   // chordMaj7,     Maj7,     //0,4,7,11
  mChordMap.insert( QStringLiteral("7#11"), chordMaj7sharp11 ); //"7#11",   // chordMaj7sharp11,  M7#11, //0,4,6,7,11
  mChordMap.insert( QStringLiteral("add9"), chordMaj_9_ ); //"add9",   // chordMaj_9_,   Madd9,    //0,2,4,7
  mChordMap.insert( QStringLiteral("9"), chordMaj_9_ ); //"add9",   // chordMaj_9_,   Madd9,    //0,2,4,7
  mChordMap.insert( QStringLiteral("7(9)"), chordMaj7_9_ ); //"7(9)",   // chordMaj7_9_,  M7(9),    //0,2,4,7,11
  mChordMap.insert( QStringLiteral("6(9)"), chordMaj6_9_ ); //"6(9)",   // chordMaj6_9_,  M6(9),    //0,2,4,7,9
  mChordMap.insert( QStringLiteral("aug"), chordAug ); //"aug",       // chordAug,   aug,       //0,4,8
  mChordMap.insert( QStringLiteral("m"), chordMin ); //"m",      // chordMin,      m,    //0,3,7
  mChordMap.insert( QStringLiteral("m6"), chordMin6 ); //"m6",     // chordMin6,     m6,    //0,3,7,9
  mChordMap.insert( QStringLiteral("m7"), chordMin7 ); //"m7",     // chordMin7,     m7,    //0,3,7,10
  mChordMap.insert( QStringLiteral("m7b5"), chordMin7b5 ); //"m7b5",   // chordMin7b5,   m7b5,    //0,3,6,10
  mChordMap.insert( QStringLiteral("m(9)"), chordMin_9_ ); //"m(9)",   // chordMin_9_,   m(9),    //0,2,3,7
  mChordMap.insert( QStringLiteral("m7(9)"), chordMin7_9_ ); //"m7(9)",  // chordMin7_9_,  m7(9),    //0,2,3,7,10
  mChordMap.insert( QStringLiteral("m7(11)"), chordMin7_11_ ); //"m7(11)", // chordMin7_11_, m7(11),    //0,3,5,7,10
  mChordMap.insert( QStringLiteral("mM7"), chordMinMaj7 ); //"mM7",    // chordMinMaj7,  mM7,    //0,3,7,11
  mChordMap.insert( QStringLiteral("m7maj"), chordMinMaj7 ); //"mM7",    // chordMinMaj7,  mM7,    //0,3,7,11
  mChordMap.insert( QStringLiteral("mM7(9)"), chordMinMaj7_9_ ); //"mM7(9)", // chordMinMaj7_9_, mM7(9),  //0,2,3,7,11
  mChordMap.insert( QStringLiteral("dim"), chordDim ); //"dim",    // chordDim,      dim,    //0,3,6
  mChordMap.insert( QStringLiteral("dim7"), chordDim7 ); //"dim7",   // chordDim7,     dim7,    //0,3,6,9
  mChordMap.insert( QStringLiteral("7"), chord7th );   //"7",      // chord7th,      7,    //0,4,7,10
  mChordMap.insert( QStringLiteral("7sus4"), chord7sus4 ); //"7sus4",  // chord7sus4,    7sus4,    //0,5,7,10
  mChordMap.insert( QStringLiteral("7b5"), chord7b5 ); //"7b5",    // chord7b5,      7b5,    //0,4,6,10
                                                       //"7(9)",   // chord7_9_,     7(9),    //0,2,4,7,10
  mChordMap.insert( QStringLiteral("7(#11)"), chord7sharp11 ); //"7(#11)", // chord7sharp11, 7(#11),    //0,4,6,7,10
  mChordMap.insert( QStringLiteral("7(13)"), chord7_13_ ); //"7(13)",  // chord7_13_,    7(13),    //0,4,7,9,10
  mChordMap.insert( QStringLiteral("7(b9)"), chord7_b9_ ); //"7(b9)",  // chord7_b9_,    7(b9),    //0,1,4,7,10
  mChordMap.insert( QStringLiteral("7(b13)"), chord7_b13_ ); //"7(b13)", // chord7_b13_,   7(b13),    //0,4,7,8,10
  mChordMap.insert( QStringLiteral("7(#9)"), chord7_sharp9_ ); //"7(#9)",  // chord7_sharp9_,7(#9),    //0,3,4,7,10
  mChordMap.insert( QStringLiteral("M7aug"), chordMaj7aug ); //"M7aug",  // chordMaj7aug,  M7aug,    //0,4,8,11
  mChordMap.insert( QStringLiteral("7aug"), chord7aug );  //"7aug",   // chord7aug,     7aug,    //0,4,8,10
  mChordMap.insert( QStringLiteral("1+8"), chord1plus8 ); //"1+8",    // chord1plus8,   1+8,    //0
  mChordMap.insert( QStringLiteral("1+5"), chord1plus5 ); //"1+5",    // chord1plus5,   1+5,    //0,7
  mChordMap.insert( QStringLiteral("sus4"), chordSus4 ); //"sus4",   // chordSus4,     sus4    //0,5,7
  mChordMap.insert( QStringLiteral("1+2+5"), chord1plus2plus5 ); //"1+2+5"   // chord1plus2plus5, 1+2+5 //0,2,7


//  mChordMap.insert( QStringLiteral(""), chord );


  }




bool CsImportText::probe(const QByteArray &fileContent)
  {
  return true;
  }



CsComposition CsImportText::read(const QByteArray &fileContent, bool &ok)
  {
  //Convert to string
  QString fileString = QString::fromUtf8( fileContent );

  //Separate to line list
  QStringList lineList = fileString.split( QChar('\n') );
  if( lineList.count() < 3 ) {
    ok = false;
    return CsComposition{};
    }

  CsComposition comp;
  comp.tickPerPartSet( duraQuarter );
  comp.partPerTaktSet( 4 );
  comp.remarkAppend( QStringLiteral("input"), QStringLiteral("Source file") );
  comp.chordAppend( QStringLiteral("chords"), QStringLiteral("Detected chords") );

  //Scan all lines and classify them to remark, chord or lyric
  //Lyric lets nearly after chord
  bool wasChord = false;
  QString chordLine;

  for( const auto &line : lineList ) {
    //Append source line as remark
    comp.remarkSet( comp.lineAppend(true), QStringLiteral("input"), line );

    if( wasChord ) {
      //Previous line was a chord line
      wasChord = false;
      //This line is song lyric all none
      if( line.isEmpty() || chordProbe(line) ) {
        //Place previous detected chords into composition
        CsChordList chordList;

        //Decomposite line to chords
        QStringList chordNameList = chordLine.split( QChar(' '), Qt::SkipEmptyParts );

        int cNote, cChord, cPos = 0;
        for( const auto &chordName : chordNameList )
          if( chordParser( chordName, cNote, cChord ) ) {
            chordList.append( CsChord( cPos, duraHole, cNote, cChord ) );
            cPos += duraHole;
            }

        comp.chordListSet( comp.lineAppend(false), QStringLiteral("chords"), chordList );
        }
      else {
        //Lyric line
        CsChordList chordList;
        CsLyricLine lyricLine;
        int i;
        int lyricPartStart = 0;
        int pos = 0;
        for( i = 0; i < chordLine.count(); i++ ) {
          if( chordLine.at(i).isSpace() ) continue;

          //At i position there is chord

          //Append lyric part
          if( lyricPartStart < line.count() && i != lyricPartStart ) {
            int lyricPartCount = qMin( i, line.count()) - lyricPartStart;
            for( int k = 0; k < lyricPartCount; k++ )
              lyricLine.append( CsLyricSymbol( line.at(lyricPartStart + k) ) );
            lyricLine.append( CsLyricSymbol(256) );
            }

          lyricPartStart = i;

          //Retrive chord
          QString chordName;
          chordName.reserve(8);
          for( ; i < chordLine.count() && !chordLine.at(i).isSpace(); i++ )
            chordName.append( chordLine.at(i) );

          //Detect chord
          int cNote = 0, cChord = chordCancel;
          chordParser( chordName, cNote, cChord );

          //Add chord
          pos += 256;
          chordList.append( CsChord( pos, duraHole, cNote, cChord ) );
          }

        //Finish lyric
        if( lyricPartStart < line.count() ) {
          int lyricPartCount = line.count() - lyricPartStart;
          for( int k = 0; k < lyricPartCount; k++ )
            lyricLine.append( CsLyricSymbol( line.at(lyricPartStart + k) ) );
          }

        int ln = comp.lineAppend(false);
        comp.chordListSet( ln, QStringLiteral("chords"), chordList );
        comp.lyricSet( ln, lyricLine );
        }
      }
    if( chordProbe(line) ) {
      //Chord line detected
      chordLine = line;
      wasChord = true;
      }
    //Simple remark, nothing do
    }

  ok = true;
  return comp;
  }




bool CsImportText::chordParser(const QString name, int &note, int &chord) const
  {
  //Note detect
  QString chordName;
  //At first must be more lenght sentences
  if( name.startsWith( QStringLiteral("C#") ) || name.startsWith( QStringLiteral("Db") ) ) {
      note = noteCd;
      chordName = name.mid( 2 );
      }
  else if( name.startsWith( QStringLiteral("C") ) ) {
    note = noteC;
    chordName = name.mid( 1 );
    }
  else if( name.startsWith( QStringLiteral("D#") ) || name.startsWith( QStringLiteral("Eb") ) ) {
    note = noteDd;
    chordName = name.mid( 2 );
    }
  else if( name.startsWith( QStringLiteral("D") ) ) {
    note = noteD;
    chordName = name.mid( 1 );
    }
  else if( name.startsWith( QStringLiteral("E") ) ) {
    note = noteE;
    chordName = name.mid( 1 );
    }
  else if( name.startsWith( QStringLiteral("F#") ) || name.startsWith( QStringLiteral("Gb") ) ) {
    note = noteFd;
    chordName = name.mid( 2 );
    }
  else if( name.startsWith( QStringLiteral("F") ) ) {
    note = noteF;
    chordName = name.mid( 1 );
    }
  else if( name.startsWith( QStringLiteral("G#") ) || name.startsWith( QStringLiteral("Ab") ) ) {
    note = noteGd;
    chordName = name.mid( 2 );
    }
  else if( name.startsWith( QStringLiteral("G") ) ) {
    note = noteG;
    chordName = name.mid( 1 );
    }
  else if( name.startsWith( QStringLiteral("A#") ) || name.startsWith( QStringLiteral("Bb") ) ) {
    note = noteAd;
    chordName = name.mid( 2 );
    }
  else if( name.startsWith( QStringLiteral("A") ) ) {
    note = noteA;
    chordName = name.mid( 1 );
    }
  else if( name.startsWith( QStringLiteral("B") ) ) {
    note = noteB;
    chordName = name.mid( 1 );
    }
  else return false;

  //Chord modification
  if( chordName.isEmpty() ) {
    chord = chordMaj;
    return true;
    }

  if( mChordMap.contains(chordName) ) {
    chord = mChordMap.value(chordName);
    return true;
    }

  return false;
  }




bool CsImportText::chordProbe(const QString &line) const
  {
  if( line.isEmpty() )
    return false;

  //Decomposite line to chords
  QStringList list = line.split( QChar(' '), Qt::SkipEmptyParts );

  if( list.count() == 0 )
    return false;

  int chordDetected = 0;
  int notePlug, chordPlug;
  for( const auto &chordName : list )
    if( chordParser( chordName, notePlug, chordPlug ) )
      chordDetected++;

  //Chord line detected if detected more than 90% chords in line
  return chordDetected * 100 / list.count() >= 60;
  }
