/*
  Project "Improving the visibility of json io"
  Author
    Alexander Sibilev
  Description
    Signal-slot simple alternate for non-qt hierarchy

    In source object for each signal must be declared signal object
    SvSignal<void(int,float)> mSignal;

    To emit signal source must call mSignal.emitSignal( 4, 5.0 );
  www
    www.salilab.com
  History
    26.04.2022 v1 Prototype creation
*/
#ifndef SVSIGNAL_H
#define SVSIGNAL_H

#include <functional>
#include <QList>

#define SV_SIGNAL_VERSION 1

class SvBinding;

class SvDisconnectable
  {
  public:
    virtual void disconnect( SvBinding *bind ) = 0;

  protected:
    void bind( SvBinding *binding, SvDisconnectable *dis );
  };


class SvBinding
  {
    SvDisconnectable *mSignal;
  public:
    SvBinding() : mSignal(nullptr) {}
    SvBinding( const SvBinding & ) = delete;
    SvBinding( SvBinding &&bind ) : mSignal(nullptr) { qSwap( mSignal, bind.mSignal ); }
    ~SvBinding() { if( mSignal != nullptr ) mSignal->disconnect( this ); }

    //void connect( SvDisconnectable *sig ) { disconnect(); mSignal = sig; }
    void disconnect() { if( mSignal != nullptr ) mSignal->disconnect( this ); mSignal = nullptr; }

    void operator = ( const SvBinding &src ) = delete;
    void operator = ( SvBinding &&src ) { qSwap( mSignal, src.mSignal ); }

    friend class SvDisconnectable;
  };



inline void SvDisconnectable::bind(SvBinding *binding, SvDisconnectable *dis)
  {
  if( binding != nullptr )
    binding->mSignal = dis;
  }


template <typename SvCallable>
class SvSignalBase : public SvDisconnectable
  {
  protected:
    struct SvSignalBinding
      {
        SvBinding  *mBinding;
        SvCallable  mCallable;

        SvSignalBinding() : mBinding(nullptr), mCallable(nullptr) {}
        SvSignalBinding( SvBinding *bind, SvCallable call ) : mBinding(bind), mCallable(call) {}
      };
    QList<SvSignalBinding> mSlotList;
  public:
    SvSignalBase() {}
    ~SvSignalBase() { disconnectAll(); }

    //!
    //! \brief connect Creates disconnectable connection. Link written into binding
    //! \param binding Object into which written link
    //! \param call    Calling object
    //!
    void connect( SvBinding *binding, SvCallable call )
      {
      SvSignalBinding sg( binding, call );
      mSlotList.append( sg );
      bind( binding, this );
      }

    void connect( SvBinding &binding, SvCallable call ) { connect( &binding, call ); }

    void connect( SvCallable call ) { connect( nullptr, call ); }

    virtual void disconnect( SvBinding *binding )
      {
      if( binding != nullptr ) {
        for( int i = 0; i < mSlotList.count(); i++ )
          if( mSlotList.at(i).mBinding == binding ) {
            mSlotList.removeAt(i);
            bind( binding, nullptr );
            return;
            }
        }
      }

    void disconnectAll()
      {
      for( int i = 0; i < mSlotList.count(); i++ ) {
        if( mSlotList.at(i).mBinding != nullptr )
          bind( mSlotList.at(i).mBinding, nullptr );
        }
      mSlotList.clear();
      }
  };


class SvSignal0 : public SvSignalBase< std::function<void ()> >
  {
  public:
    SvSignal0() {}

    void emitSignal()
      {
      for( auto &fun : qAsConst(mSlotList) )
        fun.mCallable();
      }
  };


template <typename SvArg0>
class SvSignal1 : public SvSignalBase< std::function<void ( SvArg0 arg )> >
  {
  public:
    SvSignal1() {}

    void emitSignal( SvArg0 arg )
      {
      for( auto &fun : qAsConst(this->mSlotList) )
        fun.mCallable(arg);
      }
  };


template <typename SvArg0, typename SvArg1>
class SvSignal2 : public SvSignalBase< std::function<void ( SvArg0 arg0, SvArg1 arg1 )> >
  {
  public:
    SvSignal2() {}

    void emitSignal( SvArg0 arg0, SvArg1 arg1 )
      {
      for( auto &fun : qAsConst(this->mSlotList) )
        fun(arg0,arg1);
      }
  };


template <typename SvArg0, typename SvArg1, typename SvArg2>
class SvSignal3 : public SvSignalBase< std::function<void ( SvArg0 arg0, SvArg1 arg1, SvArg2 arg2 )> >
  {
  public:
    SvSignal3() {}

    void emitSignal( SvArg0 arg0, SvArg1 arg1, SvArg2 arg2 )
      {
      for( auto &fun : qAsConst(this->mSlotList) )
        fun(arg0,arg1,arg2);
      }
  };


template <typename SvArg0, typename SvArg1, typename SvArg2, typename SvArg3>
class SvSignal4 : public SvSignalBase< std::function<void ( SvArg0 arg0, SvArg1 arg1, SvArg2 arg2, SvArg3 arg3 )> >
  {
  public:
    SvSignal4() {}

    void emitSignal( SvArg0 arg0, SvArg1 arg1, SvArg2 arg2, SvArg3 arg3 )
      {
      for( auto &fun : qAsConst(this->mSlotList) )
        fun(arg0,arg1,arg2,arg3);
      }
  };


// Forward-declare an empty template class
template< typename SomeFunctionType >
class SvSignal;

// Specialize the above with a function type and trivially inherit
// from the signal class with the arity in the typename.
template< typename ReturnType >
class SvSignal< ReturnType() > : public SvSignal0 {};

template< typename ReturnType, typename P1 >
class SvSignal< ReturnType(P1) > : public SvSignal1< P1 > {};

template< typename ReturnType, typename P1, typename P2 >
class SvSignal< ReturnType(P1, P2) > : public SvSignal2< P1, P2 > {};

template< typename ReturnType, typename P1, typename P2, typename P3 >
class SvSignal< ReturnType(P1, P2, P3) > : public SvSignal3< P1, P2, P3 > {};

template< typename ReturnType, typename P1, typename P2, typename P3, typename P4 >
class SvSignal< ReturnType(P1, P2, P3, P4) > : public SvSignal4< P1, P2, P3, P4 > {};



#endif // SVSIGNAL_H
