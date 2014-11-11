#ifndef DOCPULOAD_H
#define DOCPULOAD_H

#include "dokorgcmd.h"

#include <iostream>
#include <string>


class DoCpuLoad: public DoKorgCmd
{
public:

  DoCpuLoad(KorgCmd kc):DoKorgCmd(kc)
  {
    std::cout<<"DoCpuLoad has been created with SCHED_OTHER !!!"<<std::endl;
  }
  void executeIt();




};

#endif // DOCPULOAD_H
