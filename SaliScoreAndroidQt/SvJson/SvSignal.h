#ifndef SVSIGNAL_H
#define SVSIGNAL_H

#include <functional>
#include <QObject>


template <typename SvCallable>
class SvSignalBase
  {
  protected:
    QList<SvCallable> mSlotList;
  public:
    SvSignalBase() {}

    void connect( SvCallable call ) { mSlotList.append( call ); }

    void disconnect( SvCallable call ) { mSlotList.removeAll( call ); }

    void disconnectAll() { mSlotList.clear(); }
  };


class SvSignal0 : public SvSignalBase< std::function<void ()> >
  {
  public:
    SvSignal0() {}

    void operator () ()
      {
      for( auto &fun : qAsConst(mSlotList) )
        fun();
      }
  };


template <typename SvArg0>
class SvSignal1 : public SvSignalBase< std::function<void ( SvArg0 arg )> >
  {
  public:
    SvSignal1() {}

    void operator () ( SvArg0 arg )
      {
      for( auto &fun : qAsConst(this->mSlotList) )
        fun(arg);
      }
  };


template <typename SvArg0, typename SvArg1>
class SvSignal2 : public SvSignalBase< std::function<void ( SvArg0 arg0, SvArg1 arg1 )> >
  {
  public:
    SvSignal2() {}

    void operator () ( SvArg0 arg0, SvArg1 arg1 )
      {
      for( auto &fun : qAsConst(this->mSlotList) )
        fun(arg0,arg1);
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



#endif // SVSIGNAL_H
