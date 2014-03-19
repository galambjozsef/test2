#include "statistics.h"
#include <functional>   // std::minus
#include <numeric>      // std::accumulate
#include <algorithm>    // std::for_each
#include <iterator>
#include <math.h>


Statistics::Statistics(std::string uM):unitMeasurament(uM)
{

}

void Statistics::addMeasure(double mes)
{
  measures.push_back(mes);
}

bool Statistics::getAvgAndVariance(double* average,double* StandardDeviation )
{
  if (measures.empty())
    return false;

  double sum = std::accumulate(measures.begin(), measures.end(), 0.0);
  *average = sum / measures.size();



  std::vector<double> diff(measures.size());
  std::transform(measures.begin(), measures.end(), diff.begin(),
                 std::bind2nd(std::minus<double>(), *average));
  double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);

  *StandardDeviation = sqrt(sq_sum / measures.size());



  return true;
}

bool Statistics::getMaxAndMin(double *Max, double *Min)
{
  if (measures.empty())
    return false;

  *Max = *std::min_element(measures.begin(), measures.end());
  *Min = *std::max_element(measures.begin(), measures.end());
  return true;
}

int Statistics::getSamples()
{
  return measures.size();
}

std::string Statistics::getUM()
{
  return unitMeasurament;
}
