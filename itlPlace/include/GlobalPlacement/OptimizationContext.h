#ifndef __OPTIMIZATION_CONTEXT__
#define __OPTIMIZATION_CONTEXT__

#include "HBaseDesignItem.h"

#include "Clustering.h"
#include "LogSumExpData.h"
#include "SumOfDelaysData.h"
#include "LagrangianRelaxationData.h"
#include "SpreadingData.h"

struct CriteriaValues
{
    double objective;

    double hpwl;
    double sod;
    double spreading;
    double lr;

    double gHPWL;
    double gSpr;
    double gLR;
};

// User-defined TAO application context - contains data needed by the 
// application-provided call-back routines that evaluate the function and gradient
struct AppCtx
{
  void Initialize(HDesign& ahd, ClusteringInformation& aci);
  void FreeMemory();

  HDesign*                hd;
  ClusteringInformation*  ci;

  int nVariables;

  LSEData   LSEdata;
  SODData   SODdata;
  LRData    LRdata;
  SpreadingData spreadingData;
  
  double    borderPenaltyVal;

  int*      clusterIdx2solutionIdxLUT;
  int*      solutionIdx2clusterIdxLUT;

  int*      netListSizes;

  CriteriaValues criteriaValues;
  double*   gLSE;
  double*   gSOD;
  double*   gLR;
  double*   gQS;

  bool useLogSumExp;
  bool useSumOfDelays;
  bool useLR;

  bool useQuadraticSpreading;
  bool useLRSpreading;
  bool useUnidirectSpreading;

  bool useBorderBounds;

  bool useNetWeights;
};

#endif