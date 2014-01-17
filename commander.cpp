#include "commander.h"
#include "iworker.h"
#include "workerthread.h"




Commander::Commander()
{
}

void Commander::executeCmd(std::string sentCmds)
{
  std::vector<std::string> receivedCmds = parseString(sentCmds,",");

  if(receivedCmds.size() == 0)
  {
    std::cout<<"NO CMD FOUND !!!!"<<std::endl;
    return;
  }

  for(int i = 0;i < receivedCmds.size();i++)
  {
    std::cout<<"n° "<<i<<"Commander Parsed cmd -->"<<receivedCmds.at(i)<<std::endl;
  }

  if(receivedCmds.at(0) == "create" && !receivedCmds.at(1).empty())
  {
      std::string workerName = receivedCmds.at(1);
      IWorker* wrkr = GetWorker(workerName);
      if(wrkr != NULL)
      {
          std::cout<<"Worker named "<<workerName<<" already exists !"<<std::endl;
          return;
      }

      IWorker* a_worker = new WorkerThread(receivedCmds.at(1));
      std::pair<std::string,IWorker*> temp_item;

      temp_item.first = workerName;
      temp_item.second = a_worker;
      this->workersMap.insert(temp_item);
      a_worker->Run(0);
      std::cout<<"Worker named "<<workerName<<" has been created and running !"<<std::endl;

  }
  else if(receivedCmds.at(0) == "delete" && !receivedCmds.at(1).empty())
  {
      std::string workerName = receivedCmds.at(1);
      IWorker* wrkr = GetWorker(workerName);
      if(wrkr == NULL)
      {
          std::cout<<"Worker named "<<workerName<<" does not exist !"<<std::endl;
          return;
      }
      std::vector<std::string> appCmd;
      appCmd.push_back("stop");
      wrkr->executeCmd(appCmd);
      wrkr->m_MutexWaitThread->Wait(2000);
      delete(wrkr);
      std::map<std::string,IWorker*>::iterator it;
      it = workersMap.find(workerName);
      workersMap.erase(it);
  }
  else
  {   //Try tor process cmds to the requested worker
      std::string workerName = receivedCmds.at(0);      
      receivedCmds.erase(receivedCmds.begin());

      IWorker* wrkr = GetWorker(workerName);
      if(wrkr != NULL)
        wrkr->executeCmd(receivedCmds);
      else
        std::cout<<  "Worker named "<<workerName<<" DOES NOT EXIST !!!!";

  }

}


IWorker* Commander::GetWorker(std::string workerName)
{
  std::map<std::string,IWorker*>::iterator it;
  it = workersMap.find(workerName);
  if (it == workersMap.end())
  {
    //std::cout<<  "Worker named "<<workerName<<" DOES NOT EXIST !!!!";
    return NULL;
  }

  return (*it).second;
}



