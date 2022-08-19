#ifndef CSTRACK_H
#define CSTRACK_H

#include "CsConfig.h"
#include "CsChord.h"
#include "CsNote.h"
#include "CsLyricSymbol.h"
#include "SvLib/SvJsonIO.h"

class CsTrack
  {
  public:
    CsTrack();
    virtual ~CsTrack() {}

    virtual int         type() const = 0;


    virtual QString     textGet() const { return QString{}; }

    virtual void        textSet( const QString &t ) { Q_UNUSED(t) }


    virtual CsChordList chordListGet() const { return CsChordList{}; }

    virtual void        chordListSet( const CsChordList &list ) { Q_UNUSED(list) }


    virtual CsNoteList  noteListGet() const { return CsNoteList{}; }

    virtual void        noteListSet( const CsNoteList &list ) { Q_UNUSED(list) }


    virtual CsLyricLine lyricLineGet() const { return CsLyricLine{}; }

    virtual void        lyricLineSet( const CsLyricLine &line ) { Q_UNUSED(line) }

    //!
    //! \brief json Writes object into json writer
    //! \param js   JSON writer
    //!
    virtual void json( SvJsonWriter &js ) const
      {
      js.jsonInt( "type", type() );
      }

    //!
    //! \brief json Reads object from json reader
    //! \param js   JSON reader
    //!
    virtual void json( SvJsonReader &js )
      {
      Q_UNUSED(js)
      }

    static CsTrack *buildFromJson( SvJsonReader &js );
  };

using CsTrackPtr = CsTrack*;

#endif // CSTRACK_H
