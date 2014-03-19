#ifndef DEVICE_H
#define DEVICE_H

#include <string>

class korgDevice
{
public:
  korgDevice(){}
  korgDevice(std::string dPath);
  virtual ~korgDevice(){}


  void openkorgDevice(std::string);
  void writeFromBuffer2KorgDevice(std::string);
  void writeFromFile2KorgDevice(std::string);
  std::string readFromKorgDevice();
  std::string readFromKorgDeviceEx();
  std::string readFileAndMeasure(std::string path,int bufferSize,int* dim,double* elapsed,double* speed);

private:
  std::string devicePath;
  int device_fd;
};

#endif // DEVICE_H
