#include "criticalsectionlock.h"
#include "criticalsection.h"

CriticalSectionLock :: CriticalSectionLock( CriticalSection& s ) :
section( s)
{
	section.Enter() ;
}

CriticalSectionLock :: ~CriticalSectionLock()
{
	section.Leave() ;
}
