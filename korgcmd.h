#ifndef KORGCMD_H
#define KORGCMD_H

#include <string>
#include <stdlib.h>
#include <cstdio>
#include <vector>




class KorgCmd
{
public:
  KorgCmd(){}
  KorgCmd(std::string, std::vector<std::string>);
  virtual ~KorgCmd();

  virtual KorgCmd* Clone() const ;

  std::string getType() const
  {
    return cmdType;
  }

   std::vector<std::string> getArgs() const
  {
    return cmdArgs;
  }

 private:
  std::string cmdType;
  std::vector<std::string> cmdArgs;
};

#endif // KORGCMD_H
