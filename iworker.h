#ifndef IWORKER_H
#define IWORKER_H

#include <string>
#include <vector>
#include "thread.h"

class IWorker: public Thread
{
public:
  IWorker(std::string name):m_workerName(name)
  {
  }
  virtual ~IWorker(){}

  virtual void executeCmd(std::vector<std::string>);
  virtual void setWorkerName(std::string);
  virtual std::string getWorkerName(void);
  virtual void Main(){}

  std::string m_workerName;

};

#endif // IWORKER_H
