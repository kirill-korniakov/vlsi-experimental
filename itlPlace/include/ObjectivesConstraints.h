#ifndef _DETAILED_PLACEMENT_H_
#define _DETAILED_PLACEMENT_H_

#include "tao.h"
#include "Auxiliary.h"
#include "HDesign.h"
#include "Clustering.h"

struct Bin
{
  double xCoord;             /// x coordinates of the bin.
  double yCoord;             /// y coordinates of the bin.
  double sumPotential;

  Bin()
  {
    xCoord = 0.0; yCoord = 0.0;
    sumPotential = 0.0;
  }
};

struct BinGrid
{
  Bin**   bins;
  double  binHeight;
  double  binWidth;
  int     nBinRows;
  int     nBinCols;
  int     nBins;
};

struct LRData
{
  double    alphaTWL;
  double    c;
  double    r;
  double    mu;

  LRData()
  {
    alphaTWL = 0.0;
    mu = 0.0;
    c = 0.0;
    r = 0.0;
  }
};

struct SpreadingData
{
  BinGrid   binGrid;

  double    totalCellArea;
  double    desiredCellsAreaAtEveryBin;
  double**  clusterPotentialOverBins;
  double    potentialRadiusX;
  double    potentialRadiusY;
  double    invPSX;
  double    invPSY;

  double    muInitial;
  double    muSpreading;
  double*   muBinsPen;

  double*   binsPenaltyValues;
  double*   individualBinsDiscrepancy;

  SpreadingData()
  {
    binGrid.bins = 0;
    clusterPotentialOverBins = 0;
  }
};

// User-defined TAO application context - contains data needed by the 
// application-provided call-back routines that evaluate the function and gradient
struct AppCtx
{
  HDesign*                hd;
  ClusteringInformation*  ci;

  int*      clusterIdx2solutionIdxLUT;
  int*      solutionIdx2clusterIdxLUT;

  double    alpha;

  SpreadingData spreadingData;

  double    muBorderPenalty;
  double    borderPenaltyVal;

  double*   precalcedExponents;
  double*   argsForPrecalcedExponents;
  int       batchSize;                // quantity of exponents calculated at a time by vdExp
  
  int*      netListSizes;
  
  double*   gLSE;
  double*   gSOD;
  double*   gLR;
  double*   gQS;

  LRData    LRdata;

  double    Lbuf;
  double    Dbuf;
  double    DbufLbufDifferenceOfSquares;
  double    gradientBalance;

  double* SUM1;
  double* SUM2;
  double* SUM3;
  double* SUM4;

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

int AnalyticalObjectiveAndGradient(TAO_APPLICATION taoapp, Vec X, double* f, Vec G, void* ptr);
void CalcMuInitial(PetscScalar* x, AppCtx* context);

#endif