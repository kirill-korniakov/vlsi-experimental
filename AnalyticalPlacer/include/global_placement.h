/*  
 * global_placement.h
 * this is a part of itlDragon
 * Copyright (C) 2005, 2006, ITLab, Aleksey Bader, Kirill Kornyakov
 * email: bader@newmail.ru
 * email: kirillkornyakov@yandex.ru
 */

#ifndef _GLOBAL_PLACEMENT_H_
#define _GLOBAL_PLACEMENT_H_

#include "..\include\data_structures.h"
#include "..\include\cost_function.h"
#include "..\include\bin_swapping.h"
#include "..\include\errors.h"
#include "..\include\check_legality.h"
#include "..\include\optimal_region_determination.h"
#include "..\include\overlap_removing.h"
#include "..\include\global.h"
#include <list>
 
// function from libhmetis for hypergraph partitioning
extern "C"{
extern void HMETIS_PartRecursive( int nvtxs, int nhedges, int *vwgts, int *eptr,
                  int *eind, int *hewgts, int nparts, 
                  int ubfactor, int *options, int *part, 
                  int *edgecut );
//extern void METIS_WPartGraphRecursive(int* n, int* xadj, int* adjncy, int* vwgt, int* adjwgt, int* wgt, 
//                                      int* numflag, int* nparts, float* tpwgts, int* options, int* edgecut, int* part);
}

namespace Coarsening
{
  struct Cluster
  {
    vector<int> cellIdxs;
    double xCoord;
    double yCoord;
    double area;
    bool   isValid;
    bool   isFake;  // needed for validation
    Cluster()
    {
      xCoord  = yCoord = area = 0.0;
      isFake  = false;
      isValid = true;
    }
  };
  
  struct ClusterData
  {
    double score;
    int bestNeighborIdx;
    int clusterIdx;
    ClusterData()
    {
      score = 0.0;
      bestNeighborIdx = 0;
      clusterIdx = 0;
    }
  };

  struct ClusteredNodes
  {
    int c1;
    int c2;
    size_t size;  // end index of clusters in "vector<int> cellIdxs" of the result cluster
  };
  
  typedef ITLDRAGON_ERROR (*affinityFunc)(int& c1, int& c2, vector<Cluster>& clusters, vector<vector<int>>& netList, 
                                          vector<int>* currTableOfConnections, double* netAreas, double& result);

  ITLDRAGON_ERROR Clusterize(Circuit& circuit, vector<Cluster>& clusters, vector<vector<int>>& netList,
                             vector<int>* currTableOfConnections, list<vector<vector<int>>>& netLevels,
                             affinityFunc Affinity);

  ITLDRAGON_ERROR DisconnectClusters(int& c1, int& c2, vector<Cluster>& clusters);

  ITLDRAGON_ERROR Relaxation(Circuit& circuit, vector<Cluster>& clusters);
  
  ITLDRAGON_ERROR Interpolation(Circuit& circuit, vector<Cluster>& clusters, vector<int>* currTableOfConnections);

  ITLDRAGON_ERROR UnclusterLevelUp(vector<Cluster>& clusters, vector<int>* currTableOfConnections);

  ITLDRAGON_ERROR UpdateCoords(Circuit& circuit, vector<Cluster>& clusters);

  ITLDRAGON_ERROR MergeClusters(int& c1, int& c2, vector<Cluster>& clusters, vector<vector<int>>& netList,
                                vector<int>* currTableOfConnections, double* netAreas);
  ITLDRAGON_ERROR AffinityInterp(int& c1, int& c2, vector<Cluster>& clusters, vector<vector<int>>& netList, 
                                       vector<int>* currTableOfConnections, double* netAreas, double& result);
  ITLDRAGON_ERROR Affinity(int& c1, int& c2, vector<Cluster>& clusters, vector<vector<int>>& netList, 
                                       vector<int>* currTableOfConnections, double* netAreas, double& result);
  ITLDRAGON_ERROR AffinitySP(int& c1, int& c2, vector<Cluster>& clusters, vector<vector<int>>& netList, 
                                       vector<int>* currTableOfConnections, double* netAreas, double& result);
  ITLDRAGON_ERROR CalculateScore(int& currClusterIdx, vector<int>* currTableOfConnections, vector<vector<int>>& netList, 
                                 double* netAreas, vector<Cluster>& clusters, double& score, int& bestNeighborIdx, bool isToMark,
                                 affinityFunc Affinity);
  ITLDRAGON_ERROR InsertInHeap(list<ClusterData>& clustersDataList);
}

// binary search of elem in arr
int FindIdx( int *arr, int size, int elem );

// splitting curcuit into two parts
int SplitCircuit( bool doCut, pBin oldCirc, pBin firstCirc, pBin secondCirc );

// global placement by recursive bisection
int RecursiveBisection(Circuit& circuit, Statistics& statistics);

// adjust bins into rows structure
int AdjustmentStep(Circuit& circuit);

ITLDRAGON_ERROR CreateBinGrid(Circuit& circuit);

ITLDRAGON_ERROR CreateBinGridForGP(Circuit& circuit);

ITLDRAGON_ERROR AdjustBinsToMacros(Circuit& circuit);

ITLDRAGON_ERROR FlagBins(Circuit& circuit);

ITLDRAGON_ERROR GlobalPlacement(Circuit& circuit);

void MergeTwoBins(pBin** arrOfBins, int row1, int col1, int row2, int col2, Circuit& circuit);

void MoveCluster(pBin** arrOfBins, int toRow, int toCol, int fromRow, int fromCol, Circuit& circuit);

void ScatterBinsVert(pBin** arrOfBins, int firstCurrColIdx, int firstCurrRowIdx, int lastCurrColIdx, int lastCurrRowIdx, 
                     int rowIdxOut, int colIdxOut, int firstRowIdx, int lastRowIdx, Circuit& circuit, int dir = 1);

int AdjustBinWidths(Circuit& circuit, double* rowLengths);

int AdjustRowWidths(Circuit& circuit);

int DirectAdjusting(Circuit& circuit, double desiredRowWidth, int direction);

int AdjustWidthOfOneRow(Circuit& circuit, int rowIdx, double desiredRowWidth, int direction);

void DistribCellsAmongBins(Circuit& circuit);

#endif