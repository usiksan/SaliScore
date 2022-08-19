/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   IFF files reader. Represents one chunk in file.
*/
#ifndef IFFREADER_H
#define IFFREADER_H

#include <QFile>
#include <QByteArray>
#include <QSharedPointer>
#include <functional>

using QFilePtr = QSharedPointer<QFile>;

#define IFF_FOUR( ch0, ch1, ch2, ch3 ) ((static_cast<quint32>(ch0) << 24) | (static_cast<quint32>(ch1) << 16) | (static_cast<quint32>(ch2) << 8) | static_cast<quint32>(ch3))

class IffReader
  {
    QFilePtr mFile;         //! Source stream file
    char     mChunkName[5]; //! Current chunk name
    quint32  mChunkLength;  //! Current chunk lenght
    qint64   mChunkStart;   //! Position in file where current chunk is started
    bool     mRiff;
  public:
    IffReader( const IffReader& ) = default;
    IffReader( QFilePtr file, bool riff );
    IffReader( QString fname, bool riff );
    ~IffReader();

    //!
    //! \brief name Return current chunk name
    //! \return     Current chunk name
    //!
    const char *name() const { return mChunkName; }

    //!
    //! \brief isValid Tests if current chunk valid
    //! \return        true if chunk valid
    //!
    bool        isValid() const { return mChunkLength != 0; }

    //!
    //! \brief isEnd Tests that current file pointer reach end of chunk
    //! \return      true if current file pointer reach end of chunk
    //!
    bool        isEnd() const;

    bool        needFour( const char *name );

    bool        compareChunkName( const char *name ) const;

    //!
    //! \brief chunkFour Return current chunk name as unsigned 32 bit for extremal fast compare
    //! \return          Current chunk name as unsigned 32 bit
    //!
    quint32     chunkFour() const { return IFF_FOUR(mChunkName[0],mChunkName[1],mChunkName[2],mChunkName[3]); }

    quint32     chunkLenght() const { return mChunkLength; }

    IffReader   getChunk();

    bool        readChunk( const char *name, std::function<bool( IffReader &reader )> fun1 );

    //=====================================================================
    // Read primitives

    //!
    //! \brief readChars Reads exact counts of bytes into buffer
    //! \param dst       Buffer where will be placed readed bytes
    //! \param len       Count of readed bytes
    //! \return          true if bytes readed successfully
    //!
    bool        readChars( char *dst, int len );

    //!
    //! \brief peekUint8 Peeks next available char from input stream
    //! \return          Next available char
    //!
    quint8      peekUint8();

    //!
    //! \brief getUint8 Reads unsigned int8 (8 bit)
    //! \return         unsigned int8 (8 bit)
    //!
    quint8      getUint8();
    quint16     getUint16be();
    quint16     getUint16le();
    quint32     getUint32be();
    quint32     getUint32le();

    qint8       getInt8();
    qint16      getInt16be();
    qint16      getInt16le();
    qint32      getInt32be();
    qint32      getInt32le();
  };

#endif // IFFREADER_H
