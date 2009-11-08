#ifndef __OPTIMIZATION_CONTEXT__
#define __OPTIMIZATION_CONTEXT__

#include "HDesign.h"

#include "Clustering.h"
#include "SpreadingData.h"
#include "LagrangianRelaxationData.h"

// User-defined TAO application context - contains data needed by the 
// application-provided call-back routines that evaluate the function and gradient
struct AppCtx
{
  HDesign*                hd;
  ClusteringInformation*  ci;

  SpreadingData spreadingData;

  LRData    LRdata;
  
  int*      clusterIdx2solutionIdxLUT;
  int*      solutionIdx2clusterIdxLUT;

  double    alpha;

  double    muBorderPenalty;
  double    borderPenaltyVal;

  double*   precalcedExponents;
  double*   argsForPrecalcedExponents;
  int       batchSize;                // quantity of exponents calculated at a time by vdExp

  double* SUM1;
  double* SUM2;
  double* SUM3;
  double* SUM4;

  int*      netListSizes;

  double*   gLSE;
  double*   gSOD;
  double*   gLR;
  double*   gQS;

  double    Lbuf;
  double    Dbuf;
  double    DbufLbufDifferenceOfSquares;
  double    gradientBalance;

  bool useLogSumExp;
  bool useSumOfDelays;
  bool useLR;

  bool useQuadraticSpreading;
  bool useLRSpreading;

  bool useBorderPenalty;
  bool useBorderBounds;
  bool useUnidirectSpreading;

  bool useNetWeights;

  //constructor
  AppCtx()
  {
    hd           = NULL;
    ci           = NULL;
    clusterIdx2solutionIdxLUT = 0;
    solutionIdx2clusterIdxLUT = 0;

    gLSE = 0;
    gSOD = 0;
    gLR  = 0;
    gQS  = 0;

    Lbuf = 0;
    DbufLbufDifferenceOfSquares = 0;
  }
};

#endif