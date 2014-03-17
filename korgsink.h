#ifndef KORGSINK_H
#define KORGSINK_H

#include "criticalsection.h"
#include "korgcmd.h"
#include "invoker.h"

#include <queue>
#include <memory>


typedef std::queue< KorgCmd* > KorgCmdQueue ;





class KorgSink
{
public:
  KorgSink();
  virtual ~KorgSink();

  virtual void Notify( const KorgCmd* e ) ;

  std::auto_ptr<KorgCmd> GetKorgCmd();

protected:
  bool HandleFirstKorgCmd();
  void HandleKorgCmd( const KorgCmd* );

  template< class T1, class T2 > void subscribeHandlingFunction( void (T1::*f)(const T2*) )
  {
    T1* downcastedThis = static_cast< T1* >( this ) ;
    TypeSafeInvoker< T1, T2 >* invoker =  new TypeSafeInvoker< T1, T2 >( downcastedThis, f ) ;

    //theInvoker = invoker;
  }

  void CleanupQueue();

private :
  KorgCmdQueue queue;
  CriticalSection guard;
  Invoker* theInvoker; //AA: funzione di gestione del KorgCmd

};

#endif // KORGSINK_H
