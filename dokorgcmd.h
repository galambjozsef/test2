#ifndef DOKORGCMD_H
#define DOKORGCMD_H
#include "korgcmd.h"

#include <iostream>
#include <string>



class DoRead;
class DoTransfer;

class DoKorgCmd
{
public:
  DoKorgCmd()
  {}
  DoKorgCmd(KorgCmd kc);



  static DoKorgCmd* factory(KorgCmd kc);

  virtual void executeIt(){}

  void setKorgCmd(KorgCmd kc)
  {
    kC = kc;
  }

protected:
  KorgCmd kC;
};





class DoRead: public DoKorgCmd
{
public:

  DoRead(KorgCmd kc):DoKorgCmd(kc)
  {
    std::cout<<"DoRead has been created !!!"<<std::endl;
  }
  void executeIt();


};

class DoTransfer: public DoKorgCmd
{
public:

  DoTransfer(KorgCmd kc):DoKorgCmd(kc)
  {
    std::cout<<"DoTransfer has been created !!!"<<std::endl;
  }
  void executeIt();

};




#endif // DOKORGCMD_H
