#include "HDesign.h"
#include "Spreading.h"
#include "SpreadingData.h"
#include "OptimizationContext.h"

using namespace std;

void DetermineDimensionsOfBinGrid(HDesign& hd, vector<Cluster>& clusters, 
                                  const int nClusters, BinGrid& binGrid,
                                  int desiredNumberOfClustersAtEveryBin)
{
  static double minX = hd.Circuit.PlacementMinX();
  static double maxX = hd.Circuit.PlacementMaxX();
  static double minY = hd.Circuit.PlacementMinY();
  static double maxY = hd.Circuit.PlacementMaxY();

  double circuitAspectRatio = (maxX - minX) / (maxY - minY);

  binGrid.nBinRows = static_cast<int>(sqrt(circuitAspectRatio * nClusters/desiredNumberOfClustersAtEveryBin));
  binGrid.nBinCols = static_cast<int>(Aux::dtoi(circuitAspectRatio * binGrid.nBinRows));

  //WARNING: we need odd numbers, because cells will not move otherwise
  if (binGrid.nBinRows % 2 == 0) binGrid.nBinRows++; 
  if (binGrid.nBinCols % 2 == 0) binGrid.nBinCols++;

  binGrid.binWidth = (maxX - minX) / binGrid.nBinCols;
  binGrid.binHeight = (maxY - minY) / binGrid.nBinRows;
}

void ConstructBinGrid(HDesign& hd, AppCtx& context, int aDesiredNumberOfClustersAtEveryBin)
{
  static int desiredNumberOfClustersAtEveryBin;
  if (aDesiredNumberOfClustersAtEveryBin != -1)
    desiredNumberOfClustersAtEveryBin = aDesiredNumberOfClustersAtEveryBin;
  else
    desiredNumberOfClustersAtEveryBin--;

  BinGrid& binGrid = context.spreadingData.binGrid;
  double& potentialRadiusX = context.spreadingData.potentialRadiusX;
  double& potentialRadiusY = context.spreadingData.potentialRadiusY;
  double& invPSX = context.spreadingData.invPSX;
  double& invPSY = context.spreadingData.invPSY;
  double**& clusterPotentialOverBins = context.spreadingData.clusterPotentialOverBins;

  // Calculate current bin grid
  DetermineDimensionsOfBinGrid(hd, context.ci->clusters, context.ci->mCurrentNumberOfClusters, 
    binGrid, desiredNumberOfClustersAtEveryBin);

  //TODO: correct this potential radius calculation
  double potentialRatio = hd.cfg.ValueOf("GlobalPlacement.potentialRatio", 2.1); //WARNING: must be greater than 0.5
  potentialRadiusX = potentialRatio*binGrid.binWidth;
  potentialRadiusY = potentialRatio*binGrid.binHeight;
  invPSX = 1 / potentialRadiusX / potentialRadiusX;
  invPSY = 1 / potentialRadiusY / potentialRadiusY;

  int maxAffectedRows = CalcMaxAffectedArea(potentialRadiusY, binGrid.binHeight);
  int maxAffectedCols = CalcMaxAffectedArea(potentialRadiusX, binGrid.binWidth);

  //free memory from previous iteration
  //FIXME: last iteration causes memory leak
  if (clusterPotentialOverBins)
  {
    for (int i = 0; i<maxAffectedRows; i++)
    {
      if (clusterPotentialOverBins[i])
        delete [] clusterPotentialOverBins[i];
    }
    delete [] clusterPotentialOverBins;
  }
  if (binGrid.bins)
  {
    for (int j = 0; j < binGrid.nBinRows; ++j)
    {
      if (binGrid.bins[j])
        delete [] binGrid.bins[j];
    }
    delete [] binGrid.bins;
  }

  //allocate memory
  clusterPotentialOverBins = new double*[maxAffectedRows];
  for (int i = 0; i < maxAffectedRows; i++)
  {
    clusterPotentialOverBins[i] = new double[maxAffectedCols];
  }
  binGrid.bins = new Bin*[binGrid.nBinRows];
  for (int j = 0; j < binGrid.nBinRows; ++j)
  {
    binGrid.bins[j] = new Bin[binGrid.nBinCols];
  }

  for (int i = 0; i < binGrid.nBinRows; ++i)
  {
    for (int j = 0; j < binGrid.nBinCols; ++j)
    {
      binGrid.bins[i][j].xCoord = (j + 0.5) * binGrid.binWidth;
      binGrid.bins[i][j].yCoord = (i + 0.5) * binGrid.binHeight;
    }
  }
  //set coordinates of bins from lower and upper border
  for (int j = 0; j < binGrid.nBinCols; ++j)
  {
    binGrid.bins[0][j].yCoord = 0.0;
    binGrid.bins[binGrid.nBinRows-1][j].yCoord = binGrid.binHeight * binGrid.nBinRows;
  }
  //set coordinates of bins from left and right border
  for (int i = 0; i < binGrid.nBinRows; ++i)
  {
    binGrid.bins[i][0].xCoord = 0.0;
    binGrid.bins[i][binGrid.nBinCols-1].xCoord = binGrid.binWidth * binGrid.nBinCols;
  }

  binGrid.nBins = binGrid.nBinCols * binGrid.nBinRows;

  context.spreadingData.totalCellArea = 0.0;
  for (int i = 0; i < static_cast<int>(context.ci->clusters.size()); ++i)
  {
    if (context.ci->clusters[i].isFake == false)
    {
      context.spreadingData.totalCellArea += context.ci->clusters[i].area;
    }
  }
  context.spreadingData.desiredCellsAreaAtEveryBin = 
    context.spreadingData.totalCellArea / binGrid.nBins;

  //if (useNetWeights)
  {
    int nNets = context.ci->netList.size();//hd.Nets.Count();
    HNets::ActiveNetsEnumeratorW netW = hd.Nets.GetNetsEnumeratorW();

    for (int i = 0; i < nNets && netW.MoveNext(); ++i)
    {
      context.ci->netList[i].weight = netW.Weight();
    }
  }
  if (context.useLRSpreading)
  {
    context.borderPenaltyVal    = 0.0;
    context.spreadingData.binsPenaltyValues   = new double[binGrid.nBins * 3];
    context.spreadingData.muBinsPen = 
      context.spreadingData.binsPenaltyValues + binGrid.nBins;
    context.spreadingData.individualBinsDiscrepancy = 
      context.spreadingData.binsPenaltyValues + binGrid.nBins*2;
  }
}