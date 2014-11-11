#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "korgdevice.h"
#include <string.h>
#include "utility.h"
#include "timestamp.h"

#define MAX_SIZE 256

korgDevice::korgDevice(std::string dPath):devicePath(dPath)
{
  device_fd = -1;
}



bool korgDevice::openkorgDevice(std::string dPath)
{
  devicePath = dPath;
//  int kmem_fd = open((const char*)devicePath.c_str(), O_RDWR);
  device_fd = open((const char*)devicePath.c_str(), O_RDWR);
  if(device_fd < 0)
  {
    std::cout<<"error opening "<<devicePath<<std::endl;
    devicePath = "";
    return false;
  }
  std::cout<<"opened "<<devicePath<<std::endl;
  return true;
}

void korgDevice::closekorgDevice(void)
{
//  int kmem_fd = open((const char*)devicePath.c_str(), O_RDWR);
  int res = close(device_fd);
  if(res < 0)
  {
    std::cout<<"error opening "<<devicePath<<std::endl;
    devicePath = "";
  }
}
void korgDevice::writeFromBuffer2KorgDevice(std::string buffer)
{

//    ssize_t res = pwrite(device_fd, buffer.c_str(), buffer.size(), 0);

//    if(res < 0)
//    {
//        std::cout<<"error writing "<<devicePath<<std::endl;
//    }
//    std::cout<<res<<" bytes written to"<<devicePath<<std::endl;

  ssize_t remaingBytesToWrite = buffer.size();
  std::string dataToWrite;
  ssize_t res = MAX_SIZE;

  while(remaingBytesToWrite > MAX_SIZE)
  {
      ssize_t bytesToWrite = MAX_SIZE - (MAX_SIZE -res); //Nel caso che nella precedente scrittura sia sia scritto un numero inferiore
      dataToWrite = buffer.substr(buffer.size() - remaingBytesToWrite,bytesToWrite);
      res = pwrite(device_fd, dataToWrite.c_str(), dataToWrite.size(),buffer.size() - remaingBytesToWrite);
      if(res < 0)
      {
          std::cout<<"error writing "<<devicePath<<std::endl;
      }
      std::cout<<res<<" bytes written to"<<devicePath<<std::endl;

      remaingBytesToWrite -= res;
  }

  if(remaingBytesToWrite > 0 && buffer.size() > MAX_SIZE)
  {
      ssize_t bytesToWrite = MAX_SIZE - (MAX_SIZE -res);
      dataToWrite = buffer.substr(buffer.size() - remaingBytesToWrite,bytesToWrite);
      res = pwrite(device_fd, dataToWrite.c_str(), dataToWrite.size(), buffer.size() - remaingBytesToWrite);
      if(res < 0)
      {
          std::cout<<"error writing "<<devicePath<<std::endl;
      }
      std::cout<<res<<" bytes written to"<<devicePath<<std::endl;
  }
  else
  {
      ssize_t res = pwrite(device_fd, buffer.c_str(), buffer.size(), 0);
      if(res < 0)
      {
          std::cout<<"error writing "<<devicePath<<std::endl;
      }
      std::cout<<res<<" bytes written to"<<devicePath<<std::endl;
  }


}
void korgDevice::writeFromFile2KorgDevice(std::string filePath)
{
  FILE *streamin;
  streamin = fopen(filePath.c_str(), ("r+b") );
  if( streamin  == NULL )
  {
    std::cout<<"Error opening --> "<<filePath<<std::endl;
    return;
  }

  ssize_t totBytesToWrite = fsize(streamin);
  ssize_t remaingBytesToWrite = totBytesToWrite;
  std::string dataToWrite;
  ssize_t res = MAX_SIZE;

  std::cout<<"totBytesToWrite --> "<<totBytesToWrite<<std::endl;
  std::cout<<"MAX_SIZE --> "<<MAX_SIZE<<std::endl;


  char buffer[MAX_SIZE];
  while(remaingBytesToWrite > MAX_SIZE)
  {
      memset(buffer,0,MAX_SIZE);
      ssize_t bytesToWrite = MAX_SIZE - (MAX_SIZE -res); //Nel caso che nella precedente scrittura sia sia scritto un numero inferiore


      size_t offset = totBytesToWrite - remaingBytesToWrite;

      fseek(streamin,offset,SEEK_SET);
      size_t readBytes = fread( buffer, sizeof( char ), bytesToWrite, streamin );


      std::cout<<"Offset = "<<offset<<" -- "<<"readBytes = "<<readBytes<<std::endl;

      res = pwrite(device_fd, buffer, readBytes, offset);
      if(res < 0)
      {
          std::cout<<"error writing "<<devicePath<<std::endl;
      }
      std::cout<<res<<" bytes written to"<<devicePath<<std::endl;

      remaingBytesToWrite -= res;
  }


  if(remaingBytesToWrite > 0 && totBytesToWrite > MAX_SIZE)
  {
      char buffer[remaingBytesToWrite];
      memset(buffer,0,remaingBytesToWrite);

      size_t offset = totBytesToWrite - remaingBytesToWrite;


      fseek(streamin,offset,SEEK_SET);
      ssize_t readBytes = fread( buffer, sizeof( char ), remaingBytesToWrite, streamin );
      std::cout<<"Offset = "<<offset<<" -- "<<"readBytes = "<<readBytes<<" remaingBytesToWrite = "<<remaingBytesToWrite<<std::endl;

      res = pwrite(device_fd, buffer,readBytes, offset);
      if(res < 0)
      {
          std::cout<<"error writing "<<devicePath<<std::endl;
          return;
      }
      std::cout<<res<<" bytes written to"<<devicePath<<std::endl;
      remaingBytesToWrite -= res;
  }
  else if (totBytesToWrite < MAX_SIZE)
  {
      std::cout<<" totBytesToWrite < MAX_SIZE !!!"<<std::endl;
      char buffer[totBytesToWrite];
      memset(buffer,0,totBytesToWrite);

      fseek(streamin,0,SEEK_SET);
      ssize_t readBytes = fread( buffer, sizeof( char ), totBytesToWrite, streamin );
      std::cout<<readBytes<<" bytes read "<<std::endl;
      res = pwrite(device_fd, buffer, readBytes,0);

      if(res < 0)
      {
          std::cout<<"error writing "<<devicePath<<std::endl;
      }
      std::cout<<res<<" bytes written to"<<devicePath<<std::endl;
      remaingBytesToWrite -= res;
  }

  std::cout<<totBytesToWrite<<" bytes were to be written "<<remaingBytesToWrite<<" bytes are left"<<std::endl;
  if(streamin)
    fclose( streamin );

}


std::string korgDevice::readFromKorgDevice()
{
  if(device_fd < 0)
    return "";

  char test_r[4];

  ssize_t res =  pread(device_fd, (char*)test_r, 4, 0);

  if (res < 0)
  {
      printf("error reading !\n");
      return "";
  }
  std::cout<<res<<" bytes read from"<<devicePath<<std::endl;
  std::string readString(test_r);
  std::cout<<"Read String = "<<test_r<<std::endl;
   printf("Read String = %s\n",test_r);
 //  std::cout<<"Read String size =  "<<readString.size()<<std::endl;

  return readString;

}


std::string korgDevice::readFromKorgDeviceEx()
{

  ssize_t res = MAX_SIZE;
  ssize_t bRead = 0;
  std::string readString;

  char buffer[MAX_SIZE];

  do
  {
      memset(buffer,0,MAX_SIZE);

      ssize_t bytesToRead = MAX_SIZE;
      res = pread(device_fd, (char*)buffer, bytesToRead, bRead);
      bRead += res;
      std::cout<<"res = "<<res<<" bRead = "<<bRead<<std::endl;

      if(res == -1)
      {
          printf("error reading !\n");
          return "";
      }
      if(res > 0)
      {
          std::string app(buffer,res);
          readString += app;
      }

  }while (res);

  std::cout<<"Read String  =  "<<readString<<std::endl;
  std::cout<<"Read String size =  "<<readString.size()<<std::endl;


  return readString;
}




std::string korgDevice::readFileAndMeasure(std::string path,int bufferSize,int* dim,double* elapsed,double* speed)
{
  int fd = open((const char*)path.c_str(), O_RDONLY);


  if(fd < 0)
  {
      std::cout<<"fd = "<<fd<<" Couldn't open "<<path<<std::endl;
      return "";
  }

  int size = fsize(fd);

  *dim = size;

  std::cout<<"size = "<<size<<std::endl;

  ssize_t res = bufferSize;
  ssize_t bRead = 0;
  std::string readString;

  //char buffer[MAX_SIZE];

  char * buffer;
  buffer = (char*) malloc (sizeof(char)*bufferSize);

  TimeStamp tmr;
  int64_t elapsedTime = 0;
  double speedByte = 0;

  system("sh -c \"sync; echo 3 > /proc/sys/vm/drop_caches\""); //AA:Flush cache!!!!!!!!

  do
  {
      memset(buffer,0,bufferSize);

      ssize_t bytesToRead = bufferSize;
      lseek ( fd, bRead, bufferSize);

      tmr.SnapShot();
      res = read(fd,buffer,bytesToRead);
      elapsedTime += tmr.ElapsedTime();
      fsync(fd);

      if(res == -1)
      {
          printf("error reading !\n");
          return "";
      }
      bRead += res;
//      std::cout<<"res = "<<res<<" bRead = "<<bRead<<std::endl;
      if(res > 0)
      {
          std::string app(buffer,res);
          readString += app;
      }

  }while (res);

//  std::cout<<"Read String  =  "<<readString<<std::endl;
  std::cout<<"Read String size =  "<<readString.size()<<std::endl;


  *elapsed = (double)elapsedTime/1000;
  *speed = (double)(size/(*elapsed) / (1024*1024)) ;//MBytes/s



  std::cout<<"Elapsed Time =  "<<*elapsed<<"s"<<std::endl;
  std::cout<<"speed =  "<<*speed<<" MB/s"<<std::endl;

  close(fd);
  free (buffer);

  return readString;
}













