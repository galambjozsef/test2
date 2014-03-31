#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>


void Sleep(long milliSec);
char* itoa(int n, char *buff, int radix);
std::vector<std::string> parseString(std::string string2Parse,std::string delimiter);
int fsize(FILE *fp);
int fsize(int fd);
std::string int2string(int const& _i);
std::string long2string(long const& _i);
bool InsensitiveSearch(const std::string& _source,const std::string& _token);
int GetFileDim(const std::string& TarFile,long& filedim);
//auxiliary function to compare case insensitive
static std::string toUPPER(const std::string& s)
{
	std::string temp;
	temp = s.c_str();
	for(unsigned int i = 0; i< temp.length();i++)
		temp[i] = toupper(temp[i]);
	return temp;
}

#endif // THREADUTIL_H
