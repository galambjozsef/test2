#ifndef COMMANDER_H
#define COMMANDER_H

#include<map>
#include "utility.h"



class IWorker;

class Commander
{
public:
  Commander();

  void executeCmd(std::string );

private:
  std::map<std::string,IWorker*> workersMap;
  IWorker* GetWorker(std::string);

};

#endif // COMMANDER_H
