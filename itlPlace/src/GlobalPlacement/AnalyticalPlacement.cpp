#include <windows.h>
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include "math.h"
#include <time.h>
#include <algorithm>
#include <vector>
#include <list>

#include "tao.h"
#include "Auxiliary.h"
#include "Configuration.h"
#include "Clustering.h"
#include "GlobalPlacement.h"
#include "ObjectivesConstraints.h"
#include "Spreading.h"
#include "Utils.h"
#include "PlacementQualityAnalyzer.h"

using namespace std;

timetype expTime = 0;
timetype lseTime = 0;
timetype lseGradTime = 0;
timetype calcPotentialsTime = 0;
timetype quadraticSpreading = 0;
timetype quadraticSpreadingGradTime = 0;

namespace AnalyticalGlobalPlacement
{
  int Relaxation(HDesign& hd, ClusteringInformation& ci);

  void ReportTimes();
  int Solve(HDesign &hd, ClusteringInformation& ci, AppCtx &context, TAO_APPLICATION taoapp, 
    TAO_SOLVER tao, Vec x);

  void UpdateMu( AppCtx &user, HDesign &hd, int iterate );

  int Interpolation(HDesign& hd, ClusteringInformation& ci);

  void InitializeDataStructures(std::vector<Cluster>& clusters, NetList& netList);  
  int InitializeTAO(HDesign& hd, ClusteringInformation &ci, AppCtx &context, Vec& x, Vec& xl, Vec& xu, 
    TAO_SOLVER& tao, TAO_APPLICATION& taoapp);

  void InitializeOptimizationContext(HDesign& hd, ClusteringInformation& ci, AppCtx& context);
  void FreeMemory(AppCtx &user);
 
  void SetVariablesValues(ClusteringInformation & ci, Vec& x);
  void GetVariablesValues(ClusteringInformation& ci, Vec x);
  void SetInitialState(HDesign& hd, ClusteringInformation& ci);
  void UpdateCellsCoordinates(HDesign& hd, ClusteringInformation& ci);
  void WriteCellsCoordinates2Clusters(HDesign& hd, ClusteringInformation& ci);
  void SetClustersCoords(ClusteringInformation& ci, Vec& x);
  void GetClusterCoordinates(ClusteringInformation& ci, Vec x);
  void SetKValues(ClusteringInformation& ci, Vec& x);
  void GetKValues(ClusteringInformation& ci, Vec x);
  void SetBounds(HDesign& hd, ClusteringInformation& ci, Vec& xl, Vec& xu);

  double CalculateSumOfK(HDesign& hd, ClusteringInformation& ci);
  void ReportIterationInfo(ClusteringInformation& ci, AppCtx& user);
  void ReportBinGridInfo(AppCtx& user);
}

using namespace AnalyticalGlobalPlacement;

void AnalyticalPlacement(HDesign& hd)
{
  ConfigContext ctx = hd.cfg.OpenContext("GlobalPlacement");

  WRITELINE("");
  ALERT("ANALYTICAL PLACEMENT STARTED");

  ClusteringInformation ci(hd);
  ci.affinityFunction = Affinity;

  //clustering
  Clustering(hd, ci);
  
  //set initial placement
  SetInitialState(hd, ci);

  //perform placement of clustered netlist
  //TODO: think about reorganization of loop below
  //it is simply loop between clustering levels, why it is so complex?
  Relaxation(hd, ci);

  ClusteringLogIterator clusteringLogIterator = ci.clusteringLog.rbegin();
  NetLevelsIterator netLevelsIterator = ci.netLevels.rbegin();
  if (netLevelsIterator != ci.netLevels.rend()) 
    ++netLevelsIterator;

  for (; clusteringLogIterator != ci.clusteringLog.rend(); ++clusteringLogIterator, ++netLevelsIterator)
  {
    ci.netList = *netLevelsIterator;
    UnclusterLevelUp(hd, ci, clusteringLogIterator);
    Interpolation(hd, ci);
    Relaxation(hd, ci);
  }

  //TODO: consider second v-cycle

  WRITELINE("");
  ALERTFORMAT(("HPWL after analytical placement: %f", Utils::CalculateHPWL(hd, true)));
  ALERT("ANALYTICAL PLACEMENT FINISHED");

  hd.isGlobalPlacementDone = true;
}

int TaoInit(const char* taoCmd)
{
  int argc = 1;
  char** argv = 0;

  //calculate number of arguments
  if (strlen(taoCmd)) 
  {
    argc++;  
    char* ptr = (char*)taoCmd;
    while(ptr = strchr(ptr+1, ' '))
      argc++;
  }

  //initialize array of arguments
  argv = new char*[argc+1];
  int i = 0;

  char path[_MAX_PATH];
  GetModuleFileName(NULL, path, _MAX_PATH);
  argv[i++] = path;

  char seps[] = " \n";
  char *token;  
  token = strtok((char*)taoCmd, seps);  
  while (token != NULL)
  {
    argv[i++] = token;
    token = strtok(NULL, seps);
  }
  argv[i] = NULL;

  PetscInitialize(&argc, &argv, (char *)0, NULL);
  TaoInitialize(&argc, &argv, (char *)0, NULL);

  //int info;       // used to check for functions returning nonzero
  //int size,rank;  // number of processes running
  //info = MPI_Comm_size(PETSC_COMM_WORLD, &size); CHKERRQ(info);
  //info = MPI_Comm_rank(PETSC_COMM_WORLD, &rank); CHKERRQ(info);

  //if (size > 1) {
  //  if (rank == 0)
  //    PetscPrintf(PETSC_COMM_SELF, "This application is intended for single processor use!\n");
  //  SETERRQ(1, "Incorrect number of processors");
  //}

  return OK;
}

void AnalyticalGlobalPlacement::SetInitialState(HDesign& hd, ClusteringInformation& ci)
{
  ALERT("SET INITIAL STATE");

  static double minX = hd.Circuit.PlacementMinX();
  static double maxX = hd.Circuit.PlacementMaxX();
  static double minY = hd.Circuit.PlacementMinY();
  static double maxY = hd.Circuit.PlacementMaxY();
  static double shufflePercent = hd.cfg.ValueOf(".shufflePercent", 0.0);

  int clusterIdx = -1;
  while (GetNextActiveClusterIdx(&ci, clusterIdx))
  {
    ci.clusters[clusterIdx].xCoord = (minX + maxX) / 2.0;
    ci.clusters[clusterIdx].xCoord += 2.0*((double)rand()/(double)RAND_MAX - 0.5)*
      shufflePercent/100.0*hd.Circuit.PlacementWidth();
    ci.clusters[clusterIdx].yCoord = (minY + maxY) / 2.0;
    ci.clusters[clusterIdx].yCoord += 2.0*((double)rand()/(double)RAND_MAX - 0.5)*
      shufflePercent/100.0*hd.Circuit.PlacementHeight();
  }
  UpdateCellsCoordinates(hd, ci);
  ALERTFORMAT(("Initial state HPWL = %f", Utils::CalculateHPWL(hd, true)));

  int netListSize = static_cast<int>(ci.netList.size());
  for (int i = 0; i < netListSize; i++)
  {
    ci.netList[i].k = 0.0;
  }
}

int* InitIdxs(int nVariables, int shift)
{
  int* idxs = new int[nVariables];
  for (int i = 0; i < nVariables; ++i)
  {
    idxs[i] = shift + i;
  }
  return idxs;
}

void AnalyticalGlobalPlacement::SetClustersCoords(ClusteringInformation& ci, Vec& x)
{
  PetscScalar* initValues = new PetscScalar[2*ci.mCurrentNumberOfClusters];
  int idx = 0;
  int clusterIdx = -1;
  while (GetNextActiveClusterIdx(&ci, clusterIdx))
  {
    initValues[2*idx+0] = ci.clusters[clusterIdx].xCoord;
    initValues[2*idx+1] = ci.clusters[clusterIdx].yCoord;
    idx++;
  }

  int* idxs = InitIdxs(2*ci.mCurrentNumberOfClusters, 0);

  VecSetValues(x, 2*ci.mCurrentNumberOfClusters, idxs, initValues, INSERT_VALUES);

  delete[] initValues;
  delete[] idxs;
}

void AnalyticalGlobalPlacement::SetKValues(ClusteringInformation& ci, Vec& x)
{
  int nK = ci.netList.size();

  PetscScalar* initValues = new PetscScalar[nK];
  int* idxs = InitIdxs(nK, 2*ci.mCurrentNumberOfClusters);

  for (int i = 0; i < nK; i++)
  {
    initValues[i] = 0.0;
  }
  VecSetValues(x, nK, idxs, initValues, INSERT_VALUES);

  delete[] initValues;
  delete[] idxs;
}

void AnalyticalGlobalPlacement::SetVariablesValues(ClusteringInformation & ci, Vec& x)
{
  SetClustersCoords(ci, x);
  SetKValues(ci, x);
}

void AnalyticalGlobalPlacement::SetBounds(HDesign& hd, ClusteringInformation& ci, Vec& xl, Vec& xu)
{
  int nVariables = 2 * ci.mCurrentNumberOfClusters + ci.netList.size();
  PetscScalar* initValues = new PetscScalar[nVariables];
  int* idxs = InitIdxs(nVariables, 0);
  int idx;
  int clusterIdx;

  //HPlacementRows::EnumeratorW rIter = hd.PlacementRows.GetEnumeratorW();
  //rIter.MoveNext();
  double siteHeight2 = 0.0;//rIter.SiteHeight() * 0.5;
  double siteWidth2 = 0.0;//rIter.SiteWidth() * 0.5;

  //TODO: change borders according cluster sizes, we have to calculate width
  //and height of each cluster and shift slightly borders for each cluster

  idx = 0;
  clusterIdx = -1;
  while (GetNextActiveClusterIdx(&ci, clusterIdx))
  {
    initValues[2*idx+0] = hd.Circuit.PlacementMinX() + siteWidth2;
    initValues[2*idx+1] = hd.Circuit.PlacementMinY() + siteHeight2;
    idx++;
  }
  for (int i = 2*idx; i < nVariables; i++)
  {//set lower borders for ki variables
    initValues[i] = 0.0;
  }
  VecSetValues(xl, nVariables, idxs, initValues, INSERT_VALUES);

  idx = 0;
  clusterIdx = -1;
  while (GetNextActiveClusterIdx(&ci, clusterIdx))
  {
    initValues[2*idx+0] = hd.Circuit.PlacementMaxX() - siteWidth2;
    initValues[2*idx+1] = hd.Circuit.PlacementMaxY() - siteHeight2;
    idx++;
  }
  for (int i = 2*idx; i < nVariables; i++)
  {//set upper borders for ki variables
    initValues[i] = hd.cfg.ValueOf(".bufferCountUpperBound", 100.0); //FIXME: set correct upper bound
  }
  VecSetValues(xu, nVariables, idxs, initValues, INSERT_VALUES);

  delete [] initValues;
  delete [] idxs;
}

void AnalyticalGlobalPlacement::GetClusterCoordinates(ClusteringInformation& ci, Vec x)
{
  PetscScalar* values = new PetscScalar[2*ci.mCurrentNumberOfClusters];
  int* idxs = InitIdxs(2*ci.mCurrentNumberOfClusters, 0);

  VecGetValues(x, PetscInt(2*ci.mCurrentNumberOfClusters), idxs, values);
  int idx = 0;
  int clusterIdx = -1;
  while (GetNextActiveClusterIdx(&ci, clusterIdx))
  {
    ci.clusters[clusterIdx].xCoord = values[2*idx+0];
    ci.clusters[clusterIdx].yCoord = values[2*idx+1];
    idx++;
  }

  delete[] values;
  delete[] idxs;
}

void AnalyticalGlobalPlacement::GetKValues(ClusteringInformation& ci, Vec x)
{
  int nK = ci.netList.size();
  PetscScalar* values = new PetscScalar[nK];
  int* idxs = InitIdxs(nK, 2*ci.mCurrentNumberOfClusters);

  VecGetValues(x, PetscInt(nK), idxs, values);
  for (int i = 0; i < nK; i++)
  {
    ci.netList[i].k = values[i];
  }

  delete[] values;
  delete[] idxs;
}

void AnalyticalGlobalPlacement::GetVariablesValues(ClusteringInformation& ci, Vec x)
{
  GetClusterCoordinates(ci, x);
  GetKValues(ci, x);
}

void AnalyzeMovementFromInitialPoint(HDesign& hd, ClusteringInformation& ci)
{
  static double minX = hd.Circuit.PlacementMinX();
  static double maxX = hd.Circuit.PlacementMaxX();
  static double minY = hd.Circuit.PlacementMinY();
  static double maxY = hd.Circuit.PlacementMaxY();

  int    nUnmoved = 0;
  int    nMoved = 0;
  double averageMovement = 0.0;
  double movement = 0.0;

  int clusterIdx = -1;
  while (GetNextActiveClusterIdx(&ci, clusterIdx))
  {
    movement = sqrt((ci.clusters[clusterIdx].xCoord - (minX + maxX) / 2.0)*
                    (ci.clusters[clusterIdx].xCoord - (minX + maxX) / 2.0)+
                    (ci.clusters[clusterIdx].yCoord - (minY + maxY) / 2.0)*
                    (ci.clusters[clusterIdx].yCoord - (minY + maxY) / 2.0));
    if (movement < 0.01) //TODO: use macros for comparison
    {
      nUnmoved++;
    }
    else
    {
      nMoved++;
      averageMovement += movement;
    }
  }

  averageMovement /= static_cast<double>(nUnmoved + nMoved);
  ALERTFORMAT(("Number of unmoved clusters %d", nUnmoved));
  ALERTFORMAT(("Average movement %f", averageMovement));
}

void PrintTerminationReason(TaoTerminateReason reason)
{
  char message[256];

  switch(reason)
  {
  case 2:
    strcpy(message, "(residual of optimality conditions <= absolute tolerance)");
    break;
  case 3:
    strcpy(message, "(rtol)"); //residual of optimality conditions / initial residual of optimality conditions <= relative tolerance
    break;
  case 4:
    strcpy(message, "(current trust region size <= trtol)");  
    break;
  case 5:
    strcpy(message, "(function value <= fmin)");  
    break;
  case -2:
    strcpy(message, "(its > maxits)");
    break;
  case -4:
    strcpy(message, "(numerical problems)");
    break;
  case -5:
    strcpy(message, "(number of function evaluations > maximum number of function evaluations)");
    break;
  case -6:
    strcpy(message, "(line search failure)");
    break;
  default:
    strcpy(message, "(unrecognized error)");  
    break;
  }
  ALERTFORMAT(("TAO termination reason = %d %s", reason, message));
}


void AnalyticalGlobalPlacement::UpdateCellsCoordinates(HDesign& hd, ClusteringInformation& ci)
{
  int clusterIdx = -1;
  while (GetNextActiveClusterIdx(&ci, clusterIdx))
  {
    for (int j = 0; j < static_cast<int>(ci.clusters[clusterIdx].cells.size()); ++j)
    {
      HCell cell = ci.clusters[clusterIdx].cells[j]; //Utils::FindCellByName(hd, ci.clusters[clusterIdx].cells[j]);
      hd.Set<HCell::X>(cell, ci.clusters[clusterIdx].xCoord - hd.GetDouble<HCell::Width>(cell)/2.0);
      hd.Set<HCell::Y>(cell, ci.clusters[clusterIdx].yCoord - hd.GetDouble<HCell::Height>(cell)/2.0);
    }
  }
}

void AnalyticalGlobalPlacement::WriteCellsCoordinates2Clusters(HDesign& hd, ClusteringInformation& ci)
{
  int clusterIdx = -1;
  while (GetNextActiveClusterIdx(&ci, clusterIdx))
  {
    HCell cell = ci.clusters[clusterIdx].cells[0]; //Utils::FindCellByName(hd, ci.clusters[clusterIdx].cells[0]);
    ci.clusters[clusterIdx].xCoord = hd.GetDouble<HCell::X>(cell) + hd.GetDouble<HCell::Width>(cell)/2.0;
    ci.clusters[clusterIdx].yCoord = hd.GetDouble<HCell::Y>(cell) + hd.GetDouble<HCell::Height>(cell)/2.0;
  }
}

void AnalyticalGlobalPlacement::UpdateMu(AppCtx& context, HDesign& hd, int iterate)
{
  context.spreadingData.muSpreading     *= hd.cfg.ValueOf("TAOOptions.muSpreadingMultiplier", 2);
  context.muBorderPenalty *= hd.cfg.ValueOf("TAOOptions.muBorderPenaltyMultiplier", 2);

  if (context.useLRSpreading)
  {
    BinGrid& binGrid = context.spreadingData.binGrid;

    for (int i = 0; i < binGrid.nBinRows; ++i)
    {
      for (int j = 0; j < binGrid.nBinCols; ++j)
      {
        context.spreadingData.muBinsPen[i * binGrid.nBinCols + j] += 
          context.spreadingData.muInitial 
          * pow(context.spreadingData.muInitial, 0.2) 
          / pow(2.0, iterate)
          * (binGrid.bins[i][j].sumPotential - context.spreadingData.desiredCellsAreaAtEveryBin)
          * (binGrid.bins[i][j].sumPotential - context.spreadingData.desiredCellsAreaAtEveryBin);
      }
    }
  }
}

int AnalyticalGlobalPlacement::Interpolation(HDesign& hd, ClusteringInformation& ci)
{
  //TODO: consider terminals during interpolation??
  vector<MergeCandidate> clustersData(ci.clusters.size());
  list<MergeCandidate> clustersDataList;
  list<MergeCandidate>::iterator clustersDataIterator;
  bool* isCPoint = new bool[ci.clusters.size()];  // C-point is a cluster which location is fixed during interpolation
  double part = 0.5;  // part of ci.clusters to be initially fixed during interpolation (part of C-points)
  int currClusterIdx;
  int neighborIdx;
  int netIdx;
  vector<int> currCPoints;
  double sumX;
  double sumY;
  double area;

  int* netListSizes = new int[ci.netList.size()]; //TODO: free this memory
  CalculateNetListSizes(ci.netList, netListSizes);

  CalculateScores(hd, ci, clustersData, netListSizes);
  int clusterIdx = -1;
  while (GetNextActiveClusterIdx(&ci, clusterIdx))
  {
    isCPoint[clusterIdx] = false;
  }

  delete[] netListSizes;
  sort(clustersData.begin(), clustersData.end(), PredicateMergePairGreater);

  for (int i = 0; i < static_cast<int>(ci.clusters.size()); ++i)
  {
    if (clustersData[i].score >= 1.5)
      clustersDataList.push_back(clustersData[i]);
    else
      break;
  }
  clustersDataIterator = clustersDataList.begin();
  for (int i = 0; i < static_cast<int>(clustersDataList.size() * part); ++i)
  {
    isCPoint[clustersDataIterator->clusterIdx] = true;
    ++clustersDataIterator;
  }

  for (int i = static_cast<int>(clustersDataList.size() * part); i < static_cast<int>(clustersDataList.size()); ++i)
  {
    currClusterIdx = clustersDataIterator->clusterIdx;
    currCPoints.clear();
    for (int j = 0; j < static_cast<int>(ci.tableOfAdjacentNets[currClusterIdx].size()); ++j)
    {
      netIdx = ci.tableOfAdjacentNets[currClusterIdx][j];
      for (int k = 0; k < static_cast<int>(ci.netList[netIdx].clusterIdxs.size()); ++k)
      {
        neighborIdx = ci.netList[netIdx].clusterIdxs[k];
        //TODO: probably better to consider terminals (primary pins) also
        if (!IsMovableCell(neighborIdx) || isCPoint[neighborIdx] == false)
          continue;
        // remember all placed (fixed) neighbors of current cluster
        currCPoints.push_back(neighborIdx);
      }
    }
    isCPoint[currClusterIdx] = true;
    if (currCPoints.size() == 0)
      continue;
    sumX = sumY = area = 0.0;
    for (int j = 0; j < static_cast<int>(currCPoints.size()); ++j)
    {
      sumX += ci.clusters[currCPoints[j]].area * ci.clusters[currCPoints[j]].xCoord;
      sumY += ci.clusters[currCPoints[j]].area * ci.clusters[currCPoints[j]].yCoord;
      area += ci.clusters[currCPoints[j]].area;
    }
    ci.clusters[currClusterIdx].xCoord = sumX / area;
    ci.clusters[currClusterIdx].yCoord = sumY / area;
    ++clustersDataIterator;
  }

  UpdateCellsCoordinates(hd, ci);

  delete[] isCPoint;

  return OK;
}

void AnalyticalGlobalPlacement::InitializeOptimizationContext(HDesign& hd, ClusteringInformation& ci, AppCtx& context)
{
  context.hd = &hd;
  context.ci = &ci;

  int j = 0;
  context.solutionIdx2clusterIdxLUT = new int[2*CalculateNumberOfActiveClusters(ci) + static_cast<int>(ci.clusters.size())]; 
  context.clusterIdx2solutionIdxLUT = context.solutionIdx2clusterIdxLUT + 2*CalculateNumberOfActiveClusters(ci); 
  for(int i = 0; i < static_cast<int>(ci.clusters.size()); i++)
  {
    if (!ci.clusters[i].isFake)
    {
      context.clusterIdx2solutionIdxLUT[i] = j;
      context.solutionIdx2clusterIdxLUT[2*j+0] = i;
      context.solutionIdx2clusterIdxLUT[2*j+1] = i;
      ++j;
    }
    else
      context.clusterIdx2solutionIdxLUT[i] = -1;
  }

  int nVariables = 2 * context.ci->mCurrentNumberOfClusters + context.ci->netList.size();

  context.gLSE = new double[4*nVariables + 4*context.ci->netList.size()];
  context.gSOD = context.gLSE + nVariables;
  context.gLR  = context.gSOD + nVariables;
  context.gQS  = context.gLR  + nVariables;
  context.SUM1 = context.gQS  + nVariables;
  context.SUM2 = context.SUM1 + context.ci->netList.size();
  context.SUM3 = context.SUM2 + context.ci->netList.size();
  context.SUM4 = context.SUM3 + context.ci->netList.size();

  double L, D;
  Utils::CalcBufferLD(hd, 0, &L, &D);
  context.Lbuf = L;
  context.Dbuf = D;
  context.DbufLbufDifferenceOfSquares = D*D - L*L;
  
  //FIXME: set real values
  context.LRdata.alphaTWL = 1.0;
  context.LRdata.c = 1.0;
  context.LRdata.r = 1.0;
  context.LRdata.mu = hd.cfg.ValueOf(".muLR", 1.0);

  context.gradientBalance       = hd.cfg.ValueOf("TAOOptions.gradientBalance", 1.0);
  
  context.useLogSumExp          = hd.cfg.ValueOf(".useLogSumExp", true);
  context.useSumOfDelays        = hd.cfg.ValueOf(".useSumOfDelays", false);
  context.useLR                 = hd.cfg.ValueOf(".useLR", false);
  context.useQuadraticSpreading = hd.cfg.ValueOf(".useQuadraticSpreading", true);
  context.useLRSpreading        = hd.cfg.ValueOf(".useLRSpreading", false);
  context.useBorderPenalty      = hd.cfg.ValueOf(".useBorderPenalty", true);
  context.useBorderBounds       = hd.cfg.ValueOf(".useBorderBounds", true);
  context.useUnidirectSpreading = hd.cfg.ValueOf(".useUnidirectSpreading", false);
  context.batchSize             = hd.cfg.ValueOf(".batchSizeForvdExp", 1000);
  context.useNetWeights         = hd.cfg.ValueOf("NetWeighting.useNetWeights", false);
}

void AnalyticalGlobalPlacement::FreeMemory(AppCtx &context)
{
  for (int j = 0; j < context.spreadingData.binGrid.nBinRows; ++j)
  {
    delete [] context.spreadingData.binGrid.bins[j];
  }
  delete [] context.spreadingData.binGrid.bins;

  int maxAffectedRows = CalcMaxAffectedArea(context.spreadingData.potentialRadiusY, 
    context.spreadingData.binGrid.binHeight);
  for (int i = 0; i < maxAffectedRows; ++i)
  {
    delete [] context.spreadingData.clusterPotentialOverBins[i];
  }
  delete [] context.spreadingData.clusterPotentialOverBins;
  
  if (context.useLRSpreading)
  {
    delete [] context.spreadingData.binsPenaltyValues;
    delete [] context.spreadingData.muBinsPen;
    delete [] context.spreadingData.individualBinsDiscrepancy;
  }

  //FIXME: check correctness of memory deletion
  delete[] context.precalcedExponents;
  delete[] context.netListSizes;
  delete[] context.solutionIdx2clusterIdxLUT;
  delete[] context.gLSE;
}

void AnalyticalGlobalPlacement::ReportBinGridInfo(AppCtx& context)
{
  ALERTFORMAT(("Bin grid: %d x %d", 
    context.spreadingData.binGrid.nBinRows, 
    context.spreadingData.binGrid.nBinCols));
  ALERTFORMAT(("Bin width: %f\tBin height: %f", 
    context.spreadingData.binGrid.binWidth, 
    context.spreadingData.binGrid.binHeight));
  ALERTFORMAT(("Potential radius X: %f\tPotential radius Y: %f", 
    context.spreadingData.potentialRadiusX, 
    context.spreadingData.potentialRadiusY));
}

void AnalyticalGlobalPlacement::ReportIterationInfo(ClusteringInformation& ci, AppCtx& user)
{
  WRITELINE("");
  ALERTFORMAT(("RELAXATION ITERATION STARTED"));
  ALERTFORMAT(("Number of clusters: %d", ci.mCurrentNumberOfClusters));
  ALERTFORMAT(("Number of nets: %d", ci.netList.size()));
}

int AnalyticalGlobalPlacement::InitializeTAO(HDesign& hd, ClusteringInformation &ci, AppCtx &context, 
                                             Vec& x, Vec& xl, Vec& xu, TAO_SOLVER& tao, TAO_APPLICATION& taoapp)
{
  const char* taoCmd = hd.cfg.ValueOf("TAOOptions.commandLine");
  TaoInit(taoCmd);
  int info;

  /* Create TAO solver with desired solution method */
  info = TaoCreate(PETSC_COMM_SELF, hd.cfg.ValueOf("TAOOptions.method"), &tao); CHKERRQ(info);
  info = TaoApplicationCreate(PETSC_COMM_SELF, &taoapp); CHKERRQ(info);

  // Allocate vectors for the solution and gradient
  int nVariables = 2*ci.mCurrentNumberOfClusters + ci.netList.size();
  info = VecCreateSeq(PETSC_COMM_SELF, nVariables, &x); CHKERRQ(info);
  // Set solution vec and an initial guess
  SetVariablesValues(ci, x);

  hd.Plotter.ShowGlobalPlacement(false, context.spreadingData.binGrid.nBinRows, 
    context.spreadingData.binGrid.nBinCols, HPlotter::WAIT_1_SECOND);

  info = TaoAppSetInitialSolutionVec(taoapp, x); CHKERRQ(info);

  //set bounds
  if (context.useBorderBounds)
  {
    info = VecCreateSeq(PETSC_COMM_SELF, nVariables, &xl); CHKERRQ(info);
    info = VecCreateSeq(PETSC_COMM_SELF, nVariables, &xu); CHKERRQ(info);
    SetBounds(hd, ci, xl, xu);
    info = TaoAppSetVariableBounds(taoapp, xl, xu);
  }

  info = TaoAppSetObjectiveAndGradientRoutine(taoapp, AnalyticalObjectiveAndGradient, (void*)&context);
  CHKERRQ(info);

  int nInnerIterations = hd.cfg.ValueOf("TAOOptions.nInnerIterations", 1000);
  info = TaoSetMaximumIterates(tao, nInnerIterations); CHKERRQ(info);
  double fatol = hd.cfg.ValueOf("TAOOptions.fatol", 1.0e-14);
  double frtol = hd.cfg.ValueOf("TAOOptions.frtol", 1.0e-14);
  double catol = hd.cfg.ValueOf("TAOOptions.catol", 1.0e-8);
  double crtol = hd.cfg.ValueOf("TAOOptions.crtol", 1.0e-8);
  info = TaoSetTolerances(tao, fatol, frtol, catol, crtol); CHKERRQ(info);

  /* Check for TAO command line options */
  info = TaoSetOptions(taoapp, tao); CHKERRQ(info);

  /* Get the mu value */
  PetscScalar* solution;
  VecGetArray(x, &solution);
  CalcMuInitial(solution, &context);
  VecRestoreArray(x, &solution);  
  
  return info;
}

void AnalyticalGlobalPlacement::ReportTimes()
{
  ALERTFORMAT(("EXP Calc time = %f", GETSECONDSFROMTIME(expTime)));
  ALERTFORMAT(("lseTime = %f", GETSECONDSFROMTIME(lseTime)));
  ALERTFORMAT(("lseGradTime = %f", GETSECONDSFROMTIME(lseGradTime)));
  ALERTFORMAT(("calcPotentialsTime = %f", GETSECONDSFROMTIME(calcPotentialsTime)));
  ALERTFORMAT(("quadraticSpreading = %f", GETSECONDSFROMTIME(quadraticSpreading)));
  ALERTFORMAT(("quadraticSpreadingGradTime = %f", GETSECONDSFROMTIME(quadraticSpreadingGradTime)));
  expTime = lseTime = lseGradTime = calcPotentialsTime = quadraticSpreading = quadraticSpreadingGradTime;
}

int AnalyticalGlobalPlacement::Relaxation(HDesign& hd, ClusteringInformation& ci)
{
  Vec             x;          // solution vector
  Vec             xl, xu;     // bounds vectors
  AppCtx          context;    // context-defined application context
  TAO_SOLVER      tao;        // TAO_SOLVER solver context
  TAO_APPLICATION taoapp;     // TAO application context
  int             retCode;    // used to check for functions returning nonzero

  //INITIALIZE OPTIMIZATION PROBLEM PARAMETERS
  InitializeOptimizationContext(hd, ci, context);
  ConstructBinGrid(hd, context, hd.cfg.ValueOf("Clustering.desiredNumberOfClustersAtEveryBin", 10));
  retCode = InitializeTAO(hd, ci, context, x, xl, xu, tao, taoapp); CHKERRQ(retCode);
  ReportIterationInfo(ci, context);
  ReportBinGridInfo(context);

  //SOLVE THE PROBLEM
  retCode = Solve(hd, ci, context, taoapp, tao, x); CHKERRQ(retCode);
  ReportTimes();

  //Free TAO&PETSc data structures
  retCode = TaoDestroy(tao); CHKERRQ(retCode);
  retCode = TaoAppDestroy(taoapp); CHKERRQ(retCode);
  retCode = VecDestroy(x); CHKERRQ(retCode);
  if (context.useBorderBounds)
  {
    retCode = VecDestroy(xl); CHKERRQ(retCode);
    retCode = VecDestroy(xu); CHKERRQ(retCode);
  }
  FreeMemory(context);

  return OK;
}

int AnalyticalGlobalPlacement::Solve(HDesign& hd, ClusteringInformation& ci, AppCtx& context, 
                                     TAO_APPLICATION taoapp, TAO_SOLVER tao, Vec x)
{
  int retCode;
  double discrepancy = 100.0; //NOTE: dummy big value
  static int metaIteration = 1;

  static int nOuterIters = hd.cfg.ValueOf("TAOOptions.nOuterIterations", 32);
  ALERTFORMAT(("hd.isGlobalPlacementDone = %s", hd.isGlobalPlacementDone ? "true" : "false"));
  if (hd.isGlobalPlacementDone)
  { // reload the value (we're in the beginning of the GP)
    nOuterIters = hd.cfg.ValueOf("TAOOptions.nOuterIterations", 32);
    hd.isGlobalPlacementDone = false;
  }

  PlacementQualityAnalyzer* QA = 0;
  if(hd.cfg.ValueOf(".useQAClass", false))
  {
    QA = new PlacementQualityAnalyzer(hd);
    if (hd.cfg.ValueOf(".earlyExit", false))
    {
      QA->SaveCurrentPlacementAsBestAchieved();
    }
  }

  int iteration = 1;
  static double targetDiscrepancy = hd.cfg.ValueOf(".targetDiscrepancy", 2.0);
  while (1)
  {
    //print iteration info
    WRITELINE("");
    ALERTFORMAT(("TAO iteration %d.%d", metaIteration, iteration++));
    if (context.useQuadraticSpreading)
      ALERTFORMAT(("muSpreading = %.20f", context.spreadingData.muSpreading));
    if (context.useBorderPenalty)
      ALERTFORMAT(("muBorderPenalty = %.20f", context.muBorderPenalty));
    ALERTFORMAT(("HPWL initial   = %f", Utils::CalculateHPWL(hd, true)));

    hd.Plotter.ShowGlobalPlacement(hd.cfg.ValueOf(".plotWires", false), 
      context.spreadingData.binGrid.nBinRows, context.spreadingData.binGrid.nBinCols);

    // Tao solve the application
    retCode = TaoSolveApplication(taoapp, tao); CHKERRQ(retCode);

    //Get termination information
    int innerTAOIterations;
    double f;
    double gnorm;
    double cnorm;
    double xdiff;
    TaoTerminateReason reason;  // termination reason
    retCode = TaoGetSolutionStatus(tao, &innerTAOIterations, &f, &gnorm, &cnorm, &xdiff, &reason); CHKERRQ(retCode);
    PrintTerminationReason(reason);

    // if found any solution
    if (innerTAOIterations >= 1)
    {
      GetVariablesValues(ci, x);
      UpdateCellsCoordinates(hd, ci);
      discrepancy = CalculateDiscrepancy(x, &context);
    }

    UpdateMu(context, hd, 32); //TODO: ISSUE 18 COMMENT 12

    //print iteration info
    ALERTFORMAT(("discrepancy = %f", discrepancy));
    ALERTFORMAT(("Sum of Ki = %f", CalculateSumOfK(hd, ci)));
    
    if (hd.cfg.ValueOf(".useQAClass", false))
    {
      QA->AnalyzeQuality(iteration - 2);
      if (hd.cfg.ValueOf(".earlyExit", false))
        if (!QA->IsNextIterationApproved())
        {
          QA->RestoreBestAchievedPlacement();
          WriteCellsCoordinates2Clusters(hd, ci);
          ALERTFORMAT(("Early exit called"));
          break;
        }
    }
    if (discrepancy <= targetDiscrepancy)
    {
      ALERTFORMAT(("Discrepancy achieved"));
      break;
    }
    if (iteration > nOuterIters)
    {
      ALERTFORMAT(("Iterations finished"));
      break;
    }
  }

  metaIteration++;

  //QA report
  if (hd.cfg.ValueOf(".useQAClass", false))
  {
    if (QA->IsAcceptableImprovementAchieved() || !hd.cfg.ValueOf(".earlyExit", false))
      QA->AnalyzeQuality(iteration);
    QA->Report();
    delete QA;
  }

  return OK;
}

double AnalyticalGlobalPlacement::CalculateSumOfK(HDesign& hd, ClusteringInformation& ci)
{
  double s = 0.0;
  int netListSize = static_cast<int>(ci.netList.size());
  for (int i = 0; i < netListSize; i++)
  {
    s += ci.netList[i].k;
  }
  return s;
}
