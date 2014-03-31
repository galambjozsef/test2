#include "utility.h"
#include <algorithm>
#include <unistd.h>
#include <string.h>

void Sleep(long milliSec)
{
    usleep(milliSec*1000);
}



char* itoa(int n, char *buff, int radix)
// convert a positive integer n to char *buff
// for instant, this function work with radix <= 10;
// a little change to run with radix > 10
{
    if( radix == 10 )
      sprintf( buff, "%d", n );
    else if( radix == 16 )
      sprintf( buff, "%x", n );
    else if( radix == 8 )
      sprintf( buff, "%o", n );

    return( buff );
}


bool InsensitiveSearch(const std::string& _source,const std::string& _token)
{
	//search case insensitive
	if(_source.empty())
		return false;
	std::string app = toUPPER(_source);
	std::string app_token = toUPPER(_token);
	int pos = app.find(app_token.c_str());
	if(pos != std::string::npos)
		return true;
	else
		return false;
}

std::string int2string(int const& _i)
{
	char buff[65];
	memset(buff,0,65);
	itoa( _i, buff, 10 );
	std::string app(buff);
	return app;
}

std::string long2string(long const& _i)
{
	char buff[65];
	itoa( _i, buff, 10 );
	std::string app(buff);
	return app;
}

std::vector<std::string> parseString(std::string string2Parse,std::string delimiter)
{
    std::vector < std::string > tokens;       //we'll put all of the tokens in here
    std::string  temp;

    string2Parse.erase(std::remove(string2Parse.begin(), string2Parse.end(), ' '), string2Parse.end());

    if(string2Parse.find(delimiter, 0) == std::string::npos)
      return tokens;

    while (string2Parse.find(delimiter, 0) != std::string::npos)
    { //does the string has a delimiter in it?
      size_t  pos = string2Parse.find(delimiter, 0); //store the position of the delimiter
      temp = string2Parse.substr(0, pos);      //get the token
      string2Parse.erase(0, pos + 1);          //erase it from the source
      tokens.push_back(temp);                  //and put it into the array
    }

    if(!string2Parse.empty())
      tokens.push_back(string2Parse);             //the last token is all alone
    return tokens;
}


int fsize(FILE *fp)
{
    int prev=ftell(fp);
    fseek(fp, 0L, SEEK_END);
    int sz=ftell(fp);
    fseek(fp,prev,SEEK_SET); //go back to where we were
    return sz;
}


int fsize(int fd)
{
  int prev= lseek(fd, 0L, SEEK_CUR);
  int sz=lseek(fd, 0L, SEEK_END);
  lseek(fd,prev,SEEK_SET); //go back to where we were
  return sz;
}


int GetFileDim(const std::string& TarFile,long& filedim)
{

    FILE *streamin;
    streamin = fopen(TarFile.c_str(), ("r+b") );

    if( streamin  == NULL )
            return 1;

    fseek(streamin,SEEK_END,SEEK_END);
    filedim = ftell(streamin);
    fclose( streamin );

    return 0;
}
