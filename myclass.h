#ifndef MYCLASS_H
#define MYCLASS_H

#include <stdio.h>
#include <string>

class myClass
{
public:
  myClass();
  void printMyName();

private:
  std::string myName;

};

#endif // MYCLASS_H
