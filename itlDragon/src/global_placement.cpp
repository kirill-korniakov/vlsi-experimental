/*  
 * global_circuit.placement.cpp
 * this is a part of itlDragon
 * Copyright (C) 2005, ITLab, Aleksey Bader, Kirill Kornyakov
 * email: bader@newmail.ru
 * email: kirillkornyakov@yandex.ru
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

using namespace std;

struct cellIdxAndY
{
  int idx;
  double yOpt;
};

// Return whether xOpt of first element is less than the second
bool predicate( cellIdxAndY elem1, cellIdxAndY elem2 )
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

// splitting circuit into two parts
int SplitCircuit( bool doCut, pBin oldCirc, pBin firstCirc, pBin secondCirc )
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
  //options[7] = 23539;   // seed for generator, negative if random need
  options[7] = -1;   // seed for generator, negative if random need
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

/// Global Placement
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
  double currWL;
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
  cout << "Desired final bin grid: " << FinalNumOfRows
       << " x " << FinalNumOfCols << "\n";

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
    //doBinSwapping = false;
    if (gOptions.doBinSwapping && doBinSwapping)
    {
      doBinSwapping = false;

      statistics.currentWL = oldWL = cf_recalc_all(1, circuit.nNets, circuit.nets, circuit.placement);
      //cout << "WL before bin-swapping: " << oldWL << "\n";
      currWL = BinSwapping(circuit, statistics, gOptions.innerParameters[1]);
      //cout << "WL after  bin-swapping: "  << currWL << "\n";
      cout << "Reduction: " << (1 - currWL/oldWL)*100 << "%\n";
    }

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
    
    if ((!FinalNumOfCols) && (!FinalNumOfRows) || (minBinSize < 2))
      break;    

    ++counter;
  } // while(maxBinSize > MAX_CLUSTER_SIZE)
  
  finish = clock();       // stop timer
  cout << "Duration of global placement phase: " 
     << (double)(finish - start) / CLOCKS_PER_SEC  << "\n";

  errorCode = CheckBins(circuit);
  
  return errorCode;
}

ITLDRAGON_ERROR CreateBinGrid(Circuit& circuit)
{
  int errorCode = OK;

  // if we input global placement, we have to create bin grid
  vector<double> diffXCoords;
  for (int i = 0; i < circuit.nNodes; ++i)
  {
    if (find(diffXCoords.begin(), diffXCoords.end(), circuit.placement[i].xCoord) == diffXCoords.end())
      diffXCoords.push_back(circuit.placement[i].xCoord);
  }
  circuit.nBinRows  = circuit.nRows;
  circuit.nBinCols  = static_cast<int>(diffXCoords.size());
  circuit.arrOfBins = new pBin*[circuit.nBinRows];
  for (int i = 0; i < circuit.nBinRows; ++i)
  {
    circuit.arrOfBins[i] = new pBin[circuit.nBinCols];
    for(int j = 0; j < circuit.nBinCols; ++j)
      circuit.arrOfBins[i][j] = new Bin;
  }
  circuit.binsHeight = circuit.rows[0].height;
  sort( diffXCoords.begin(), diffXCoords.end() );
  circuit.binsWidth  = diffXCoords[1] - diffXCoords[0];
  int row, col;
  for (int i = 0; i < circuit.nNodes; ++i)
  {
    row = static_cast<int>(circuit.placement[i].yCoord / circuit.binsHeight);
    col = static_cast<int>(circuit.placement[i].xCoord / circuit.binsWidth);
    circuit.arrOfBins[row][col]->nodes.push_back(i);
    circuit.arrOfBins[row][col]->sumLength += circuit.nodes[i].width;
  }
  
  double binHeight = circuit.rows[0].height;
  //double firstX = -1.0;
  int counter = 0;
  int cellIdx;
  /*for (int i = 0; i < circuit.nBinRows; ++i)
    if (circuit.arrOfBins[i][0]->nodes.size() != 0)
    {
      firstX = circuit.placement[circuit.arrOfBins[i][0]->nodes[0]].xCoord;
      break;
    }
  if (firstX == -1.0) firstX = circuit.binsWidth * 0.5;*/
  for (int i = 0; i < circuit.nBinRows; ++i)
    for(int j = 0; j < circuit.nBinCols; ++j)
    {
      counter = i;
      if (circuit.arrOfBins[counter][j]->nodes.size() == 0)
      { 
        counter = 0;
        while (circuit.arrOfBins[counter][j]->nodes.size() == 0)
          counter++;
      }
      //circuit.arrOfBins[i][counter]->xCoord = firstX + circuit.binsWidth * j;
      cellIdx = circuit.arrOfBins[counter][j]->nodes[0];
      circuit.arrOfBins[i][j]->xCoord = circuit.placement[cellIdx].xCoord;
      circuit.arrOfBins[i][j]->yCoord = (i + 0.5) * binHeight;
    }

  // check legality of Bins
  errorCode = CheckXEquality(circuit);
  if (errorCode != OK) return errorCode;
  errorCode = CheckYEquality(circuit);
  if (errorCode != OK) return errorCode;
  /*errorCode = CheckBins(circuit);
  if (errorCode != OK) return errorCode;*/
  
  errorCode = FlagBins(circuit);
  if (errorCode != OK) return errorCode;
  
  return OK;
}

ITLDRAGON_ERROR CreateBinGridForGP(Circuit& circuit)
{
  int errorCode = OK;

  // if we input global placement, we have to create bin grid
  vector<double> diffXCoords;
  vector<double> diffYCoords;
  for (int i = 0; i < circuit.nNodes; ++i)
  {
    // make coordinates integer for future comparison
    circuit.placement[i].xCoord = dtoi(circuit.placement[i].xCoord);
    circuit.placement[i].yCoord = dtoi(circuit.placement[i].yCoord);
    
    if (find( diffXCoords.begin(), diffXCoords.end(), circuit.placement[i].xCoord ) == diffXCoords.end())
      diffXCoords.push_back( circuit.placement[i].xCoord );
    if (find( diffYCoords.begin(), diffYCoords.end(), circuit.placement[i].yCoord ) == diffYCoords.end())
      diffYCoords.push_back( circuit.placement[i].yCoord );
  }
  circuit.nBinRows = static_cast<int>(diffYCoords.size());
  circuit.nBinCols = static_cast<int>(diffXCoords.size());
  circuit.arrOfBins = new pBin*[circuit.nBinRows];
  for (int i = 0; i < circuit.nBinRows; i++ )
  {
    circuit.arrOfBins[i] = new pBin[circuit.nBinCols];
    for(int j = 0; j < circuit.nBinCols; ++j )
      circuit.arrOfBins[i][j] = new Bin;
  }
  sort(diffXCoords.begin(), diffXCoords.end());
  sort(diffYCoords.begin(), diffYCoords.end());
  circuit.binsWidth  = diffXCoords[1] - diffXCoords[0];
  circuit.binsHeight = 10.429688;
  int row, col;
  for (int i=0; i < circuit.nNodes; ++i)
  {
    row = static_cast<int>(circuit.placement[i].yCoord / circuit.binsHeight);
    col = static_cast<int>(circuit.placement[i].xCoord / circuit.binsWidth);
    circuit.arrOfBins[row][col]->nodes.push_back(i);
    circuit.arrOfBins[row][col]->sumLength += circuit.nodes[i].width;
  }

  // check legality of Bins
  errorCode = CheckBins(circuit);
  if (errorCode != OK) return errorCode;

  return OK;
}

// algorithm to adjust bins into row structure
int AdjustmentStep(Circuit& circuit)
{
  int errorCode = OK;

  // check legality of Bins
  errorCode = CheckBins(circuit);
  if (errorCode != OK) return errorCode;
 
  // create new array of bins circuit.nBinRows*circuit.nBinCols
  pBin** new_bins;
  new_bins = new pBin*[circuit.nRows];
  for (int i = 0; i < circuit.nRows; ++i) 
  {
    new_bins[i] = new pBin[circuit.nBinCols];
    for (int j = 0; j < circuit.nBinCols; ++j)
      new_bins[i][j] = new Bin;
  }
  
  // Algorithm:
  //   merge all the cells within the same column in the bin grids, 
  //   and then evenly divide them into rows
  double cellWidth = 0.0;   // 
  double totalWidth = 0.0;  // total width of cells in column
  int numCellsInColumn = 0;   // number of cells in 'cells'
  double* coordsOfCenters;
  double rowHeight = circuit.rows[0].height;
  int rowIdx = 0;

  vector<cellIdxAndY> sortedCells;
  cellIdxAndY         newIdxAndY;
     
  for (int colIdx = 0; colIdx < circuit.nBinCols; ++colIdx)
  {
    sortedCells.clear();

    // merge all cells from one column 
    for (rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx )
    {
      for (int i = 0; i < static_cast<int>(circuit.arrOfBins[rowIdx][colIdx]->nodes.size()); ++i)
      {
        newIdxAndY.idx = circuit.arrOfBins[rowIdx][colIdx]->nodes[i];
        newIdxAndY.yOpt = GetOptimalY( &newIdxAndY.idx, 1, circuit.placement,
                                       circuit.nNets, circuit.nets, circuit.tableOfConnections,
                                       NULL, -1 );
        sortedCells.push_back(newIdxAndY);
      }
    }
    sort( sortedCells.begin(), sortedCells.end(), predicate );
    
    totalWidth = 0.0;
    numCellsInColumn = static_cast<int>(sortedCells.size());
    coordsOfCenters = new double[numCellsInColumn];

    for (int i = 0; i < numCellsInColumn; ++i)
    {
      cellWidth = circuit.nodes[sortedCells[i].idx].width;
      coordsOfCenters[i] = totalWidth + cellWidth * 0.5;
      totalWidth += cellWidth;
    }

    int nextForFillingRowIdx = 0; // row which is next after last filled
    for (int i = 0; i < numCellsInColumn; ++i)
    {
      rowIdx = static_cast<int>((coordsOfCenters[i] / totalWidth) * circuit.nRows);
      
      // we do not create empty bins
      if (nextForFillingRowIdx < rowIdx) rowIdx = nextForFillingRowIdx;

      new_bins[rowIdx][colIdx]->nodes.push_back(sortedCells[i].idx);
      new_bins[rowIdx][colIdx]->sumLength += circuit.nodes[sortedCells[i].idx].width;
      circuit.placement[sortedCells[i].idx].yCoord = rowHeight * (rowIdx + 0.5);
      new_bins[rowIdx][colIdx]->xCoord = circuit.arrOfBins[0][colIdx]->xCoord;
      new_bins[rowIdx][colIdx]->yCoord = rowHeight * (rowIdx + 0.5);

      nextForFillingRowIdx = rowIdx + 1;
    }
   
    delete[] coordsOfCenters;
  } // for each column merge cells and divide them between rows

  // delete old bins
  for (int i = 0; i < circuit.nBinRows; ++i) 
    for (int j = 0; j < circuit.nBinCols; ++j)
      delete circuit.arrOfBins[i][j];
  for (int k = 0; k < circuit.nBinRows; ++k )
    delete[] circuit.arrOfBins[k];
  delete[] circuit.arrOfBins;

  circuit.arrOfBins   = new_bins;
  circuit.nBinRows    = circuit.nRows;
  circuit.binsHeight  = rowHeight;

  // chek bins legality
  errorCode = CheckBins(circuit);

  return errorCode;
}

ITLDRAGON_ERROR FlagBins(Circuit& circuit)
{
  int firstColIdx;
  int firstRowIdx;
  int lastColIdx;
  int lastRowIdx;
  //int width;
  //int height;
  int macroIdx;
  float leftBorder;
  float rightBorder;
  float lowerBorder;
  float upperBorder;
  //int cellIdx;
  int i;
  
  /*int colIdx;
  int rowIdx;*/
  
  for (i = 0; i < circuit.nTerminals; ++i)
  {
    macroIdx = i + circuit.nNodes;
    
    leftBorder  = static_cast<float>(circuit.placement[macroIdx].xCoord - 0.5 * circuit.terminals[i].width);
    rightBorder = static_cast<float>(circuit.placement[macroIdx].xCoord + 0.5 * circuit.terminals[i].width);
    lowerBorder = static_cast<float>(circuit.placement[macroIdx].yCoord - 0.5 * circuit.terminals[i].height);
    upperBorder = static_cast<float>(circuit.placement[macroIdx].yCoord + 0.5 * circuit.terminals[i].height);
    
    if ((rightBorder <= 0.0) || (leftBorder  >= circuit.width) ||
        (upperBorder <= 0.0) || (lowerBorder >= circuit.height))
      continue;
    
    firstColIdx = static_cast<int>(leftBorder  / circuit.binsWidth + 0.49);
    firstRowIdx = static_cast<int>(lowerBorder / circuit.binsHeight);
    lastColIdx  = static_cast<int>(rightBorder / circuit.binsWidth + 0.49 - 1.0);
    lastRowIdx  = static_cast<int>(upperBorder / circuit.binsHeight - 1.0);
    
    firstColIdx = max(0, firstColIdx);
    firstRowIdx = max(0, firstRowIdx);
    lastColIdx  = min(circuit.nBinCols - 1, lastColIdx);
    lastRowIdx  = min(circuit.nBinRows - 1, lastRowIdx);
    
    //if (circuit.arrOfBins[firstRowIdx][firstColIdx]->nodes.size() != 0)
    {
      //cellIdx = circuit.arrOfBins[firstRowIdx][firstColIdx]->nodes[0];
      if (circuit.arrOfBins[firstRowIdx][firstColIdx]->xCoord < leftBorder - circuit.binsWidth / 8)
        break;
      if (circuit.arrOfBins[firstRowIdx][firstColIdx]->yCoord < lowerBorder)
        break;
    }
      
    //if (circuit.arrOfBins[lastRowIdx][lastColIdx]->nodes.size() != 0)
    {
      //cellIdx = circuit.arrOfBins[lastRowIdx][lastColIdx]->nodes[0];
      if (circuit.arrOfBins[lastRowIdx][lastColIdx]->xCoord > rightBorder + circuit.binsWidth / 8)
        break;
      if (circuit.arrOfBins[lastRowIdx][lastColIdx]->yCoord > upperBorder)
        break;
    }
    
    if (lastColIdx < circuit.nBinCols - 1) 
    {
      /*int rowIdx = 0;
      while (circuit.arrOfBins[rowIdx][lastColIdx+1]->nodes.size() == 0) ++rowIdx;
      cellIdx = circuit.arrOfBins[rowIdx][lastColIdx+1]->nodes[0];*/
      if (circuit.arrOfBins[0][lastColIdx+1]->xCoord + circuit.binsWidth / 32 <= rightBorder)
        lastColIdx++;
    }
    
    for (int rowIdx = firstRowIdx; rowIdx <= lastRowIdx; ++rowIdx)
      for (int colIdx = firstColIdx; colIdx <= lastColIdx; ++colIdx)
        circuit.arrOfBins[rowIdx][colIdx]->isOnMacroBlock = true;
  }
  
  if (i != circuit.nTerminals)
  {
    cout << "terminal index = " << i << endl;
    return NOT_ONTO_MACRO;
  }
  return OK;
}

ITLDRAGON_ERROR AdjustBinsToMacros(Circuit& circuit)
{
  ITLDRAGON_ERROR errorCode = FlagBins(circuit);
  CheckCode(errorCode);
  
  const float BIN_SHRED_COEF = 0.3f;
  
  int firstColIdx;
  int firstRowIdx;
  int lastColIdx;
  int lastRowIdx;
  
  int firstCurrColIdx;
  int firstCurrRowIdx;
  int lastCurrColIdx;
  int lastCurrRowIdx;
  
  int macroIdx;
  int rowIdxOut;      // indexes of the
  int colIdxOut;      // outer bins
  float leftBorder;
  float rightBorder;
  float lowerBorder;
  float upperBorder;
  int i;
  int counter = 0;
  
  bool isLeftCol  = true;   // determines that there are free bins around
  bool isRightCol = true;
  bool isLowerRow = true;
  bool isUpperRow = true;
  
  for (i = 0; i < circuit.nTerminals; ++i)
  {
    macroIdx = i + circuit.nNodes;
    
    leftBorder  = static_cast<float>(circuit.placement[macroIdx].xCoord - 0.5 * circuit.terminals[i].width);
    rightBorder = static_cast<float>(circuit.placement[macroIdx].xCoord + 0.5 * circuit.terminals[i].width);
    lowerBorder = static_cast<float>(circuit.placement[macroIdx].yCoord - 0.5 * circuit.terminals[i].height);
    upperBorder = static_cast<float>(circuit.placement[macroIdx].yCoord + 0.5 * circuit.terminals[i].height);
    
    if ((rightBorder <= 0.0) || (leftBorder  >= circuit.width) ||
        (upperBorder <= 0.0) || (lowerBorder >= circuit.height))
      continue;
    
    firstColIdx = static_cast<int>(leftBorder  / circuit.binsWidth + 0.49);
    firstRowIdx = static_cast<int>(lowerBorder / circuit.binsHeight);
    lastColIdx  = static_cast<int>(rightBorder / circuit.binsWidth + 0.49 - 1.0);
    lastRowIdx  = static_cast<int>(upperBorder / circuit.binsHeight - 1.0);
    
    firstColIdx = max(0, firstColIdx);
    firstRowIdx = max(0, firstRowIdx);
    lastColIdx  = min(circuit.nBinCols - 1, lastColIdx);
    lastRowIdx  = min(circuit.nBinRows - 1, lastRowIdx);
    
    if (lastColIdx < circuit.nBinCols - 1) 
    {
      //int cellIdx;
      //if (circuit.arrOfBins[lastRowIdx][lastColIdx+1]->nodes.size() != 0)
      {
        //cellIdx = circuit.arrOfBins[lastRowIdx][lastColIdx+1]->nodes[0];
        if (circuit.arrOfBins[lastRowIdx][lastColIdx+1]->xCoord + circuit.binsWidth / 32 <= rightBorder)
          lastColIdx++;
      }
    }
    
    int binsCounter = 0;
    if (firstColIdx == 0 || circuit.arrOfBins[firstRowIdx][firstColIdx - 1]->isOnMacroBlock == true)
    {
      isLeftCol  = false;
      if (firstColIdx != 0)
      {
        // if there are only several not flagged bins
        // we assume isLeftCol = true
        for (int k = firstRowIdx; k <= lastRowIdx; ++k)
          if (circuit.arrOfBins[k][firstColIdx - 1]->isOnMacroBlock == false)
          {
            binsCounter++;
          }
        if (binsCounter > (lastRowIdx - firstRowIdx)/8) isLeftCol  = true;
      }
    }
    /*if (firstRowIdx == 0 || circuit.arrOfBins[firstRowIdx - 1][firstColIdx]->isOnMacroBlock == true)
    {
      isLowerRow = false;
      
    }*/
    if (firstRowIdx != 0)
    {
      for (int k = firstColIdx; k <= lastColIdx; ++k)
        if (circuit.arrOfBins[firstRowIdx - 1][k]->isOnMacroBlock == true)
        {
          isLowerRow = false;
          break;
        }
    }
    else isLowerRow = false;
    if (lastColIdx  == circuit.nBinCols - 1 ||
        circuit.arrOfBins[firstRowIdx][lastColIdx + 1]->isOnMacroBlock == true)
    {
      isRightCol = false;
      if (lastColIdx  != circuit.nBinCols - 1)
      {
        // if there are only several not flagged bins
        // we assume isLeftCol = true
        binsCounter = 0;
        for (int k = firstRowIdx; k <= lastRowIdx; ++k)
          if (circuit.arrOfBins[k][lastColIdx + 1]->isOnMacroBlock == false)
          {
            binsCounter++;
          }
        if (binsCounter > (lastRowIdx - firstRowIdx)/8) isRightCol  = true;
      }
    }
    /*if (lastRowIdx  == circuit.nBinRows - 1 ||
        circuit.arrOfBins[lastRowIdx + 1][firstColIdx]->isOnMacroBlock == true)
    {
      isUpperRow = false;
      
    }*/
    if (lastRowIdx  != circuit.nBinRows - 1)
    {
      for (int k = firstColIdx; k <= lastColIdx; ++k)
        if (circuit.arrOfBins[lastRowIdx + 1][k]->isOnMacroBlock == true)
        {
          isUpperRow = false;
          break;
        }
    }
    else isUpperRow = false;
    
    if (isLeftCol)
    {
      //rowIdxOut = firstRowIdx;
      
      firstCurrColIdx = firstColIdx;
      firstCurrRowIdx = static_cast<int>((lowerBorder + BIN_SHRED_COEF * circuit.terminals[i].height) / circuit.binsHeight);
      lastCurrColIdx  = static_cast<int>((leftBorder  + 0.5  * circuit.terminals[i].width ) / circuit.binsWidth + 0.49 - 1.0);
      lastCurrRowIdx  = static_cast<int>((lowerBorder + (1-BIN_SHRED_COEF) * circuit.terminals[i].height) / circuit.binsHeight - 1.0);
      
      rowIdxOut = firstCurrRowIdx;
      colIdxOut = firstCurrColIdx - 1;
      while (rowIdxOut < lastRowIdx && circuit.arrOfBins[rowIdxOut][colIdxOut]->isOnMacroBlock == true) ++rowIdxOut;
      if (rowIdxOut == lastRowIdx) rowIdxOut = firstRowIdx;
      if (!isRightCol)
        if (firstCurrColIdx > 1 && circuit.arrOfBins[rowIdxOut][firstCurrColIdx - 2]->isOnMacroBlock == false) 
          colIdxOut = firstCurrColIdx - 2;
      
      if (!isUpperRow) lastCurrRowIdx = lastRowIdx;
      if (!isLowerRow) firstCurrRowIdx = firstRowIdx;
      
      ScatterBinsVert(circuit.arrOfBins, firstCurrColIdx, firstCurrRowIdx, lastCurrColIdx, lastCurrRowIdx, 
                      rowIdxOut, colIdxOut, firstRowIdx, lastRowIdx, circuit);
      if (!isRightCol)
      {
        colIdxOut = firstCurrColIdx - 1;
        
        firstCurrColIdx = static_cast<int>((leftBorder  + 0.5  * circuit.terminals[i].width ) / circuit.binsWidth + 0.49);
        firstCurrRowIdx = static_cast<int>((lowerBorder + BIN_SHRED_COEF * circuit.terminals[i].height) / circuit.binsHeight);
        lastCurrColIdx  = lastColIdx;
        lastCurrRowIdx  = static_cast<int>((lowerBorder + (1-BIN_SHRED_COEF) * circuit.terminals[i].height) / circuit.binsHeight - 1.0);
        
        if (!isUpperRow) lastCurrRowIdx = lastRowIdx;
        if (!isLowerRow) firstCurrRowIdx = firstRowIdx; 
        
        ScatterBinsVert(circuit.arrOfBins, firstCurrColIdx, firstCurrRowIdx, lastCurrColIdx, lastCurrRowIdx, 
                        rowIdxOut, colIdxOut, firstRowIdx, lastRowIdx, circuit);
      }
    }
    
    if (isRightCol)
    {
      //rowIdxOut = firstRowIdx;
      
      firstCurrColIdx = static_cast<int>((leftBorder  + 0.5  * circuit.terminals[i].width ) / circuit.binsWidth + 0.49);
      firstCurrRowIdx = static_cast<int>((lowerBorder + BIN_SHRED_COEF * circuit.terminals[i].height) / circuit.binsHeight);
      lastCurrColIdx  = lastColIdx;
      lastCurrRowIdx  = static_cast<int>((lowerBorder + (1-BIN_SHRED_COEF) * circuit.terminals[i].height) / circuit.binsHeight - 1.0);
      
      rowIdxOut = firstCurrRowIdx;
      colIdxOut = lastCurrColIdx + 1;
      while (rowIdxOut < lastRowIdx && circuit.arrOfBins[rowIdxOut][colIdxOut]->isOnMacroBlock == true) ++rowIdxOut;
      if (rowIdxOut == lastRowIdx) rowIdxOut = firstRowIdx;
      if (!isLeftCol)
        if (lastCurrColIdx < circuit.nBinCols - 3 && circuit.arrOfBins[rowIdxOut][lastCurrColIdx + 2]->isOnMacroBlock == false) 
          colIdxOut = lastCurrColIdx + 2;
      
      if (!isUpperRow) lastCurrRowIdx  = lastRowIdx;
      if (!isLowerRow) firstCurrRowIdx = firstRowIdx;
      
      ScatterBinsVert(circuit.arrOfBins, firstCurrColIdx, firstCurrRowIdx, lastCurrColIdx, lastCurrRowIdx, 
                      rowIdxOut, colIdxOut, firstRowIdx, lastRowIdx, circuit);
      if (!isLeftCol)
      {
        colIdxOut = lastCurrColIdx + 1;
        
        firstCurrColIdx = firstColIdx;
        firstCurrRowIdx = static_cast<int>((lowerBorder + BIN_SHRED_COEF * circuit.terminals[i].height) / circuit.binsHeight);
        lastCurrColIdx  = static_cast<int>((leftBorder  + 0.5  * circuit.terminals[i].width ) / circuit.binsWidth + 0.49 - 1.0);
        lastCurrRowIdx  = static_cast<int>((lowerBorder + (1-BIN_SHRED_COEF) * circuit.terminals[i].height) / circuit.binsHeight - 1.0);
        
        if (!isUpperRow) lastCurrRowIdx  = lastRowIdx;
        if (!isLowerRow) firstCurrRowIdx = firstRowIdx;
        
        ScatterBinsVert(circuit.arrOfBins, firstCurrColIdx, firstCurrRowIdx, lastCurrColIdx, lastCurrRowIdx, 
                        rowIdxOut, colIdxOut, firstRowIdx, lastRowIdx, circuit);
      }
    }
    
    if (isUpperRow)
    {
      firstCurrColIdx = firstColIdx;
      firstCurrRowIdx = static_cast<int>((lowerBorder + (1-BIN_SHRED_COEF) * circuit.terminals[i].height) / circuit.binsHeight);
      lastCurrColIdx  = lastColIdx;
      lastCurrRowIdx  = lastRowIdx;
      
      rowIdxOut = lastCurrRowIdx + 1;
      colIdxOut = firstCurrColIdx;
      //while (rowIdxOut < lastRowIdx && circuit.arrOfBins[rowIdxOut][colIdxOut]->isOnMacroBlock == true) ++rowIdxOut;
      //if (rowIdxOut == lastRowIdx) rowIdxOut = firstRowIdx;
      
      if (!isLeftCol && !isRightCol)
      {
        firstCurrRowIdx = static_cast<int>((lowerBorder + 0.5 * circuit.terminals[i].height) / circuit.binsHeight);
        if (!isLowerRow)
          firstCurrRowIdx = firstRowIdx;
      }
      
      for (int colIdx = firstCurrColIdx; colIdx <= lastCurrColIdx; ++colIdx)
      {
        ScatterBinsVert(circuit.arrOfBins, colIdx, firstCurrRowIdx, colIdx, lastCurrRowIdx, 
                        rowIdxOut, colIdx, lastCurrRowIdx + 1, 
                        lastCurrRowIdx + (lastCurrRowIdx - firstCurrRowIdx)/2, circuit);
      }
    }
    
    if (isLowerRow)
    {
      firstCurrColIdx = firstColIdx;
      firstCurrRowIdx = firstRowIdx;
      lastCurrColIdx  = lastColIdx;
      lastCurrRowIdx  = static_cast<int>((lowerBorder + BIN_SHRED_COEF * circuit.terminals[i].height) / circuit.binsHeight - 1.0);
      
      rowIdxOut = firstRowIdx - 1;
      colIdxOut = firstCurrColIdx;
      
      if (!isLeftCol && !isRightCol)
      {
        lastCurrRowIdx = static_cast<int>((lowerBorder + 0.5 * circuit.terminals[i].height) / circuit.binsHeight - 1.0);
        if (!isUpperRow)
          lastCurrRowIdx = lastRowIdx;
      }
      
      for (int colIdx = firstCurrColIdx; colIdx <= lastCurrColIdx; ++colIdx)
      {
        ScatterBinsVert(circuit.arrOfBins, colIdx, firstCurrRowIdx, colIdx, lastCurrRowIdx, 
                        rowIdxOut, colIdx, firstCurrRowIdx - (lastCurrRowIdx - firstCurrRowIdx)/2 - 1, 
                        firstCurrRowIdx - 1, circuit);
      }
    }
    
    isLeftCol  = true;   // determines that there are free bins around
    isRightCol = true;
    isLowerRow = true;
    isUpperRow = true;
    
    /*for (int rowIdx = firstRowIdx; rowIdx <= lastRowIdx; ++rowIdx)
      for (int colIdx = firstColIdx; colIdx <= lastColIdx; ++colIdx)
        if (circuit.arrOfBins[rowIdx][colIdx]->nodes.size() != 0)
           cout << "Not empty bin!";*/
  }
  errorCode = CheckBinsOnMacros(circuit);
  if (errorCode != OK) return errorCode;
  return OK;
}

void ScatterBinsVert(pBin** arrOfBins, int firstCurrColIdx, int firstCurrRowIdx, int lastCurrColIdx, int lastCurrRowIdx, 
                     int rowIdxOut, int colIdxOut, int firstRowIdx, int lastRowIdx, Circuit& circuit, int dir)
{ 
  if (dir == 1)
  {
    if (firstRowIdx < 0) firstRowIdx = 0;
    while (rowIdxOut < lastRowIdx && rowIdxOut < circuit.nBinRows && 
                      circuit.arrOfBins[rowIdxOut][colIdxOut]->isOnMacroBlock == true) ++rowIdxOut;
    if (rowIdxOut > lastRowIdx || rowIdxOut >= circuit.nBinRows) rowIdxOut = firstRowIdx;
    
    for (int rowIdx = firstCurrRowIdx; rowIdx <= lastCurrRowIdx; ++rowIdx)
      for (int colIdx = firstCurrColIdx; colIdx <= lastCurrColIdx; ++colIdx)
      {
        MergeTwoBins(circuit.arrOfBins, rowIdxOut, colIdxOut, rowIdx, colIdx, circuit);
        rowIdxOut;
        while (rowIdxOut <= lastRowIdx && rowIdxOut < circuit.nBinRows && 
                      circuit.arrOfBins[rowIdxOut][colIdxOut]->isOnMacroBlock == true) ++rowIdxOut;
        if (rowIdxOut > lastRowIdx || rowIdxOut >= circuit.nBinRows)
        {
          rowIdxOut = firstRowIdx;
          while (rowIdxOut <= lastRowIdx && rowIdxOut < circuit.nBinRows && 
                      circuit.arrOfBins[rowIdxOut][colIdxOut]->isOnMacroBlock == true) ++rowIdxOut;
        }
      }
  }
  else
  {
    while (rowIdxOut > firstRowIdx && rowIdxOut > -1 && 
                      circuit.arrOfBins[rowIdxOut][colIdxOut]->isOnMacroBlock == true) --rowIdxOut;
    if (rowIdxOut < firstRowIdx || rowIdxOut < 0) rowIdxOut = lastRowIdx;
    
    for (int rowIdx = lastCurrRowIdx; rowIdx >= firstCurrRowIdx; --rowIdx)
      for (int colIdx = firstCurrColIdx; colIdx <= lastCurrColIdx; ++colIdx)
      {
        MergeTwoBins(circuit.arrOfBins, rowIdxOut, colIdxOut, rowIdx, colIdx, circuit);
        rowIdxOut--;
        while (rowIdxOut >= firstRowIdx && rowIdxOut > -1 && 
                      circuit.arrOfBins[rowIdxOut][colIdxOut]->isOnMacroBlock == true) --rowIdxOut;
        if (rowIdxOut < firstRowIdx || rowIdxOut < 0) rowIdxOut = lastRowIdx;
      }
  }
}

void MergeTwoBins(pBin** arrOfBins, int row1, int col1, int row2, int col2, Circuit& circuit)
{
  int oldVectorSize;
  //size_t tmpSize;
  int oldNodesSize;
  vector<int> tmpVec;
  
  // Update placement
  int cellIdx;
  //int cellIdx2;
  for (int i = 0; i < static_cast<int>(arrOfBins[row2][col2]->nodes.size()); ++i)
  {
    cellIdx = arrOfBins[row2][col2]->nodes[i];
    //cellIdx2 = arrOfBins[row1][col1]->nodes[0];
    circuit.placement[cellIdx].xCoord = arrOfBins[row1][col1]->xCoord;
    circuit.placement[cellIdx].yCoord = arrOfBins[row1][col1]->yCoord;
  }
  
  // Copy indexes of nodes
  oldNodesSize = oldVectorSize = static_cast<int>(arrOfBins[row1][col1]->nodes.size());
  /*arrOfBins[row1][col1]->nodes.resize(oldVectorSize + arrOfBins[row2][col2]->nodes.size());*/
  tmpVec.resize(oldVectorSize + arrOfBins[row2][col2]->nodes.size());
  merge(arrOfBins[row1][col1]->nodes.begin(), arrOfBins[row1][col1]->nodes.end(), 
        arrOfBins[row2][col2]->nodes.begin(), arrOfBins[row2][col2]->nodes.end(), 
        tmpVec.begin());
  arrOfBins[row1][col1]->nodes.resize(oldVectorSize + arrOfBins[row2][col2]->nodes.size());
  copy(tmpVec.begin(), tmpVec.end(), arrOfBins[row1][col1]->nodes.begin());
  //sort(arrOfBins[row1][col1]->nodes.begin(), arrOfBins[row1][col1]->nodes.end());
  
  arrOfBins[row1][col1]->sumLength += arrOfBins[row2][col2]->sumLength;
  
  // Clear old bin
  arrOfBins[row2][col2]->Clear();
}

void MoveCluster(pBin** arrOfBins, int toRow, int toCol, int fromRow, int fromCol, Circuit& circuit)
{
  int oldVectorSize;
  size_t tmpSize;
  int oldNodesSize;
  vector<int> tmpVec;
  
  // Update placement
  int cellIdx;
  //int cellIdx2;
  for (int i = 0; i < static_cast<int>(arrOfBins[fromRow][fromCol]->nodes.size()); ++i)
  {
    cellIdx = arrOfBins[fromRow][fromCol]->nodes[i];
    //cellIdx2 = arrOfBins[toRow][toCol]->nodes[0];
    circuit.placement[cellIdx].xCoord = arrOfBins[toRow][toCol]->xCoord;
    circuit.placement[cellIdx].yCoord = arrOfBins[toRow][toCol]->yCoord;
  }
  
  // Copy indexes of nodes
  oldNodesSize = oldVectorSize = static_cast<int>(arrOfBins[toRow][toCol]->nodes.size());
  /*arrOfBins[toRow][toCol]->nodes.resize(oldVectorSize + arrOfBins[fromRow][fromCol]->nodes.size());*/
  tmpVec.resize(oldVectorSize + arrOfBins[fromRow][fromCol]->nodes.size());
  merge(arrOfBins[toRow][toCol]->nodes.begin(), arrOfBins[toRow][toCol]->nodes.end(), 
        arrOfBins[fromRow][fromCol]->nodes.begin(), arrOfBins[fromRow][fromCol]->nodes.end(), 
        tmpVec.begin());
  arrOfBins[toRow][toCol]->nodes.resize(oldVectorSize + arrOfBins[fromRow][fromCol]->nodes.size());
  copy(tmpVec.begin(), tmpVec.end(), arrOfBins[toRow][toCol]->nodes.begin());
  //sort(arrOfBins[toRow][toCol]->nodes.begin(), arrOfBins[toRow][toCol]->nodes.end());
  
  // Copy nets     
  oldVectorSize = static_cast<int>(arrOfBins[toRow][toCol]->nets.size());
  arrOfBins[toRow][toCol]->nets.resize(oldVectorSize + arrOfBins[fromRow][fromCol]->nets.size());
  copy(arrOfBins[fromRow][fromCol]->nets.begin(), arrOfBins[fromRow][fromCol]->nets.end(), 
       arrOfBins[toRow][toCol]->nets.begin() + oldVectorSize);
  sort(arrOfBins[toRow][toCol]->nets.begin(), arrOfBins[toRow][toCol]->nets.end());
  
  /*int tmpIdx = arrOfBins[toRow][toCol]->nets[arrOfBins[toRow][toCol]->nets.size() - 1];
  int i = 0;
  unique(arrOfBins[toRow][toCol]->nets.begin(), arrOfBins[toRow][toCol]->nets.end());
  while (tmpIdx != arrOfBins[toRow][toCol]->nets[i]) ++i;
  arrOfBins[toRow][toCol]->nets.resize(i+1);*/
  
  // Fill cwts in
  arrOfBins[toRow][toCol]->cwts.clear();
  for (int i = 0; i < static_cast<int>(tmpVec.size()); ++i)
  {
    arrOfBins[toRow][toCol]->cwts.push_back(circuit.nodes[tmpVec[i]].width);
  }
  /*oldVectorSize = static_cast<int>(arrOfBins[toRow][toCol]->cwts.size());
  arrOfBins[toRow][toCol]->cwts.resize(oldVectorSize + arrOfBins[fromRow][fromCol]->cwts.size());
  copy(arrOfBins[fromRow][fromCol]->cwts.begin(), arrOfBins[fromRow][fromCol]->cwts.end(), 
       arrOfBins[toRow][toCol]->cwts.begin() + oldVectorSize);*/
  
  // Copy indexes
  tmpSize = arrOfBins[toRow][toCol]->hedges.size();
  for (int i = 0; i < static_cast<int>(arrOfBins[fromRow][fromCol]->indexes.size()); ++i)
  {
    arrOfBins[fromRow][fromCol]->indexes[i] += static_cast<int>(tmpSize);
  }
  oldVectorSize = static_cast<int>(arrOfBins[toRow][toCol]->indexes.size());
  arrOfBins[toRow][toCol]->indexes.resize(oldVectorSize - 1 + arrOfBins[fromRow][fromCol]->indexes.size());
  copy(arrOfBins[fromRow][fromCol]->indexes.begin(), arrOfBins[fromRow][fromCol]->indexes.end(), 
       arrOfBins[toRow][toCol]->indexes.begin() + oldVectorSize - 1);
  
  // Copy hedges
  for (int i = 0; i < static_cast<int>(arrOfBins[fromRow][fromCol]->hedges.size()); ++i)
  {
    arrOfBins[fromRow][fromCol]->hedges[i] += oldNodesSize;
  }
  oldVectorSize = static_cast<int>(arrOfBins[toRow][toCol]->hedges.size());
  arrOfBins[toRow][toCol]->hedges.resize(oldVectorSize + arrOfBins[fromRow][fromCol]->hedges.size());
  copy(arrOfBins[fromRow][fromCol]->hedges.begin(), arrOfBins[fromRow][fromCol]->hedges.end(), 
       arrOfBins[toRow][toCol]->hedges.begin() + oldVectorSize);
  
  /*arrOfBins[toRow][toCol]->hedges.push_back(arrOfBins[toRow][toCol]->nodes[0]);?
  arrOfBins[toRow][toCol]->hedges.push_back(arrOfBins[fromRow][fromCol]->nodes[0]);
  arrOfBins[toRow][toCol]->indexes.push_back(arrOfBins[toRow][toCol]->hedges.size() - 1);*/
  
  arrOfBins[toRow][toCol]->sumLength += arrOfBins[fromRow][fromCol]->sumLength;
  
  // Clear old bin
  arrOfBins[fromRow][fromCol]->Clear();
}

int AdjustBinWidths(Circuit& circuit, double* rowLengths)
{
  int errorCode = OK;

  // check legality of Bins
  //errorCode = CheckBins(circuit);
  //if (errorCode != OK) return errorCode;

  double rowWidth = circuit.rows[0].siteWidth * circuit.rows[circuit.nRows/2].numSites;
  // for white space distribution
  
  double binWidth;
  double leftBorder = 0.0;
  double rightBorder = 0.0;
  double whitespace;
  double ratioOfFreeSpace;
  double currSumLength = 0.0;
  int firstColIdx;
  int lastColIdx;
  int spanIdx;
  int cellIdx;
 
  for (int rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
  {
    for (spanIdx = 0; spanIdx < static_cast<int>(circuit.rowTerminalBorders[rowIdx].size() / 2); ++spanIdx)
    {
      leftBorder  = circuit.rowTerminalBorders[rowIdx][2*spanIdx];
      rightBorder = circuit.rowTerminalBorders[rowIdx][2*spanIdx+1];
      
      firstColIdx = static_cast<int>(leftBorder  / circuit.binsWidth + 0.49);
      lastColIdx  = static_cast<int>(rightBorder / circuit.binsWidth + 0.49 - 1.0);
      
      currSumLength = 0.0;
      for (int colIdx = firstColIdx; colIdx <= lastColIdx; ++colIdx)
      {
        currSumLength += circuit.arrOfBins[rowIdx][colIdx]->sumLength;
      }
      
      ratioOfFreeSpace = (rightBorder - leftBorder - currSumLength) / currSumLength;
      
      for (int colIdx = firstColIdx; colIdx <= lastColIdx; ++colIdx)
      {
        whitespace = ratioOfFreeSpace * circuit.arrOfBins[rowIdx][colIdx]->sumLength;
        binWidth = circuit.arrOfBins[rowIdx][colIdx]->sumLength + whitespace;
        for (int i = 0; i < static_cast<int>(circuit.arrOfBins[rowIdx][colIdx]->nodes.size()); ++i)
        {
          cellIdx = circuit.arrOfBins[rowIdx][colIdx]->nodes[i];
          circuit.placement[cellIdx].xCoord = leftBorder + binWidth / 2;
        }
        circuit.arrOfBins[rowIdx][colIdx]->xCoord = leftBorder + binWidth / 2;
        leftBorder += binWidth;
      }
    }
  }
  /*int cellIdx;
  for (int rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
  {
    for (int colIdx = 0; colIdx < circuit.nBinCols; ++colIdx)
    {
      for (int i = 0; i < static_cast<int>(circuit.arrOfBins[rowIdx][colIdx]->nodes.size()); ++i)
      {
        cellIdx = circuit.arrOfBins[rowIdx][colIdx]->nodes[i];
        if (circuit.placement[cellIdx].xCoord + 0.5 * circuit.nodes[cellIdx].width >
            circuit.placement[circuit.nNodes + 3].xCoord - 0.5 * circuit.terminals[3].width &&
            circuit.placement[cellIdx].xCoord - 0.5 * circuit.nodes[cellIdx].width <
            circuit.placement[circuit.nNodes + 3].xCoord + 0.5 * circuit.terminals[3].width &&
            circuit.placement[cellIdx].yCoord > 
            circuit.placement[circuit.nNodes + 3].yCoord - 0.5 * circuit.terminals[3].height &&
            circuit.placement[cellIdx].yCoord < 
            circuit.placement[circuit.nNodes + 3].yCoord + 0.5 * circuit.terminals[3].height)
        {
          return OK;
        }
      }
    }
  }*/
  
  // check legality of bins
  /*errorCode = CheckBins(circuit);
  if (errorCode != OK) return errorCode;*/
    
  return OK;
}


int AdjustRowWidths(Circuit& circuit)
{   
  // check legality of Bins
  int errorCode = OK;

  // check legality of Bins
  errorCode = CheckBins(circuit);
  if (errorCode != OK) return errorCode;

  // we aim to distribute total cell area equally between rows
  // next variable is desired cell width per row
  double desiredRowWidth = 0.0;
  double rowWidth = circuit.rows[0].siteWidth * circuit.rows[circuit.nRows/2].numSites;
  
  for (int i = 0; i < circuit.nNodes; i++ )
  {
    desiredRowWidth += circuit.nodes[i].width;
  }

  desiredRowWidth /= circuit.nBinRows;
  cout << "desired row width " << desiredRowWidth << "\n";
  
  if (DirectAdjusting(circuit, desiredRowWidth, 1))
  {
      DirectAdjusting(circuit, desiredRowWidth, -1);
  }
 
  // check legality of bins
  errorCode = CheckBins(circuit);
  if (errorCode != OK) return errorCode;
  
  return OK;
}

int DirectAdjusting(Circuit& circuit, double desiredRowWidth, int direction)
{
  // we are trying to make total cell width in row close to the
  // desired row width and then legalize circuit.placement within row
  // free sites distributed between cells equally
  if (direction == 1) // bottom-up direction
  {
    for (int rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
    {
      if (AdjustWidthOfOneRow(circuit, rowIdx, desiredRowWidth, direction))
      return 1;
    }
  }
  else // top-down direction
  {
    for (int rowIdx = circuit.nBinRows - 1; rowIdx >= 0; rowIdx-- )
    {
      if (AdjustWidthOfOneRow( circuit, rowIdx, desiredRowWidth, direction))
      return 0;
    }
  }

  return 0;
}

int AdjustWidthOfOneRow(Circuit& circuit, int rowIdx, double desiredRowWidth, int direction)
{
  double currWidth = 0.0;
  double leftBorder = 0.0;
  int numCellsInRow = 0;
  int cellIdx = 0;
  double cellWidth = 0.0;
  int colIdx = 0;
  static double rowWidth = circuit.rows[0].siteWidth * circuit.rows[circuit.nRows/2].numSites;
  static double siteWidth = circuit.rows[0].siteWidth;
  double Yopt;

  // (1) we calculate total width of cells in this row
  for (colIdx = 0; colIdx < circuit.nBinCols; ++colIdx )
  {
    for (int i = 0; i < static_cast<int>(circuit.arrOfBins[rowIdx][colIdx]->nodes.size()); i++ )
    {
      currWidth += circuit.nodes[circuit.arrOfBins[rowIdx][colIdx]->nodes[i]].width;
      numCellsInRow++;
    }
  }
  bool isAnyMove;

  // (2) then we do total cells width closer to desired row's width by exchange of
  //   cells with lower row
  // (2.1) if total width less than row's width, we'll add cells from 
  // lower bins while total width of cells stayed less than desired row's width
  if( (currWidth < desiredRowWidth*0.95) && (rowIdx != circuit.nBinRows - 1) )
  {
    if (direction == -1) 
    {
      return 1;
    }

    // we will get cells from upper row (only if their optimal y-coordinate 
    // is greater than current y-coordinate) to achieve desired row width
    while(true)
    {
      colIdx = rand() % circuit.nBinCols;
      isAnyMove = false;

      if (circuit.arrOfBins[rowIdx+direction][colIdx]->nodes.size() < 2) 
        continue;

      for (int i = 0; i < static_cast<int>(circuit.arrOfBins[rowIdx+direction][colIdx]->nodes.size()); i++ )
      {
        cellIdx = circuit.arrOfBins[rowIdx+direction][colIdx]->nodes[i];
        Yopt = GetOptimalY( &cellIdx, 1, circuit.placement,
                             circuit.nNets, circuit.nets, 
                             circuit.tableOfConnections,
                             NULL, -1);
        if ( ((direction ==  1)&&(circuit.placement[cellIdx].yCoord > Yopt))||
             ((direction == -1)&&(circuit.placement[cellIdx].yCoord < Yopt))  )
        {
          circuit.arrOfBins[rowIdx][colIdx]->nodes.push_back( cellIdx );
          currWidth += circuit.nodes[cellIdx].width;            
          numCellsInRow++;
          isAnyMove = true;
          circuit.arrOfBins[rowIdx+direction][colIdx]->nodes[i] = circuit.arrOfBins[rowIdx+direction][colIdx]->nodes.back();
          circuit.arrOfBins[rowIdx+direction][colIdx]->nodes.pop_back();
          circuit.placement[cellIdx].yCoord = circuit.placement[circuit.arrOfBins[rowIdx][colIdx]->nodes[0]].yCoord;
          break; // we move only one cell from each bin
        }
      }
      if (!isAnyMove)
      {// we will move one cell forcibly
        cellIdx = circuit.arrOfBins[rowIdx+direction][colIdx]->nodes.back();
        circuit.arrOfBins[rowIdx][colIdx]->nodes.push_back( cellIdx );
        currWidth += circuit.nodes[cellIdx].width;            
        numCellsInRow++;
        circuit.arrOfBins[rowIdx+direction][colIdx]->nodes.pop_back();
        circuit.placement[cellIdx].yCoord = circuit.placement[circuit.arrOfBins[rowIdx][colIdx]->nodes[0]].yCoord;
      }
      if (currWidth >= desiredRowWidth*0.95) break;        
    }
  } 
  // (2.2) if total width larger than row's width, we'll move cells to the 
  // adjacent bins while total width of cells stayed larger than desired row's width
  else if (currWidth > desiredRowWidth*1.05)
  {
    if( (rowIdx == circuit.nBinRows - 1) && (direction == 1) )
      return 1;

    while(true)
    {
      colIdx = rand() % circuit.nBinCols;
      isAnyMove = false;
      
      if(circuit.arrOfBins[rowIdx][colIdx]->nodes.size() < 2)
        continue;

      for (int i = 0; i < static_cast<int>(circuit.arrOfBins[rowIdx][colIdx]->nodes.size()); i++ )
      {
        cellIdx = circuit.arrOfBins[rowIdx][colIdx]->nodes[i];
        Yopt = GetOptimalY( &cellIdx, 1, circuit.placement,
                             circuit.nNets, circuit.nets, 
                             circuit.tableOfConnections,
                             NULL, -1);
        if ( ((direction ==  1)&&(circuit.placement[cellIdx].yCoord < Yopt))||
             ((direction == -1)&&(circuit.placement[cellIdx].yCoord > Yopt))  )
        {
          circuit.arrOfBins[rowIdx+direction][colIdx]->nodes.push_back( cellIdx );
          currWidth -= circuit.nodes[cellIdx].width;            
          numCellsInRow--;
          isAnyMove = true;
          circuit.arrOfBins[rowIdx][colIdx]->nodes[i] = circuit.arrOfBins[rowIdx][colIdx]->nodes.back();
          circuit.arrOfBins[rowIdx][colIdx]->nodes.pop_back();
          circuit.placement[cellIdx].yCoord = circuit.placement[circuit.arrOfBins[rowIdx+direction][colIdx]->nodes[0]].yCoord;
          break; // we move only one cell from each bin
        }
      }
      if (!isAnyMove)
      {// we will move one cell forcibly
        cellIdx = circuit.arrOfBins[rowIdx][colIdx]->nodes.back();
        circuit.arrOfBins[rowIdx+direction][colIdx]->nodes.push_back( cellIdx );
        currWidth -= circuit.nodes[cellIdx].width;            
        numCellsInRow--;
        circuit.arrOfBins[rowIdx][colIdx]->nodes.pop_back();
        circuit.placement[cellIdx].yCoord = circuit.placement[circuit.arrOfBins[rowIdx+direction][colIdx]->nodes[0]].yCoord;
      }
      if( currWidth < desiredRowWidth*1.05 )      
        break;
    }// while(true)
  }

  cout << "row " << rowIdx << "\ttotal cell width: " << currWidth;
  cout << "\t" << currWidth/desiredRowWidth*100 << "% of desired\n";

  return 0;
}

void DistribCellsAmongBins(Circuit& circuit)
{
  vector<int> allNodes;
  int avrNCellsPerBin;
  int nExtraCells;
  int exactNumOfBins = 0;
  int currCellNum = 0;
  int currRealBinIdx = 0;
  int nCellsToPush;
  int cellIdx;
  
  for (int rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
    for (int colIdx = 0; colIdx < circuit.nBinCols; ++colIdx)
    {
      for (int k = 0; k < static_cast<int>(circuit.arrOfBins[rowIdx][colIdx]->nodes.size()); ++k)
        allNodes.push_back(circuit.arrOfBins[rowIdx][colIdx]->nodes[k]);
        
      circuit.arrOfBins[rowIdx][colIdx]->nodes.clear();
      
      if (circuit.arrOfBins[rowIdx][colIdx]->isOnMacroBlock == false) exactNumOfBins++;
    }
  
  avrNCellsPerBin = static_cast<int>(circuit.nNodes / exactNumOfBins);
  nExtraCells     = circuit.nNodes - avrNCellsPerBin * exactNumOfBins;
  
  int k;
  for (int rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
  {
    for (int colIdx = 0; colIdx < circuit.nBinCols; ++colIdx)
    {
      if (circuit.arrOfBins[rowIdx][colIdx]->isOnMacroBlock == false)
      {
        if (currRealBinIdx + 1 <= nExtraCells)
          nCellsToPush = avrNCellsPerBin + 1;
        else
          nCellsToPush = avrNCellsPerBin;
      
        circuit.arrOfBins[rowIdx][colIdx]->sumLength = 0;
        for (k = currCellNum; k < currCellNum + nCellsToPush; ++k)
        {
          cellIdx = allNodes[k];
          circuit.arrOfBins[rowIdx][colIdx]->nodes.push_back(cellIdx);
          circuit.arrOfBins[rowIdx][colIdx]->sumLength += circuit.nodes[cellIdx].width;
          circuit.placement[cellIdx].xCoord = (colIdx + 0.5) * circuit.binsWidth;
          circuit.placement[cellIdx].yCoord = (rowIdx + 0.5) * circuit.binsHeight;
        }
        
        currCellNum += nCellsToPush;
        currRealBinIdx++;
      }
    }
  }
}
