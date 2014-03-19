#ifndef STATISTICS_H
#define STATISTICS_H

#include <vector>
#include <iostream>
#include <string>

class Statistics
{
public:
  Statistics(std::string uM);

  void addMeasure(double mes);
  bool getAvgAndVariance(double* average,double* StandardDeviation);
  bool getMaxAndMin(double* Max, double* Min);
  int getSamples();
  std::string getUM();

  void setLog( bool enLg)
  {
    enableLog = enLg;
  }

private:
  bool enableLog;
  std::vector<double> measures;
  std::string unitMeasurament;

};

#endif // STATISTICS_H
