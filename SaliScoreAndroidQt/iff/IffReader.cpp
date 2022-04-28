#include "IffReader.h"

IffReader::IffReader(QFilePtr file, bool riff) :
  mFile(file),
  mRiff(riff)
  {
  //Read chunk name
  mFile->read( mChunkName, 4 );
  mChunkName[4] = 0;

  mChunkLength = mRiff ? getUint32le() : getUint32be();

  mChunkStart = mFile->pos();
  }



IffReader::IffReader(QString fname, bool riff) :
  mFile( new QFile(fname) ),
  mRiff(riff)
  {
  if( mFile->open( QIODevice::ReadOnly ) )
    mChunkLength = static_cast<quint32>( mFile->size() );
  else
    mChunkLength = 0;
  mChunkName[0] = 0;
  mChunkStart = 0;
  }






IffReader::~IffReader()
  {
  if( mChunkLength ) {
    mFile->seek( mChunkStart + mChunkLength );
    }
  }



bool IffReader::isEnd() const
  {
  return mFile->pos() >= mChunkStart + mChunkLength;
  }



bool IffReader::needFour(const char *name)
  {
  char buf[5];
  mFile->read( buf, 4 );
  buf[4] = 0;
  return strcmp( buf, name ) == 0;
  }



bool IffReader::compareChunkName(const char *name) const
  {
  return strcmp( mChunkName, name ) == 0;
  }




IffReader IffReader::getChunk()
  {
  return IffReader( mFile, mRiff );
  }



bool IffReader::readChunk(const char *name, std::function<bool (IffReader &)> fun1)
  {
  //Get next chunk
  IffReader chunk = getChunk();

  //If chunk name not equal to requested then return false
  if( !chunk.compareChunkName( name ) )
    return false;

  //Call function to parse chunk
  return fun1( chunk );
  }




//!
//! \brief readChars Reads exact counts of bytes into buffer
//! \param dst       Buffer where will be placed readed bytes
//! \param len       Count of readed bytes
//! \return          true if bytes readed successfully
//!
bool IffReader::readChars(char *dst, int len)
  {
  return mFile->read( dst, len ) == len;
  }




quint8 IffReader::peekUint8()
  {
  char ch;
  mFile->peek( &ch, 1 );
  return static_cast<quint8>(ch);
  }




quint8 IffReader::getUint8()
  {
  char ch;
  mFile->read( &ch, 1 );
  return static_cast<quint8>(ch);
  }



quint16 IffReader::getUint16be()
  {
  char buf[2];
  mFile->read( buf, 2 );
  return static_cast<quint16>( (static_cast<unsigned>(buf[0] & 0xff) << 8) | static_cast<unsigned>(buf[1] & 0xff) );
  }



quint16 IffReader::getUint16le()
  {
  char buf[2];
  mFile->read( buf, 2 );
  return static_cast<quint16>( (static_cast<unsigned>(buf[1] & 0xff) << 8) | static_cast<unsigned>(buf[0] & 0xff) );
  }



quint32 IffReader::getUint32be()
  {
  char buf[4];
  mFile->read( buf, 4 );
  return static_cast<quint32>(
      (static_cast<unsigned>(buf[0] & 0xff) << 24) |
      (static_cast<unsigned>(buf[1] & 0xff) << 16) |
      (static_cast<unsigned>(buf[2] & 0xff) << 8) |
      static_cast<unsigned>(buf[3] & 0xff) );
  }



quint32 IffReader::getUint32le()
  {
  char buf[4];
  mFile->read( buf, 4 );
  return static_cast<quint32>(
      (static_cast<unsigned>(buf[3] & 0xff) << 24) |
      (static_cast<unsigned>(buf[2] & 0xff) << 16) |
      (static_cast<unsigned>(buf[1] & 0xff) << 8) |
      static_cast<unsigned>(buf[0] & 0xff) );
  }



qint8 IffReader::getInt8()
  {
  char ch;
  mFile->read( &ch, 1 );
  return static_cast<qint8>(ch);
  }



qint16 IffReader::getInt16be()
  {
  char buf[2];
  mFile->read( buf, 2 );
  return static_cast<qint16>( (static_cast<unsigned>(buf[0] & 0xff) << 8) | static_cast<unsigned>(buf[1] & 0xff) );
  }




qint16 IffReader::getInt16le()
  {
  char buf[2];
  mFile->read( buf, 2 );
  return static_cast<qint16>( (static_cast<unsigned>(buf[1] & 0xff) << 8) | static_cast<unsigned>(buf[0] & 0xff) );
  }




qint32 IffReader::getInt32be()
  {
  char buf[4];
  mFile->read( buf, 4 );
  return static_cast<qint32>(
      (static_cast<unsigned>(buf[0] & 0xff) << 24) |
      (static_cast<unsigned>(buf[1] & 0xff) << 16) |
      (static_cast<unsigned>(buf[2] & 0xff) << 8) |
      static_cast<unsigned>(buf[3] & 0xff) );
  }




qint32 IffReader::getInt32le()
  {
  char buf[4];
  mFile->read( buf, 4 );
  return static_cast<qint32>(
      (static_cast<unsigned>(buf[3] & 0xff) << 24) |
      (static_cast<unsigned>(buf[2] & 0xff) << 16) |
      (static_cast<unsigned>(buf[1] & 0xff) << 8) |
      static_cast<unsigned>(buf[0] & 0xff) );
  }


