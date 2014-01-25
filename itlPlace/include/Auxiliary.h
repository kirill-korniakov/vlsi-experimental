#ifndef __AUXILIARY_H__
#define __AUXILIARY_H__

#include "math.h"
#include "stdTypes.h"

#include "Grow.h"

namespace Aux
{
  static const std::string SciFormat = "%.2e";

  string CreateCoolFileName(string dirName, string fileName, string extension);
  string IntToString(int intValue, int length = 0);
  string ChangeExtention(const string& filename, const char* newExt);
  bool FileExists(const string& strFilename);
  unsigned GetFileSize(const string& strFilename);
  string Format(const char* format, ...);
  double correlation(double* x, double* y, int n);

  //linear regression for two variables
  void MNK(const double *index_1,
            const double *index_2,
            const double *values,
            const int n, //length of index_1
            const int m, //length of index_2
            double& a0,
            double& a1,
            double& a2);

  
  template<class T>
  T Factorial(T n)
  {
    T f = T(1);
    for (T i = T(2); i <= n; i += T(1)) f *= i;
    return f;
  }

  template<class T>
  void NextPermutation(T hash[], unsigned int length)
  {
    if (length > 2)
    {
      unsigned int k = length - 2;
      while (k > 0 && ( hash[k] > hash[k + 1])) --k;

      if ((k == 0) && (hash[0] > hash[1]))
      {
        //inverse hash
        for (unsigned int i = 0; i < length / 2; ++i)
          ::Swap(hash + i, hash + length - i - 1);
        return;
      }

      unsigned int t = k + 1;
      while ((t < length - 1) && (hash[t + 1] > hash[k])) ++t;
      
      ::Swap(hash + k, hash + t);

      unsigned int n = (length - k - 1) / 2;
      for (unsigned int i = 1; i <= n; ++i)
        ::Swap(hash + k + i, hash + length - i);
    }
    else if (length == 2)
      ::Swap(hash + 0, hash + 1);
  }

  inline int cool_dtoi(double x)
  {
    return (int)floor(x + 0.5);
  }

  inline int sign(double x)
  {
    return x > 0 ? 1 : (x < 0) ? -1 : 0;
  }

  template<class MultipliersType>
  MultipliersType ScalarProduct(std::vector<MultipliersType> vec1, std::vector<MultipliersType> vec2)
  {
    int minSize = std::min((int)vec1.size(), (int)vec2.size());
    MultipliersType sum = 0.0;

    int i = 0;
    for (; i < minSize; ++i)
    {
      sum += vec1[i] * vec2[i];
    }

    return sum;
  }

  template<class MultipliersType>
  MultipliersType ScalarProduct(MultipliersType* vec1, MultipliersType* vec2, int size)
  {
    MultipliersType sum = 0.0;

    int i = 0;
    for (; i < size; ++i)
    {
      sum += vec1[i] * vec2[i];
    }

    return sum;
  }
}
#endif //__AUXILIARY_H__