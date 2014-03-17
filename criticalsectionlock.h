#ifndef CRITICALSECTIONLOCK_H
#define CRITICALSECTIONLOCK_H


class CriticalSection;

class CriticalSectionLock
{
public :
  CriticalSectionLock( CriticalSection & s ) ;
  ~CriticalSectionLock() ;

private :
  CriticalSection& section ;
} ;


#endif // CRITICALSECTIONLOCK_H
