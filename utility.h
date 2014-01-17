#ifndef UTIL_H
#define UTIL_H

#include <sys/types.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>


void Sleep(long milliSec);
char* itoa(int n, char *buff, int radix);
std::vector<std::string> parseString(std::string string2Parse,std::string delimiter);
int fsize(FILE *fp);

#endif // THREADUTIL_H
