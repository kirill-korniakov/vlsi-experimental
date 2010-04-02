#ifndef __OPTIMIZATION_CONTEXT__
#define __OPTIMIZATION_CONTEXT__

#include "HDesign.h"

#include "Clustering.h"
#include "LogSumExpData.h"
#include "SumOfDelaysData.h"
#include "LagrangianRelaxationData.h"
#include "SpreadingData.h"

// User-defined TAO application context - contains data needed by the 
// application-provided call-back routines that evaluate the function and gradient
struct AppCtx
{
  void Initialize(HDesign& ahd, ClusteringInformation& aci);
  void FreeMemory();

  HDesign*                hd;
  ClusteringInformation*  ci;

  LSEData   LSEdata;
  SODData   SODdata;
  LRData    LRdata;
    
  SpreadingData spreadingData;
  
  double    muBorderPenalty;
  double    borderPenaltyVal;

  int*      clusterIdx2solutionIdxLUT;
  int*      solutionIdx2clusterIdxLUT;

  int*      netListSizes;

  double*   gLSE;
  double*   gSOD;
  double*   gLR;
  double*   gQS;

  bool useLogSumExp;
  bool useSumOfDelays;
  bool useLR;

  bool useQuadraticSpreading;
  bool useLRSpreading;

  bool useBorderPenalty;
  bool useBorderBounds;
  bool useUnidirectSpreading;

  bool useNetWeights;
};

#endif