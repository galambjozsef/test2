#ifndef CRITICALSECTION_H
#define CRITICALSECTION_H

#include <pthread.h>

class CriticalSectionLock ;

class CriticalSection
{
public :
  CriticalSection() ;
  ~CriticalSection() ;
private :

  void Enter() ;
  void Leave() ;

  pthread_mutex_t mutex;
friend class CriticalSectionLock ;
} ;

#endif
