#include <iostream>
#include "utility.h"
#include "workerthread.h"


WorkerThread::WorkerThread(std::string n):IWorker(n)
{
  int isOwner = 1;
  m_MutexCmd = new pThreadSemaphore("setCmd",isOwner);
  m_stop = false;
}

WorkerThread::~WorkerThread()
{
  delete m_MutexCmd;
}

void WorkerThread::executeCmd(std::vector<std::string> cmds)
{
  m_MutexCmd->Wait(2000);

  if(cmds.size() < 1)
    return;


  for(int i = 0; i < cmds.size(); i++)
  {
      std::cout<<"WorkerThread "<< m_workerName<<" Received this command: "<<cmds.at(i) <<std::endl;
  }

  if(cmds.at(0) == "open")
  {
    m_dev.openkorgDevice(cmds.at(1));
  }
  else if (cmds.at(0) == "read")
  {
    //m_dev.readFromKorgDevice();
      m_dev.readFromKorgDeviceEx();
  }
  else if (cmds.at(0) == "write")
  {
      m_dev.writeFromBuffer2KorgDevice(cmds.at(1));
  }
  else if (cmds.at(0) == "writeF")
  {
      m_dev.writeFromFile2KorgDevice(cmds.at(1));
  }
  else if (cmds.at(0) == "ioctl")
  {

  }
  else if (cmds.at(0) == "release")
  {

  }
  else if (cmds.at(0) == "stop")
  {
    m_stop = true;
  }
  else
  {
      std::cout<<"WorkerThread Did not receive valid commands !!!!"<<std::endl;
  }


  m_MutexCmd->Unlock();
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

  this->SetScheduling(20);
  std::cout<<"WorkerThread::Main()";
  std::string prevCmd = "";
  while(!m_stop)
  {

//    if(prevCmd != m_cmd)
//    {
//        std::cout<<"Worker thread received this new cmd --> "<<m_cmd<<std::endl;
//        prevCmd = m_cmd;
//    }

//      std::cout<<"Worker "<<m_workerName<<" is running"<<std::endl;

    Sleep(1000);

  }


}
