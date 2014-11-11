#include <iostream>
#include "utility.h"
#include "workerthread.h"
#include "dokorgcmd.h"


WorkerThread::WorkerThread(std::string n):IWorker(n)
{
  int isOwner = 1;
//  m_MutexCmd = new pThreadSemaphore("setCmd",isOwner);
  m_stop = false;
  std::cout<<"WorkerThread "<< m_workerName<<" Constructor !!!!!!"<<std::endl;
}

WorkerThread::~WorkerThread()
{
  std::cout<<"WorkerThread "<< m_workerName<<" Destructor !!!!!!"<<std::endl;
//  delete m_MutexCmd;
}


//void WorkerThread::executeCmd(std::vector<std::string> cmds)
//{
//  m_MutexCmd->Wait(2000);

//  if(cmds.size() < 1)
//    return;


//  for(int i = 0; i < cmds.size(); i++)
//  {
//      std::cout<<"WorkerThread "<< m_workerName<<" Received this command: "<<cmds.at(i) <<std::endl;
//  }

//  if(cmds.at(0) == "open")
//  {
//    m_dev.openkorgDevice(cmds.at(1));
//  }
//  else if (cmds.at(0) == "read")
//  {
//    //m_dev.readFromKorgDevice();
//      m_dev.readFromKorgDeviceEx();
//  }
//  else if (cmds.at(0) == "write")
//  {
//      m_dev.writeFromBuffer2KorgDevice(cmds.at(1));
//  }
//  else if (cmds.at(0) == "writeF")
//  {
//      m_dev.writeFromFile2KorgDevice(cmds.at(1));
//  }
//  else if (cmds.at(0) == "ioctl")
//  {

//  }
//  else if (cmds.at(0) == "release")
//  {

//  }
//  else if (cmds.at(0) == "stop")
//  {
//    m_stop = true;
//  }
//  else
//  {
//      std::cout<<"WorkerThread Did not receive valid commands !!!!"<<std::endl;
//  }


//  m_MutexCmd->Unlock();
//}



void WorkerThread::executeKorgCmd(const KorgCmd* kC)
{
  std::cout<< "WorkerThread::executeKorgCmd: START"<<std::endl;
  std::string type = kC->getType();
  std::vector<std::string> args = kC->getArgs();
  std::cout<< "WorkerThread::executeKorgCmd: CMDTYPE =  "<< type<<std::endl;
  for(int i = 0;i < args.size();i++)
    std::cout<< "WorkerThread::executeKorgCmd: ARGS =  "<< args.at(i)<<"("<<i<<")"<<std::endl;


  DoKorgCmd* doKgCmd = DoKorgCmd::factory(*kC);

  if(doKgCmd == NULL)
  {
      std::cout<<"WorkerThread "<<this->getWorkerName()<<" has been given a nonexistent cmd !!!"<<std::endl;
      return;
  }
  doKgCmd->executeIt();

  delete doKgCmd;

}

void WorkerThread::setWorkerName(std::string wName)
{
  m_workerName = wName;
}

std::string WorkerThread::getWorkerName(void)
{
  return m_workerName;
}


void WorkerThread::Main()
{
  std::cout<<"WorkerThread "<<this->getWorkerName()<<" has started !!!"<<" PID = "<<syscall( __NR_gettid)<<" ID = "<<pthread_self ()<<std::endl;
  this->SetScheduling(20);
//  std::cout<<"WorkerThread::Main()";
//  std::string prevCmd = "";



  subscribeHandlingFunction(&WorkerThread::executeKorgCmd);

  while(!m_stop)
  {
//    std::cout<<"WorkerThread "<<this->getWorkerName()<<" is running !!!"<<std::endl;
    HandleFirstKorgCmd();
    Sleep(1000);
  }

}
