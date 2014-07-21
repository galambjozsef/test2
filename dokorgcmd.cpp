#include "dokorgcmd.h"
#include "korgdevice.h"
#include "statistics.h"
#include "dodevmem2.h"

DoKorgCmd::DoKorgCmd(KorgCmd kc) : kC(kc)
{
}

DoKorgCmd *DoKorgCmd::factory(KorgCmd kc)
{
  DoKorgCmd* theDoer = NULL;
  if(kc.getType() == "read")
  {
     theDoer = new DoRead(kc);
  }
  else if (kc.getType() == "transfer")
  {
    theDoer = new DoTransfer(kc);
  }
  else if (kc.getType() == "mmap")
  {
    theDoer = new DoDevMem2(kc);
  }




  return theDoer;
}


void DoRead::executeIt()
{
  std::cout<<"DoRead has been executed !!!"<<std::endl;
  korgDevice kD;
  int dim = 0;
  float bufSize;

  if(kC.getArgs().size() == 1)
    bufSize = 256;
  else if(kC.getArgs().at(1).empty())
    bufSize = 256;
  else
    bufSize = atof(kC.getArgs().at(1).c_str());


  int iterations;

  std::cout<<"Arg size = "<< kC.getArgs().size()<<std::endl;

  if(kC.getArgs().size() < 2)
    iterations = 1;
  else
    iterations = atoi(kC.getArgs().at(2).c_str());

//  float totSpeed;


  Statistics stats("MB/s");

  double speed = 0,elapsed = 0;
  for(int i = 0;i<iterations;i++)
  {
    kD.readFileAndMeasure(kC.getArgs().at(0),bufSize,&dim,&elapsed,&speed);
    //totSpeed += speed;
    std::cout<<"Adding speed = "<<speed<<std::endl;
    stats.addMeasure(speed);
  }

  //totSpeed = totSpeed / iterations;
  double avg,stdVar;
  stats.getAvgAndVariance(&avg,&stdVar);

  std::cout<<std::endl;
  std::cout<<std::endl;
  std::cout<<"##################################################################"<<std::endl;
  std::cout<<"Mean Speed = "<<avg<<" "<<stats.getUM()<<std::endl;
  std::cout<<"Standard Deviation = "<<stdVar<<" "<<stats.getUM()<<std::endl;
  std::cout<<"Samples = "<<stats.getSamples()<<std::endl;
  std::cout<<"##################################################################"<<std::endl;
}


void DoTransfer::executeIt()
{
  std::cout<<"DoTransfer has been executed !!!"<<std::endl;
  int *p = NULL;
  *p = 1;
}
