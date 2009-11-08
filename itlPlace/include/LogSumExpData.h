#ifndef __LOG_SUM_EXP_DATA__
#define __LOG_SUM_EXP_DATA__

#include "HDesign.h"
#include "Clustering.h"

class LSEData
{
public:
  double   alpha;

  double*  precalcedExponents;
  double*  argsForPrecalcedExponents;
  int      batchSize;                // quantity of exponents calculated at a time by vdExp

  double* SUM1;
  double* SUM2;
  double* SUM3;
  double* SUM4;

  void Initialize(HDesign& hd, ClusteringInformation& ci, double binWidth);
};

#endif