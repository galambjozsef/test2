#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H
#include "thread.h"
#include "psemaphore.h"
#include "iworker.h"
#include "korgdevice.h"
#include "korgsink.h"

#include "socketclient.h"






//class WorkerThread : public IWorker, public Thread
class WorkerThread : public IWorker
{
public:
  WorkerThread(std::string n);
  virtual ~WorkerThread();
  pThreadSemaphore* m_MutexCmd;

//  void executeCmd(std::vector<std::string>);

  void executeKorgCmd(const KorgCmd*);
  void setWorkerName(std::string);
  std::string getWorkerName(void);

  void Main();

private:
  std::string m_cmd;
  korgDevice m_dev;
  bool m_stop;

//  SocketClient m_client;

};

#endif // WORKERTHREAD_H
