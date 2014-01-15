#include "myclass.h"
#include <iostream>

myClass::myClass()
{

  myName = "antenio";
}


void myClass::printMyName()
{
  std::cout<<"My real name is " << myName << std::endl;
}
