#include "thread.h"
#include <iostream>
#include "psemaphore.h"
#include "utility.h"
#include "GenericException.h"

Thread::Thread()
{
}

Thread::~Thread()
{
    hThread=NULL;
    m_MutexWaitThread->Delete();
    delete m_MutexWaitThread;
}

void Thread::Main(void){}



void* Thread::ThreadBoot(void* lpdwParam)
{
    try
    {   //Per non avere 2 run sullo stesso thread !!!!
        std::cout<< "ThreadBoot !!!!!!!"<<std::endl;
        Thread* that=(Thread*) lpdwParam;
        char arg[10];
        itoa(syscall( __NR_gettid),arg,10);
        that->m_thread_pid = arg;
        int isOwner = 1;
        that->m_MutexWaitThread = new pThreadSemaphore(that->m_thread_pid.c_str(),isOwner);
        that->m_MutexWaitThread->Wait(200);
        that->Main();
        that->m_MutexWaitThread->Unlock();
    }
        catch(BaseSignalException& )
        {
                std::cout<<"hread::ThreadBoot() Caught !!!!!!!!!!!!!!!!!"<<std::endl;
                throw;//AA: nel caso che il thread eventualmente intercetti il throw dell' handler del segnale ...
        }
}

void Thread::Run(long dwStackSize)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

    pthread_create(&hThread,&attr,ThreadBoot,this);
    std::cout<<"Thread::Run!!!"<<std::endl;
}


bool Thread::SetScheduling(int _priority,int _policy)
{
    pthread_t thread_id = pthread_self();
    struct sched_param param;
    param.sched_priority = _priority;
    int irc = pthread_setschedparam(thread_id, _policy, &param);
    if(irc != 0)
    {
    //    std::cout<<"Alarm thread scheduling was not changed!!!"<<std::endl;
            return false;
    }
    else
         return true;

}


