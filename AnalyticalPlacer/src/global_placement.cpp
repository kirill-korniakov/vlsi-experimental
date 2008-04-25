/*  
 * global_placement.cpp
 * this is a part of itlAnalyticalPlacer
 * Copyright (C) 2005-2008, ITLab, Aleksey Bader, Kirill Kornyakov, Zhivoderov Artem
 * email: bader@newmail.ru
 * email: kirillkornyakov@yandex.ru
 * email: zhivoderov.a@gmail.com
 */

#include "..\include\global_placement.h"
#include "..\include\output.h"
#include "..\include\optimal_region_determination.h"
#include "math.h"
#include <conio.h>

#include <iostream>   // for output additional information
#include <time.h>     // for timing
#include <stdlib.h>   // for random numbers
#include <algorithm>

using namespace MultilevelFramework;

MULTIPLACER_ERROR GlobalPlacement(Circuit& circuit)
{
  double currentWL;
  vector<Cluster> clusters(circuit.nNodes);
  NetList netList(circuit.nNets);    // vector of nets of clusters (cluster indexes are stored in netList)
  vector<ConnectionsList> currTableOfConnections(clusters.size());
  list<ClusteringInfoAtEachLevel> clusteringLog;  // здесь сохраняется информация, 
                                                  // необходимая для перехода на предыдущие уровни кластеризации
  ClusteringLogIterator clusteringLogIterator;    // reverse_iterator по clusteringLog'у
  list<NetList> netLevels;  // список из нетлистов на каждом уровне кластеризации
  list<NetList>::reverse_iterator netLevelsIterator;
  int currNClusters;

  // initial data structures filling  
  InitializeDataStructures(circuit, clusters, netList, circuit.nNodes, currNClusters);
  netLevels.push_back(netList);
  
  // First V-cycle
#ifdef RECURSIVE_BISECTION
  MakeClustersFromBins(circuit, clusters, netList);
  currNClusters = circuit.nBinCols * circuit.nBinRows;
#else
  //Clusterize(circuit, clusters, netList, netLevels, Affinity, clusteringLog, currNClusters);
#endif
  CreateTableOfConnections(clusters, currTableOfConnections, netList, circuit.nNodes);
  // set initial placement
  SetInitialState(clusters, circuit, currNClusters);
  UpdateCoords(circuit, clusters);
  currentWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);
  cout << "initial state WL = " << currentWL << endl;

  int nLevels = static_cast<int>(netLevels.size());
  int nOuterIters = static_cast<int>(4 * pow(2.0, nLevels));
  int nInnerIters = static_cast<int>(128 / pow(2.0, nLevels));

  PrintToPL("before relaxation", circuit);
  Relaxation(circuit, clusters, netList, currTableOfConnections, nInnerIters, nOuterIters);
  PrintToPL("after  relaxation", circuit);
  //PrintPLT("after_relaxation.plt", circuit);
  /*Relaxation(circuit, clusters, netList, currTableOfConnections);
  PrintToPL("after  relaxation", circuit);*/

  currentWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);
  cout << "currWL after relaxation = " << currentWL << endl;

  //getch();
  //Exit();
#ifndef RECURSIVE_BISECTION
  netLevelsIterator = netLevels.rbegin();
  clusteringLogIterator = clusteringLog.rbegin();
  if (netLevelsIterator != netLevels.rend()) ++netLevelsIterator;

  for (; clusteringLogIterator != clusteringLog.rend(); ++clusteringLogIterator, ++netLevelsIterator)
  {
    UnclusterLevelUp(clusters, currTableOfConnections, (*netLevelsIterator), clusteringLogIterator, circuit.nNodes);
    Interpolation(circuit, clusters, currTableOfConnections, (*netLevelsIterator));
    //PrintToTmpPL(circuit);
    Relaxation(circuit, clusters, (*netLevelsIterator), currTableOfConnections, nInnerIters, nOuterIters);
    Trace("todo: out of relaxation",1);
    PrintToPL("after  relaxation", circuit);
    currentWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);
    cout << "currWL after relaxation = " << currentWL << endl;
  }
  
  // Second V-cycle
  //cout << "\nSecond V-cycle is beeing performed\n\n";
  //netList = netLevels.front();
  //netLevels.resize(1);
  //clusteringLog.clear();
  //Clusterize(circuit, clusters, netList, netLevels, AffinitySP, clusteringLog, currNClusters);
  //UpdateCoords(circuit, clusters);

  //nOuterIters = static_cast<int>(4 * pow(2.0, nLevels));
  //nInnerIters = static_cast<int>(128 / pow(2.0, nLevels));

  //netLevelsIterator = netLevels.rbegin();
  //clusteringLogIterator = clusteringLog.rbegin();
  //if (netLevelsIterator != netLevels.rend()) ++netLevelsIterator;

  //for (; clusteringLogIterator != clusteringLog.rend(); ++clusteringLogIterator, ++netLevelsIterator)
  //{
  //  UnclusterLevelUp(clusters, currTableOfConnections, (*netLevelsIterator), clusteringLogIterator, circuit.nNodes);
  //  Interpolation(circuit, clusters, currTableOfConnections, (*netLevelsIterator));
  //  /*PrintToTmpPL(circuit);*/
  //  Relaxation(circuit, clusters, (*netLevelsIterator), currTableOfConnections, nInnerIters, nOuterIters);
  //  PrintToPL("after  relaxation", circuit);
  //  currentWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);
  //  cout << "currWL after relaxation = " << currentWL << endl;
  //}
#endif
  return OK;
}

struct cellIdxAndY
{
  int idx;
  double yOpt;
};

// Return whether xOpt of first element is less than the second
bool predicate(cellIdxAndY elem1, cellIdxAndY elem2)
{
  return elem1.yOpt < elem2.yOpt;
}

const int    NUM_PARTS_IN_PARTITION       = 2;   // in which we divide the circuit
const int    DFLT_MAX_ACCEPTABLE_BIN_SIZE = 6;   // for stopping criteria
const double DFLT_ROWS_NUM_DEVIATION      = 0.5; // for choosing number of rows in partition

const int    DFLT_TIME_START_TP      = 4;
const double MIN_EXTERNAL_DEVIATION  = 1.1; // for terminal propagation
const int    DFLT_NUM_FEEDBACK_ITERS = 0;

const int NUM_DUMMY_CELLS = 2;
int DUMMY_CELL1;
int DUMMY_CELL2;

const int CLUSTER_RATIO = 5; // decrease of the number of clusters after each iteration
double totalCellArea = 0.0;

// binary search of elem in arr
int FindIdx(int *arr, int size, int elem)
{
  int lidx = 0, ridx = size;
  int i;
  while(true)
  {
    i = (ridx + lidx)/2;
    if (arr[i] < elem)
    {
      lidx = i;
    }
    else
      if (arr[i] > elem)
      {
        ridx = i;
      }
      else
      {
        return i;
      }
  }
}

int RecursiveBisection(Circuit& circuit, Statistics& statistics)
{
  int    maxAcceptableBinSize  = DFLT_MAX_ACCEPTABLE_BIN_SIZE;
  double rowsNumDeviation      = DFLT_ROWS_NUM_DEVIATION;
  bool   isTerminalPropagation = true;
  int    timeStartTP           = DFLT_TIME_START_TP;
  int    numFeedbackIters      = DFLT_NUM_FEEDBACK_ITERS;
  double minExternalDeviation  = MIN_EXTERNAL_DEVIATION;

  int errorCode = OK;

  if (gOptions.innerParameters[0][0] != -1) maxAcceptableBinSize = static_cast<int>(gOptions.innerParameters[0][0]);
  if (gOptions.innerParameters[0][1] != -1) rowsNumDeviation     = gOptions.innerParameters[0][1];
  if (gOptions.innerParameters[0][2] != -1) isTerminalPropagation = false;
  if (gOptions.innerParameters[0][3] != -1) timeStartTP = static_cast<int>(gOptions.innerParameters[0][3]);
  if (gOptions.innerParameters[0][4] != -1) numFeedbackIters = static_cast<int>(gOptions.innerParameters[0][4]);
  if (gOptions.innerParameters[0][5] != -1) minExternalDeviation = gOptions.innerParameters[0][5];

  int i, j;

  clock_t start, finish;  
  int numOfBins = 1;
  circuit.nBinRows = 1;
  circuit.nBinCols = 1;

  // maximal and minimal number of cells in each bin of current partition
  int    maxBinSize = 0;
  int    minBinSize = circuit.nNodes;
  int    tmpBinSize = 0;

  // start timer
  start = clock();

  // vertical and horizontal distance between centers of adjacent bins
  circuit.binsHeight = circuit.height;
  circuit.binsWidth  = circuit.width;

  // place all cells into the center of the circuit
  for(int i = 0; i < circuit.nNodes; ++i)
  {
    circuit.placement[i].xCoord = circuit.width/2;
    circuit.placement[i].yCoord = circuit.height/2;
  }

  // create first circuit
  pBin **bins;
  pBin **copyBins = NULL;
  bins     = new pBin*[circuit.nBinRows];
  bins[0]  = new pBin [circuit.nBinCols];
  bins[0][0] = new Bin;

  // convert to structures for hMetis
  // initialize hyper edges in the first circuit
  int currIdx = 0;
  bins[0][0]->indexes.push_back(currIdx);
  for(i = 0; i < circuit.nNets; ++i)
  {
    for(j = 0; j < circuit.nets[i].numOfPins; ++j)
    {
      if (circuit.nets[i].arrPins[j].cellIdx < circuit.nNodes)
      {
        bins[0][0]->hedges.push_back(circuit.nets[i].arrPins[j].cellIdx);
        currIdx++;
      }
    }
    bins[0][0]->indexes.push_back(currIdx);
    bins[0][0]->nets.push_back(i);
  }

  // in the first bin nodes are indexed from 0 to circuit.nNodes - 1
  for(i = 0; i < circuit.nNodes; ++i)
  {
    bins[0][0]->nodes.push_back(i);
    bins[0][0]->cwts.push_back(circuit.nodes[i].width);
  }

  bins[0][0]->xCoord = circuit.width  / 2;
  bins[0][0]->yCoord = circuit.height / 2;

  // create circuits for next iterations
  pBin **new_bins;

  double oldWL;
  double currOffset = 0;

  int counter = 0;

  int FinalNumOfRows = 1;
  for (i=0;;i++)
  {
    FinalNumOfRows *= 2;
    if (circuit.nRows <= FinalNumOfRows*(1 + rowsNumDeviation)) break;
  }
  double cellsPerRow = circuit.nNodes / circuit.nRows;
  int FinalNumOfCols = 1;
  for (i=0;;i++)
  {
    FinalNumOfCols *= 2;
    if ( cellsPerRow/FinalNumOfCols <= maxAcceptableBinSize) break;
  }

  bool doBinSwapping           = false;
  bool isHorizontalSplitting   = false;
  bool onlyHorizontalSplitting = false;
  bool onlyVerticalSplitting   = false;

  DUMMY_CELL1 = circuit.nNodes;
  DUMMY_CELL2 = DUMMY_CELL1 + 1;

  // starting recursive bisection
  while(true)        
  {
    cout << "\nrecursive bisection, iteration #" << counter + 1 << "\n";

    PrintToTmpPL(circuit, statistics, 0, 0);

    while(true)
    {
      // choose side for partition
      if (onlyVerticalSplitting) 
      {
        isHorizontalSplitting = false;
        break;
      }
      if (onlyHorizontalSplitting)
      {
        isHorizontalSplitting = true;
        break;
      }
      isHorizontalSplitting = !isHorizontalSplitting;
      break;
    }

    if (isHorizontalSplitting)
    {
      FinalNumOfRows /= 2;
      if (!onlyHorizontalSplitting && (FinalNumOfRows==1) )
        onlyVerticalSplitting = true;

      circuit.binsHeight /= 2.0;
      currOffset = circuit.binsHeight/2.0;
      new_bins = new pBin*[circuit.nBinRows*2];
      for (i = 0; i < circuit.nBinRows*2; i++ )
      {
        new_bins[i] = new pBin[circuit.nBinCols];
        for (j = 0; j < circuit.nBinCols; ++j )
          new_bins[i][j] = new Bin;
      }
    }
    else
    {
      FinalNumOfCols /= 2;
      if (!onlyVerticalSplitting && (FinalNumOfCols==1) ) 
        onlyHorizontalSplitting = true;

      circuit.binsWidth /= 2.0;
      currOffset = circuit.binsWidth/2.0;      
      new_bins = new pBin*[circuit.nBinRows];
      for (i = 0; i < circuit.nBinRows; i++ )
      {
        new_bins[i] = new pBin[circuit.nBinCols*2];
        for (j = 0; j < circuit.nBinCols*2; ++j )
          new_bins[i][j] = new Bin;
      }
    }

    // add dummy cells
    for (i = 0; i < circuit.nBinRows; ++i)
    {
      for (j = 0; j < circuit.nBinCols; ++j)
      {
        bins[i][j]->nodes.push_back(DUMMY_CELL1);
        bins[i][j]->cwts.push_back(0);
        bins[i][j]->nodes.push_back(DUMMY_CELL2);
        bins[i][j]->cwts.push_back(0);
      }
    }

    int numIters;
    bool doCut;
    double** coords = NULL;
    coords = new double*[circuit.nBinRows];
    for (i = 0; i < circuit.nBinRows; ++i)
      coords[i] = new double[circuit.nBinCols];

    for (i = 0; i < circuit.nBinRows; ++i)
      for (j = 0; j < circuit.nBinCols; ++j)
      {
        if (isHorizontalSplitting)
          coords[i][j] = circuit.placement[bins[i][j]->nodes[0]].yCoord;
        else
          coords[i][j] = circuit.placement[bins[i][j]->nodes[0]].xCoord;
      }

      if  (isTerminalPropagation && (numOfBins >= timeStartTP))
      { 
        numIters = numFeedbackIters;
        doCut = false;
        copyBins = new pBin*[circuit.nBinRows];
        for (i = 0; i < circuit.nBinRows; i++ )
        {
          copyBins[i] = new pBin[circuit.nBinCols];
          for (j = 0; j < circuit.nBinCols; ++j )
            copyBins[i][j] = new Bin;
        }
        coords = new double*[circuit.nBinRows];
        for (i = 0; i < circuit.nBinRows; ++i)
          coords[i] = new double[circuit.nBinCols];

        for (i = 0; i < circuit.nBinRows; ++i)
        {
          for (j = 0; j < circuit.nBinCols; ++j)
          {
            if (isHorizontalSplitting)
              coords[i][j] = circuit.placement[bins[i][j]->nodes[0]].yCoord;
            else
              coords[i][j] = circuit.placement[bins[i][j]->nodes[0]].xCoord;
            copyBins[i][j]->hedges.resize( bins[i][j]->hedges.size() );
            copyBins[i][j]->indexes.resize( bins[i][j]->indexes.size() );
            copy( bins[i][j]->hedges.begin(), bins[i][j]->hedges.end(), copyBins[i][j]->hedges.begin() );
            copy( bins[i][j]->indexes.begin(), bins[i][j]->indexes.end(), copyBins[i][j]->indexes.begin() );
          }
        }
      }
      else
        numIters = 0;

      // feedback cycle
      for (int k = 0; k <= numIters; ++k)
      {
        if (k == numIters) doCut = true;

        // add links to dummy cells according to terminal propagation technique
        if (isTerminalPropagation && (numOfBins >= timeStartTP))
        {
          // we have to rewrite hedges and indexes
          if (k > 0)
          {
            for (i = 0; i < circuit.nBinRows; ++i)
            {
              for (j = 0; j < circuit.nBinCols; ++j)
              {
                if (bins[i][j]->indexes.size() == 0) continue;  //this bin is empty, only dummy cells

                bins[i][j]->hedges.clear();
                bins[i][j]->indexes.clear();

                bins[i][j]->hedges.resize( copyBins[i][j]->hedges.size() );
                bins[i][j]->indexes.resize( copyBins[i][j]->indexes.size() );

                copy( copyBins[i][j]->hedges.begin(), copyBins[i][j]->hedges.end(), bins[i][j]->hedges.begin() );
                copy( copyBins[i][j]->indexes.begin(), copyBins[i][j]->indexes.end(), bins[i][j]->indexes.begin() );
              }
            }
          }
          cout << "partition with terminal propagation\n";
          TerminalPropagation(circuit, bins, isHorizontalSplitting);
        }

        maxBinSize = 0;
        minBinSize = circuit.nNodes;
        // make partition
        for (i = 0; i < circuit.nBinRows; ++i)
        {
          for (j = 0; j < circuit.nBinCols; ++j)
          { 
            if (bins[i][j]->indexes.size() == 0) continue;  //this bin is empty, only dummy cells

            if (isHorizontalSplitting)
            {
              // make hypergraph partition
              SplitCircuit(doCut, bins[i][j], new_bins[2*i][j], new_bins[2*i+1][j]);

              // write new coordinates
              tmpBinSize = static_cast<int>(new_bins[2*i][j]->nodes.size());
              if (maxBinSize < tmpBinSize) maxBinSize = tmpBinSize;
              if (minBinSize > tmpBinSize) minBinSize = tmpBinSize;
              for (int k = 0; k < tmpBinSize; ++k)
              {
                circuit.placement[new_bins[2*i][j]->nodes[k]].yCoord = coords[i][j] - currOffset;
              }
              new_bins[2*i][j]->xCoord = bins[i][j]->xCoord;
              //new_bins[2*i][j]->xCoord = circuit.placement[new_bins[2*i][j]->nodes[0]].xCoord;
              new_bins[2*i][j]->yCoord = coords[i][j] - currOffset;

              tmpBinSize = static_cast<int>(new_bins[2*i+1][j]->nodes.size());
              if (maxBinSize < tmpBinSize) maxBinSize = tmpBinSize;
              if (minBinSize > tmpBinSize) minBinSize = tmpBinSize;
              for (int k = 0; k < tmpBinSize; ++k)
              {
                circuit.placement[new_bins[2*i+1][j]->nodes[k]].yCoord = coords[i][j] + currOffset;
              }
              new_bins[2*i+1][j]->xCoord = bins[i][j]->xCoord;
              //new_bins[2*i+1][j]->xCoord = circuit.placement[new_bins[2*i+1][j]->nodes[0]].xCoord;
              new_bins[2*i+1][j]->yCoord = coords[i][j] + currOffset;
            }
            else
            {
              // make hypergraph partition
              SplitCircuit(doCut, bins[i][j], new_bins[i][2*j], new_bins[i][2*j+1]);
              // write new coordinates
              tmpBinSize = static_cast<int>( new_bins[i][2*j]->nodes.size());
              if (maxBinSize < tmpBinSize) maxBinSize = tmpBinSize;
              if (minBinSize > tmpBinSize) minBinSize = tmpBinSize;
              for (int k = 0; k < tmpBinSize; ++k)
              {
                circuit.placement[new_bins[i][2*j]->nodes[k]].xCoord = coords[i][j] - currOffset;
              }
              new_bins[i][2*j]->xCoord = coords[i][j] - currOffset;
              //new_bins[i][2*j]->yCoord = circuit.placement[new_bins[i][2*j]->nodes[0]].yCoord;
              new_bins[i][2*j]->yCoord = bins[i][j]->yCoord;

              tmpBinSize = static_cast<int>(new_bins[i][2*j+1]->nodes.size());
              if (maxBinSize < tmpBinSize) maxBinSize = tmpBinSize;
              if (minBinSize > tmpBinSize) minBinSize = tmpBinSize;
              for (int k = 0; k < tmpBinSize; ++k)
              {
                circuit.placement[new_bins[i][2*j+1]->nodes[k]].xCoord = coords[i][j] + currOffset;
              }
              new_bins[i][2*j+1]->xCoord = coords[i][j] + currOffset;
              //new_bins[i][2*j+1]->yCoord = circuit.placement[new_bins[i][2*j+1]->nodes[0]].yCoord;
              new_bins[i][2*j+1]->yCoord = bins[i][j]->yCoord;
            }    
          }  // for (j = 0; j < circuit.nBinCols; ++j)
        } // for(i = 0; i < circuit.nBinRows; ++i)

        cout << "Number of cells in bins: " << minBinSize << " - " << maxBinSize << "\n";
        oldWL = cf_recalc_all(1, circuit.nNets, circuit.nets, circuit.placement);
        cout << "WL: " << oldWL << "\n";
      } // for (int k=0; k <= numFeedbackIters; ++k) feedback cycle

      // delete old bins
      for(i = 0; i < circuit.nBinRows; ++i)
        for(j = 0; j < circuit.nBinCols; ++j)
          delete bins[i][j];
      for (int k = 0; k < circuit.nBinRows; ++k )
        delete[] bins[k];
      delete[] bins;

      // delete old copyBins
      if  (copyBins != NULL)
      {
        for(i = 0; i < circuit.nBinRows; ++i)
          for(j = 0; j < circuit.nBinCols; ++j)
            delete copyBins[i][j];
        for (int k = 0; k < circuit.nBinRows; ++k )
          delete[] copyBins[k];
        delete[] copyBins;
        copyBins = NULL;
      }
      if (coords != NULL)
      {
        for (i = 0; i < circuit.nBinRows; i++ )
          delete[] coords[i];
        delete coords;
      }

      // new circiut becomes current
      circuit.arrOfBins = bins = new_bins;  
      new_bins = NULL;

      // the bin quantity increases twice
      numOfBins *= 2;   
      if (isHorizontalSplitting) 
        circuit.nBinRows *= 2;
      else
        circuit.nBinCols *= 2;

      cout << "Number of bins:\t" << numOfBins << " = " 
        << circuit.nBinRows << " x " << circuit.nBinCols << "\n";

      // decide whether do bin-swapping (after each quadrisection and every final bisection)
      if (circuit.nBinRows == circuit.nBinCols) 
        doBinSwapping = true;

      //PrintBinsCoordinates(circuit);
      errorCode = CheckXEquality(circuit);
      if (errorCode != OK) 
        return errorCode;
      errorCode = CheckYEquality(circuit);
      if (errorCode != OK) 
        return errorCode;
      errorCode = CheckOrder(circuit);
      if (errorCode != OK)
        return errorCode;

      ++counter;

      if (counter >= 9)//((!FinalNumOfCols) && (!FinalNumOfRows) || (minBinSize < 2))
        break;

  } // while(maxBinSize > MAX_CLUSTER_SIZE)

  finish = clock();       // stop timer
  cout << "Number of clusters is " << circuit.nBinCols * circuit.nBinRows << endl;
  cout << "Duration of global placement phase: " 
    << (double)(finish - start) / CLOCKS_PER_SEC  << "\n";

  errorCode = CheckBins(circuit);

  return errorCode;
}


// functions adds connection to the dummy cell, so
// this is implementation of terminal propagation technique
int TerminalPropagation(Circuit& circuit, pBin **bins, bool isHorizontal)
{
  int i, j, k, p, q;
  int dummyCell1Idx, dummyCell2Idx;
  int netIdx;
  double c, c1, cMin, c2, cMax;
  vector<int> hedges;
  int lastButOneIdx, lastIdx;

  // we do it for every bin
  for (i = 0; i < circuit.nBinRows; ++i)
  {
    for (j = 0; j < circuit.nBinCols; ++j)
    { 
      // determine indexes of dummy cells in this bin
      dummyCell1Idx = static_cast<int>(bins[i][j]->nodes.size()) - 2;
      dummyCell2Idx = dummyCell1Idx + 1;

      // we copy hedges to new vector and clear bins' hedges vector
      // to fill it with new hedges, probably supplemented with dummy node
      hedges.clear();      
      hedges.resize( bins[i][j]->hedges.size() );      
      copy( bins[i][j]->hedges.begin(), bins[i][j]->hedges.end(), hedges.begin() );      
      bins[i][j]->hedges.clear();      

      // process each net (add connection to the dummy cell if needed)
      lastButOneIdx = 0;
      lastIdx       = 0;
      for (k = 0; k < static_cast<int>(bins[i][j]->nets.size()); ++k )
      {
        netIdx = bins[i][j]->nets[k];
        lastButOneIdx = lastIdx;
        lastIdx = bins[i][j]->indexes[k+1];

        // calculate c1, c2, cMax, cMin (see paper about THETO)
        if (isHorizontal)
        {
          c = circuit.placement[bins[i][j]->nodes[0]].yCoord;
          c1 = c - 0.5 * circuit.binsHeight; // circuit.binsHeight was decreased
          c2 = c + 0.5 * circuit.binsHeight;
          cMin = c1;
          cMax = c2;
          for (p = 0; p < circuit.nets[netIdx].numOfPins; p++)
          {
            c = circuit.placement[circuit.nets[netIdx].arrPins[p].cellIdx].yCoord + 
              circuit.nets[netIdx].arrPins[p].yOffset;
            if (c > cMax) cMax = c;
            if (c < cMin) cMin = c;
          }
        } 
        else
        {
          c = circuit.placement[bins[i][j]->nodes[0]].xCoord;
          c1 = c - 0.5 * circuit.binsWidth; // circuit.binsWidth was decreased, so c1 and c2 
          c2 = c + 0.5 * circuit.binsWidth; // is coordinates of new bins
          cMin = c1;
          cMax = c2;
          for (p = 0; p < circuit.nets[netIdx].numOfPins; p++)
          {
            c = circuit.placement[circuit.nets[netIdx].arrPins[p].cellIdx].xCoord + 
              circuit.nets[netIdx].arrPins[p].xOffset;
            if (c > cMax) cMax = c;
            if (c < cMin) cMin = c;
          }
        }

        // rewrite hedge
        for (q = lastButOneIdx; q < lastIdx; q++ )
        {
          bins[i][j]->hedges.push_back(hedges[q]);
        }
        bins[i][j]->indexes[k+1] = static_cast<int>(bins[i][j]->hedges.size());

        // and add dummy cell if needed (criterion is from THETO paper)
        if ((c1 - cMin) > MIN_EXTERNAL_DEVIATION*(cMax - c2))
        {
          bins[i][j]->hedges.push_back(dummyCell1Idx);
          bins[i][j]->indexes[k+1]++;
        }
        else if ((c1 - cMin)*MIN_EXTERNAL_DEVIATION < (cMax - c2))
        {
          bins[i][j]->hedges.push_back(dummyCell2Idx);
          bins[i][j]->indexes[k+1]++;
        }
      } // for each net in bin
    } // for each bin in row
  } // for each bin row

  return OK;
}

int SplitCircuit(bool doCut, pBin oldCirc, pBin firstCirc, pBin secondCirc)
{
  if (oldCirc->nodes.size() == 0)
  {
    return 1;
  }

  static int options[9]; // options for hmetis
  options[0] = 1;
  options[1] = 10;  // NRuns
  options[2] = 2;   // CType
  options[3] = 1;   // RType
  options[4] = 3;   // V-cycle
  options[5] = 0;   // reconstruct
  options[6] = 1;   // pre-assigned
  options[7] = 23539;   // seed for generator, negative if random need
  //options[7] = -1;   // seed for generator, negative if random need
  options[8] = 0;   // print info
  int *part  = new int[oldCirc->nodes.size()];
  int i;
  for (i = 0; i < static_cast<int>(oldCirc->nodes.size()) - NUM_DUMMY_CELLS; ++i)
    part[i] = -1;
  part[i++] = 0;
  part[i]   = 1;
  static int edgecut;        // number of broken edges

  // make partitioning (before add dummy net if no one exist)
  if (static_cast<int>(oldCirc->indexes.size()) == 1)
  {
    oldCirc->hedges.push_back(0);
    oldCirc->indexes.push_back(1);
  }
  HMETIS_PartRecursive( static_cast<int>(oldCirc->nodes.size()),
    static_cast<int>(oldCirc->indexes.size()) - 1,
    &(oldCirc->cwts).front(), 
    &(oldCirc->indexes).front(), 
    &(oldCirc->hedges).front(),
    NULL, 2, 1, &options[0], part, &edgecut );
  /*for (i = 0; i < static_cast<int>(oldCirc->nodes.size()); ++i)
  cout << part[i] << endl;*/

  firstCirc->nodes.clear();
  secondCirc->nodes.clear();  
  // fill bins with nodes
  for (i = 0; i < static_cast<int>(oldCirc->nodes.size()) - NUM_DUMMY_CELLS; ++i)
  {
    if (part[i] == 0)
    {
      firstCirc->nodes.push_back(oldCirc->nodes[i]);
      if (doCut) 
        firstCirc->cwts.push_back(oldCirc->cwts[i]);
    }
    else
    {
      secondCirc->nodes.push_back(oldCirc->nodes[i]);
      if (doCut) 
        secondCirc->cwts.push_back(oldCirc->cwts[i]);
    }
  }

  if (doCut)
  {
    // computes number of circuit.nets in each part
    firstCirc->indexes.push_back(0);
    firstCirc->indexes.push_back(0);
    secondCirc->indexes.push_back(0);
    secondCirc->indexes.push_back(0);
    bool isPushed1 = false;
    bool isPushed2 = false;
    for (i = 0; i < static_cast<int>(oldCirc->indexes.size()) - 1; ++i)
    {
      isPushed1 = false;
      isPushed2 = false;
      for (int k = oldCirc->indexes[i]; k < oldCirc->indexes[i+1]; ++k)
      {
        // we do not add dummy cell into circuit.nets
        if ((oldCirc->nodes[oldCirc->hedges[k]] == DUMMY_CELL1)||
          (oldCirc->nodes[oldCirc->hedges[k]] == DUMMY_CELL2)) continue;

        if (part[oldCirc->hedges[k]] == 0)
        {
          firstCirc->hedges.push_back(FindIdx(&firstCirc->nodes.front(), 
            static_cast<int>( firstCirc->nodes.size()), oldCirc->nodes[oldCirc->hedges[k]]));
          firstCirc->indexes[firstCirc->indexes.size() - 1]++;
          if(!isPushed1)
          {
            firstCirc->nets.push_back(oldCirc->nets[i]);
            isPushed1 = true;
          }
        }
        else if (part[oldCirc->hedges[k]] == 1)
        {
          secondCirc->hedges.push_back(FindIdx(&secondCirc->nodes.front(), 
            static_cast<int>(secondCirc->nodes.size()), oldCirc->nodes[oldCirc->hedges[k]]));
          secondCirc->indexes[secondCirc->indexes.size() - 1]++;
          if(!isPushed2)
          {
            secondCirc->nets.push_back(oldCirc->nets[i]);
            isPushed2 = true;
          }
        }
      } // for (int k = oldCirc->indexes[i]; k < oldCirc->indexes[i+1]; ++k)

      // add new pointer to the array of the indexes
      if (isPushed1)
        firstCirc->indexes.push_back(firstCirc->indexes[firstCirc->indexes.size()-1]);    
      if (isPushed2)
        secondCirc->indexes.push_back(secondCirc->indexes[secondCirc->indexes.size()-1]);

    } // for (j = 0; j < oldCirc.nEdges; ++j)

    secondCirc->indexes.pop_back();
    firstCirc->indexes.pop_back();
  }

  delete[] part;

  return OK;
}
