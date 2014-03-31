#ifndef THREAD_H
#define THREAD_H



#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string>
#include <iostream>

class pThreadSemaphore;


class Thread
{

private:
  static void* ThreadBoot(void*); //AA: ci vuole un mutex ...
  pthread_t hThread;
  std::string m_thread_pid;

public:
 pThreadSemaphore* m_MutexWaitThread;

protected:
  virtual void Main(void);
 // pThreadSemaphore* m_MutexWaitThread;

public:
  Thread();
  virtual ~Thread();
  virtual void Run(long);

  virtual bool SetScheduling(int _priority,int _policy = SCHED_RR  /*SCHED_FIFO*/);
//  void GetSchedulingInfo(int& _priority,int& _policy);
};



#endif // THREAD_H
