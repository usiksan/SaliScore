#ifndef CSLINE_H
#define CSLINE_H

#include "CsConfig.h"
#include "CsChord.h"
#include "CsNote.h"
#include "CsLyricSymbol.h"
#include "SvLib/SvJsonIO.h"

#include <QMap>

class CsLine
  {
    QMap<QString,QString>             mTrackMap;    //!< Map of line tracks
    int                               mTaktCount;   //!< Takt count per line

    //Cashes
    mutable QMap<QString,CsChordList> mChordMap;
    mutable QMap<QString,CsNoteList>  mNoteMap;
    mutable CsLyricLine               mLyricLine;
  public:
    CsLine();
    CsLine( const QString &lang, const QString &rem );


    //========================================================
    //    Information
    bool        contains( const QString &part ) const { return mTrackMap.contains(part); }

    bool        isRemark() const { return mTrackMap.contains(KEY_LYRIC); }


    QString     textGet( const QString &part ) const { return mTrackMap.value(part); }

    void        textSet( const QString &part, const QString &text ) { mTrackMap.insert( part, text ); }


    CsChordList chordListGet( const QString &part ) const;

    void        chordListSet( const QString &part, const CsChordList &chordList );


    CsNoteList  noteListGet( const QString &part ) const;

    void        noteListSet( const QString &part, const CsNoteList &noteList );


    CsLyricLine lyricLineGet() const;

    void        lyricLineSet( const CsLyricLine &line );


    void        partRename( const QString &prevPart, const QString &newPart );

    void        partRemove( const QString &part );

    void        clear();

    void        clearCashe();

    //========================================================
    //   Tick and Takt
    int         taktCount() const { return mTaktCount; }

    void        taktCountSet( int tc ) { mTaktCount = tc; }

    //========================================================
    //    json part
    void        json( SvJsonWriter &js ) const;

    void        json( const SvJsonReader &js );

  private:
    template<typename CsItem>
    void parseList( const QString str, QList<CsItem> &list ) const
      {
      list.clear();
      if( !str.isEmpty() ) {
        QStringList strItemList = str.split( QChar('|') );
        list.reserve( strItemList.count() );
        for( auto const &str : strItemList )
          list.append( CsItem::fromString( str ) );
        }
      }

    template<typename CsItem>
    QString toString( const QList<CsItem> &list ) const
      {
      if( list.count() ) {
        QStringList strItemList;
        strItemList.reserve( list.count() );
        for( auto const &item : list )
          strItemList.append( item.toString() );
        return strItemList.join( QChar('|') );
        }
      return QString{};
      }

  };

using CsLineList = QList<CsLine>;

#endif // CSLINE_H
