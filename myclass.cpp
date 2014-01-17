#include "myclass.h"
#include <iostream>

myClass::myClass(std::string n,std::string p):myName(n),myPassion(p)
{
  myName = n;
  myPassion = p;
}


void myClass::printMyName()
{
  std::cout<<"My real name is " << myName << std::endl;
  std::cout<<"My passion is " << myPassion << std::endl;
}
