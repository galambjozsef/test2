#include "korgsink.h"
#include "criticalsectionlock.h"


KorgSink::KorgSink()
{
}

KorgSink::~KorgSink()
{
  CleanupQueue();
}

void KorgSink::Notify( const KorgCmd* kC )
{
  CriticalSectionLock l( guard );
  queue.push( kC->Clone() );
}



std::auto_ptr<KorgCmd> KorgSink::GetKorgCmd()
{
  CriticalSectionLock l( guard );

  if( queue.empty() )
    return( (std::auto_ptr<KorgCmd>)(NULL) );
  else
  {
    KorgCmd* kC = queue.front();
    queue.pop();
    return( (std::auto_ptr<KorgCmd>)kC );
  }
}




void KorgSink::HandleKorgCmd( const KorgCmd* kC )
{

  theInvoker->InvokeHandler(kC);

}


bool KorgSink::HandleFirstKorgCmd()
{
  std::auto_ptr< KorgCmd > kC( GetKorgCmd() ) ;
  const KorgCmd* tmpPtr = kC.get() ;
  if( tmpPtr != NULL )
  {
    HandleKorgCmd( tmpPtr );
    return true;
  }
  else
  {
    return false;
  }
}

void KorgSink::CleanupQueue()
{
  CriticalSectionLock l( guard );

  while( ! queue.empty() )
  {
    KorgCmd* kC = queue.front();
    delete kC;
    queue.pop();
  }
}








