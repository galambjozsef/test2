#include "psemaphore.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "timestamp.h"
#include "utility.h"




pSemaphore::pSemaphore(const char* name,int owner)
{
  int ret=0;
  m_name = "/";
  m_name +=name;

  semaphore=sem_open(m_name.c_str(),
                      O_CREAT,
                      S_IRUSR | S_IWUSR |
                      S_IRGRP | S_IWGRP |
                      S_IROTH | S_IWOTH,
                      owner);
}

pSemaphore::~pSemaphore()
{
  if(semaphore)
    sem_close(semaphore);
}

void pSemaphore::UnLink()
{
    if(semaphore)
      sem_close(semaphore);
    sem_unlink(m_name.c_str());
    semaphore=NULL;
}

bool pSemaphore::Wait(int msec_timeout)
{
  int s;
  if(semaphore)
  {
      int ret=-1;
      TimeStamp t;
      do
      {
          ret=sem_trywait(semaphore);

          if(ret==0)
                  return true;
          else
          {
      //     if(errno == EAGAIN)
      //     {
      //        std::cout<<"PSemaphore EAGAIN---->Unlock"<<std::endl;
      //	Unlock();
      //     }
          }

          Sleep(10);
      }
      while(!(t.IsElapsed(msec_timeout)));
  }
  return false;
}




bool pSemaphore::WaitInfinite()
{
    int s;
    if(semaphore)
    {
      int ret;
      ret=sem_wait(semaphore);
      if(ret==0)
        return true;
    }
    return false;
}

bool pSemaphore::Unlock()
{
    if(semaphore)
    {
        if(sem_post(semaphore)==0)
        {
            return true;
        }
    }
    return false;
}


pThreadSemaphore::pThreadSemaphore(const char* name,int owner)
{
    int ret=0;
    semaphore = NULL;
    m_name = "/";
    m_name +=name;

    semaphore=sem_open(	m_name.c_str(),
                      O_CREAT,
                      S_IRUSR | S_IWUSR |
                      S_IRGRP | S_IWGRP |
                      S_IROTH | S_IWOTH,
                      owner);
                                            /*
    if(semaphore!=SEM_FAILED)
    {                               //shared = 0 valido tra i thread di un processo,
                                //shared = 1 valido tra più processi
                                //owner=1 mutex libero e quindi chiamando la wait questa ritorna subito e prende possesso del mutex

            sem_init(semaphore,shared,owner);//imposto SHARED e 0 come valore iniziale
    }*/
}

pThreadSemaphore::~pThreadSemaphore()
{
    Unlock();
}

void pThreadSemaphore::Delete()
{
    if(semaphore)
      sem_close(semaphore);
    sem_unlink(m_name.c_str());
    semaphore=NULL;
}


bool pThreadSemaphore::Wait(int msec_timeout)
{
    int s;
    if(semaphore)
    {
        int ret=-1;
        TimeStamp t;
        do
        {
            ret=sem_trywait(semaphore);

            if(ret==0)
                    return true;
            else
            {
                //if(errno == EAGAIN)
                //{
                //    std::cout<<"pThreadSemaphore EAGAIN---->Unlock"<<std::endl;
                //    Unlock();
                //}
            }

            Sleep(10);
        }
        while(!(t.IsElapsed(msec_timeout)));
    }
    return false;
}




bool pThreadSemaphore::Unlock()
{
    if(semaphore)
    {
        if(sem_post(semaphore)==0)
        {
            return true;
        }
    }
    return false;
}
