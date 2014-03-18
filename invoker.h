#ifndef INVOKER_H
#define INVOKER_H

//#include "checkedcast.h"
#include <iostream>


class KorgCmd ;

class Invoker
{
public :
  virtual ~Invoker() {} ;
  virtual void InvokeHandler( const KorgCmd* e ) const = 0 ;
} ;

//AA: in pratica serve ad invocare una funzione di una classa il cui argomento derivi da KorgCmd* ...

template< class T1, class T2 >
class TypeSafeInvoker : public Invoker
{
public:
  typedef void (T1::*HandlingFunction)( const T2* ) ;
  TypeSafeInvoker( T1* theSink, HandlingFunction theHandler )
  {
    sink = theSink ;
    f = theHandler ;
  }
  virtual void InvokeHandler( const KorgCmd* kC ) const
  {
    const T2* kC2 = static_cast< const T2* >( kC ) ;
    (sink->*f)( kC2 ) ;
  }
private :
  T1* sink ;
  HandlingFunction f ;
} ;
#endif // INVOKER_H
