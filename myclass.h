#ifndef MYCLASS_H
#define MYCLASS_H

#include <stdio.h>
#include <string>

class myClass
{
public:
  myClass(std::string,std::string);
  void printMyName();

private:
  std::string myName;
  std::string myPassion;

};

#endif // MYCLASS_H
