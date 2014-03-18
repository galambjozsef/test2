#include "dokorgcmd.h"
#include "korgdevice.h"

DoKorgCmd::DoKorgCmd(KorgCmd kc) : kC(kc)
{
}

DoKorgCmd *DoKorgCmd::factory(KorgCmd kc)
{
  DoKorgCmd* theDoer = NULL;
  if(kc.getType() == "read")
     theDoer= new DoRead(kc);
  else if (kc.getType() == "transfer")
    return new DoTransfer(kc);



  if(theDoer == NULL)
    return theDoer;


  return theDoer;
}


void DoRead::executeIt()
{
  std::cout<<"DoRead has been executed !!!"<<std::endl;
  korgDevice kD;
  int dim = 0;
  float elapsed = 0,speed = 0,bufSize;

  if(kC.getArgs().size() == 1)
    bufSize = 256;
  else if(kC.getArgs().at(1).empty())
    bufSize = 256;
  else
    bufSize = atoi(kC.getArgs().at(1).c_str());

  kD.readFileAndMeasure(kC.getArgs().at(0),bufSize,&dim,&elapsed,&speed);

}


void DoTransfer::executeIt()
{
  std::cout<<"DoTransfer has been executed !!!"<<std::endl;
}
