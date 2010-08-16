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

    double lse; double* gLSE; double gLSENorm; 
    double sod; double* gSOD;                  
    double spr; double* gQS;  double gSprNorm; 
    double lr;  double* gLR;  double gLRNorm;  
};

struct Weights
{
    double lseW;
    double sodW;
    double lrW;
    double sprW;
};

// User-defined TAO application context - contains data needed by the 
// application-provided call-back routines that evaluate the function and gradient
struct AppCtx
{
  void Initialize(HDesign& ahd, ClusteringInformation& aci);
  void FreeMemory();

  HDesign*                hd;
  ClusteringInformation*  ci;

  int   nVariables;
  int*  netListSizes;  
  int*  clusterIdx2solutionIdxLUT;
  int*  solutionIdx2clusterIdxLUT;
  
  LSEData   LSEdata;
  SODData   SODdata;
  LRData    LRdata;
  SprData   sprData;

  CriteriaValues criteriaValues;
  Weights weights;

  bool useLogSumExp;
  bool useSumOfDelays;
  bool useLR;

  bool useQuadraticSpreading;

  bool useBorderBounds;

  //TODO: possibly we can eliminate this parameter
  bool useNetWeights;
};

#endif