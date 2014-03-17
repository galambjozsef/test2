#include "criticalsection.h"

CriticalSection :: CriticalSection()
{
  pthread_mutexattr_t mta;
  pthread_mutexattr_init(&mta);
  pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&mutex, &mta);
}

CriticalSection :: ~CriticalSection()
{
  pthread_mutex_destroy(&mutex);
}

void CriticalSection :: Enter()
{
  pthread_mutex_lock (&mutex);
}

void CriticalSection :: Leave()
{
  pthread_mutex_unlock (&mutex);
}
