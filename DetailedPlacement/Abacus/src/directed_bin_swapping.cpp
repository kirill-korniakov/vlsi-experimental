/*  
 * directed_bin_swapping.cpp
 * this is a part of itlAnalyticalPlacer
 * Copyright (C) 2006, ITLab, Kirill Kornyakov
 * email: kirillkornyakov@yandex.ru
 */

#include "..\include\directed_bin_swapping.h"
#include "math.h"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;


const int DFLT_NUM_OF_NEIGHBOURS = 5;  // numbers neghbourhoods (with optimal bin)
const int DFLT_SIZE_OF_CHAIN     = 3;   // maximum number of participants in exchanging
const double DFLT_STOPPING_CRITERIA_VALUE = 0.0001;

void ApplyCircularlyInterchanging( int lastIdxInChain, BinPosition* chain, pBin** Bins, Place* placement )
{
  double firstBinXCoord;
  double firstBinYCoord;
  firstBinXCoord = Bins[chain[0].binRowIdx][chain[0].binColIdx]->xCoord;
  firstBinYCoord = Bins[chain[0].binRowIdx][chain[0].binColIdx]->yCoord;
    
  // virtually swapping bins (change only coordinates of elements)
  int i, j;
  int fromRow, fromCol;
  int toRow, toCol;
  double toX, toY;
  
  // do interchange except last move (last cluster to the current cluster place)
  for (i = 0; i < lastIdxInChain; ++i)
  {
    fromRow = chain[i].binRowIdx;
    fromCol = chain[i].binColIdx;
    toRow = chain[i+1].binRowIdx;
    toCol = chain[i+1].binColIdx;
    
    toX = Bins[toRow][toCol]->xCoord;
    toY = Bins[toRow][toCol]->yCoord;
    for (j = 0; j < static_cast<int>(Bins[fromRow][fromCol]->nodes.size()); ++j)
    {
      placement[ Bins[fromRow][fromCol]->nodes[j] ].xCoord = toX;
      placement[ Bins[fromRow][fromCol]->nodes[j] ].yCoord = toY;
    }
    Bins[fromRow][fromCol]->xCoord = toX;
    Bins[fromRow][fromCol]->yCoord = toY;
  }

  fromRow = chain[lastIdxInChain].binRowIdx;
  fromCol = chain[lastIdxInChain].binColIdx;
  for (j = 0; j < static_cast<int>(Bins[fromRow][fromCol]->nodes.size()); ++j)
  {
    placement[Bins[fromRow][fromCol]->nodes[j]].xCoord = firstBinXCoord;
    placement[Bins[fromRow][fromCol]->nodes[j]].yCoord = firstBinYCoord;
  }
  Bins[fromRow][fromCol]->xCoord = firstBinXCoord;
  Bins[fromRow][fromCol]->yCoord = firstBinYCoord;
}

void ApplyBackwardInterchanging( int lastIdxInChain, BinPosition* chain, pBin** Bins, Place* placement )
{  
  double lastBinXCoord;
  double lastBinYCoord;
  lastBinXCoord = Bins[chain[lastIdxInChain].binRowIdx][chain[lastIdxInChain].binColIdx]->xCoord;
  lastBinYCoord = Bins[chain[lastIdxInChain].binRowIdx][chain[lastIdxInChain].binColIdx]->yCoord;
  
  // virtually swapping bins (change only coordinates of elements)
  int i, j;
  int fromRow, fromCol;
  int toRow, toCol;
  double toX, toY;
  
  // do interchange except last move (last cluster to the current cluster place)
  for (i = lastIdxInChain; i > 0; i--)
  {
    fromRow = chain[i].binRowIdx;
    fromCol = chain[i].binColIdx;
    toRow = chain[i-1].binRowIdx;
    toCol = chain[i-1].binColIdx;    
    
    toX = Bins[toRow][toCol]->xCoord;
    toY = Bins[toRow][toCol]->yCoord;
    for (j = 0; j < static_cast<int>(Bins[fromRow][fromCol]->nodes.size()); ++j)
    {
      placement[ Bins[fromRow][fromCol]->nodes[j] ].xCoord = toX;
      placement[ Bins[fromRow][fromCol]->nodes[j] ].yCoord = toY;
    }
    Bins[fromRow][fromCol]->xCoord = toX;
    Bins[fromRow][fromCol]->yCoord = toY;
  }

  fromRow = chain[0].binRowIdx;
  fromCol = chain[0].binColIdx;
  for (j = 0; j < static_cast<int>(Bins[fromRow][fromCol]->nodes.size()); ++j)
  {
    placement[Bins[fromRow][fromCol]->nodes[j]].xCoord = lastBinXCoord;
    placement[Bins[fromRow][fromCol]->nodes[j]].yCoord = lastBinYCoord;
  }
  Bins[fromRow][fromCol]->xCoord = lastBinXCoord;
  Bins[fromRow][fromCol]->yCoord = lastBinYCoord;
}

void FillChangedNodesVector( vector<int>* changedNodes, int lastIdxInChain, BinPosition* chain, pBin** Bins )
{
  changedNodes->clear();
  int size    = 0;
  int oldSize = 0;
  for (int i = 0; i <= lastIdxInChain; ++i)
  {
    size = static_cast<int>(Bins[chain[i].binRowIdx][chain[i].binColIdx]->nodes.size());
    changedNodes->resize( oldSize + size ); 
    std::copy( Bins[chain[i].binRowIdx][chain[i].binColIdx]->nodes.begin(),
               Bins[chain[i].binRowIdx][chain[i].binColIdx]->nodes.end(),
               changedNodes->begin() + oldSize);
    oldSize += size;
  }
}

void GetNextNeighbour( int idxOfNeighbour, int& rowIdx, int& colIdx, int optRowIdx, int optColIdx )
{
  switch(idxOfNeighbour)
  {
    case 0:
    {
      rowIdx = optRowIdx;
      colIdx = optColIdx;
      return;
    }
    case 1:
    {
      rowIdx = optRowIdx + 1;
      colIdx = optColIdx;
      return;
    }
    case 2:
    {
      rowIdx = optRowIdx - 1;
      colIdx = optColIdx;
      return;
    }
    case 3:
    {
      rowIdx = optRowIdx;
      colIdx = optColIdx - 1;
      return;
    }
    case 4:
    {
      rowIdx = optRowIdx;
      colIdx = optColIdx + 1;
      return;
    }
    case 5:
    {
      rowIdx = optRowIdx + 1;
      colIdx = optColIdx - 1;
      return;
    }
    case 6:
    {
      rowIdx = optRowIdx + 1;
      colIdx = optColIdx + 1;
      return;
    }
    case 7:
    {
      rowIdx = optRowIdx - 1;
      colIdx = optColIdx - 1;
      return;
    }
    case 8:
    {
      rowIdx = optRowIdx - 1;
      colIdx = optColIdx + 1;
      return;
    }
    case 9:
    {
      rowIdx = optRowIdx + 2;
      colIdx = optColIdx;
      return;
    }
    case 10:
    {
      rowIdx = optRowIdx - 2;
      colIdx = optColIdx;
      return;
    }
    case 11:
    {
      rowIdx = optRowIdx + 2;
      colIdx = optColIdx - 1;
      return;
    }
    case 12:
    {
      rowIdx = optRowIdx + 2;
      colIdx = optColIdx + 1;
      return;
    }
    case 13:
    {
      rowIdx = optRowIdx - 2;
      colIdx = optColIdx - 1;
      return;
    }
    case 14:
    {
      rowIdx = optRowIdx - 2;
      colIdx = optColIdx + 1;
      return;
    }
  }
}

// this function tries to move clusters to their optimal region
// method is based on the following paper:
// S. Goto "An Efficient Algorithm for the Two-Dimensional Placement Problem 
// in Electrical Circuit Layout"
int DirectedBinSwapping(Circuit& circuit, Statistics& statistics, double* paramsOfDBS)
{
  int i, j, k, l;

  int numOfNeighbours = DFLT_NUM_OF_NEIGHBOURS;
  int sizeOfChain     = DFLT_SIZE_OF_CHAIN;
   double stoppingCriteriaValue = DFLT_STOPPING_CRITERIA_VALUE;

  if (paramsOfDBS[0] != -1) numOfNeighbours = static_cast<int>(paramsOfDBS[0]);
  if (paramsOfDBS[1] != -1) sizeOfChain = static_cast<int>(paramsOfDBS[1]);
  if (sizeOfChain == 0) return OK;
  if (paramsOfDBS[2] != -1) stoppingCriteriaValue = paramsOfDBS[2];
  
  //Chain is an array of clusters, which will be interchanged clockwise
  BinPosition** Chains;
  Chains = new  BinPosition*[numOfNeighbours];
  for (i = 0; i < numOfNeighbours; ++i)
    Chains[i] = new BinPosition[sizeOfChain];

  double xOpt, yOpt;               // coordinates of the optimal point
  int    optRowIdx, optColIdx;     // row and column of the optimal bin
  int    bestRowIdx, bestColIdx;
  double oldWL, newWL, bestNewWL;

  vector<int> changedNodes;
  bool isInterchanged;

  int iterCounter = 0;

  cout << "i#\tWL before\tWL after\tReduction\n";
  // do while there is good benefit from last iteration
  int errorCode = OK;
  do
  { 
    iterCounter++;
    cout << iterCounter << "\t" << statistics.currentWL << "\t";
    oldWL = statistics.currentWL;

    //Try to move every cluster to its optimal region
    for (i = 0; i < circuit.nBinRows; ++i)
    {
      for (j = 0; j < circuit.nBinCols; ++j)
      {
        if (circuit.arrOfBins[i][j]->isOnMacroBlock == true)
          continue;
        
        isInterchanged = false;
      
        //  Add current cluster to the every Chain
        for (k = 0; k < numOfNeighbours; ++k)
        {
          Chains[k][0].binRowIdx = i;
          Chains[k][0].binColIdx = j;
        }

        // determine optimal bin for the last cluster in chain
        GetOptimalXY( &circuit.arrOfBins[i][j]->nodes,
                      circuit.placement, 
                      circuit.nNets, circuit.nets, 
                      circuit.tableOfConnections, NULL, NULL,
                      &xOpt, &yOpt, -1 );
        // shift if optimal bin is extreme
        optRowIdx = static_cast<int>(floor((yOpt - circuit.rows[0].coordinate)/circuit.binsHeight));
        optRowIdx = max( 2, optRowIdx );
        optRowIdx = min( circuit.nBinRows - 3, optRowIdx );
        optColIdx = static_cast<int>(floor((xOpt - circuit.rows[0].subrowOrigin)/circuit.binsWidth));
        optColIdx = max( 1, optColIdx );
        optColIdx = min( circuit.nBinCols - 2, optColIdx );
        
        // Get next cluster if optimal bin is a current bin
        if ((optRowIdx==i)&&(optColIdx==j)) continue;
      
        for (k = 0; k < numOfNeighbours; ++k)
        {
          GetNextNeighbour( k, Chains[k][1].binRowIdx, Chains[k][1].binColIdx, optRowIdx, optColIdx );
          if (circuit.arrOfBins[Chains[k][1].binRowIdx][Chains[k][1].binColIdx]->isOnMacroBlock == true)
            continue;
          
          ApplyCircularlyInterchanging(1, Chains[k], circuit.arrOfBins, circuit.placement);
          FillChangedNodesVector(&changedNodes, 1, Chains[k], circuit.arrOfBins);
          newWL = cf_recalc_some_nodes1(0, circuit.nNets, circuit.nets, statistics.currentWL, &changedNodes, 
                                        static_cast<int>(changedNodes.size()),
                                        circuit.tableOfConnections, circuit.placement);
          if (newWL < statistics.currentWL)
          {
            statistics.currentWL = cf_recalc_some_nodes1(1, circuit.nNets, circuit.nets, statistics.currentWL, 
                                                         &changedNodes, static_cast<int>(changedNodes.size()),
                                                         circuit.tableOfConnections, circuit.placement);            
            // bins interchanging
            pBin tmpBin;
            int lastBinRowIdx = Chains[k][1].binRowIdx;
            int lastBinColIdx = Chains[k][1].binColIdx;
            tmpBin = circuit.arrOfBins[lastBinRowIdx][lastBinColIdx];
            circuit.arrOfBins[Chains[k][1].binRowIdx][Chains[k][1].binColIdx] = 
            circuit.arrOfBins[Chains[k][0].binRowIdx][Chains[k][0].binColIdx];
            circuit.arrOfBins[Chains[k][0].binRowIdx][Chains[k][0].binColIdx] = tmpBin;
            isInterchanged = true;
            break;
          }
          else
          {
            ApplyBackwardInterchanging(1, Chains[k], circuit.arrOfBins, circuit.placement);
          }
          if (isInterchanged) break;
        }
        if (isInterchanged) continue; // get next cluster

        // in next loop we will try to find chain of interchanges which will reduce the WL
        for (k = 1; k < sizeOfChain - 1; ++k)
        {
          // for every chain calculate we will add new neighbour and calculate
          // the benefit from exchanging according to the Chain
          // and if there will be reduction in WL, we will apply changes
          for (int currChain = 0; currChain < numOfNeighbours; currChain++)
          {
            if (circuit.arrOfBins[Chains[currChain][k].binRowIdx][Chains[currChain][k].binColIdx]->isOnMacroBlock == true)
              continue;
            
            bestNewWL = 0.0;
            
            // determine optimal bin for the last cluster in chain
            GetOptimalXY( &circuit.arrOfBins[Chains[currChain][k].binRowIdx][Chains[currChain][k].binColIdx]->nodes,
                          circuit.placement, 
                          circuit.nNets, circuit.nets, 
                          circuit.tableOfConnections, NULL, NULL,
                          &xOpt, &yOpt, -1 );
            // shift if optimal bin is extreme
            optRowIdx = static_cast<int>(floor((yOpt - circuit.rows[0].coordinate)/circuit.binsHeight));
            optColIdx = static_cast<int>(floor((xOpt - circuit.rows[0].subrowOrigin)/circuit.binsWidth));
            optRowIdx = static_cast<int>(floor((yOpt - circuit.rows[0].coordinate)/circuit.binsHeight));
            optRowIdx = max( 2, optRowIdx );
            optRowIdx = min( circuit.nBinRows - 3, optRowIdx );
            optColIdx = static_cast<int>(floor((xOpt - circuit.rows[0].subrowOrigin)/circuit.binsWidth));
            optColIdx = max( 1, optColIdx );
            optColIdx = min( circuit.nBinCols - 2, optColIdx );

            // try to add new neighbour and calculate benefit
            for (l = 0; l < numOfNeighbours; l++)
            {
              GetNextNeighbour(l, Chains[currChain][k+1].binRowIdx, Chains[currChain][k+1].binColIdx, 
                                 optRowIdx, optColIdx);
              if (circuit.arrOfBins[Chains[currChain][k+1].binRowIdx][Chains[currChain][k+1].binColIdx]->isOnMacroBlock == true)
                continue;
              
              ApplyCircularlyInterchanging(k + 1, Chains[currChain], circuit.arrOfBins, circuit.placement);
              FillChangedNodesVector(&changedNodes, k + 1, Chains[currChain], circuit.arrOfBins);
              newWL = cf_recalc_some_nodes1(0, circuit.nNets, circuit.nets, statistics.currentWL, &changedNodes, 
                                            static_cast<int>(changedNodes.size()),
                                            circuit.tableOfConnections, circuit.placement );
              if (newWL < statistics.currentWL)
              {
                statistics.currentWL = cf_recalc_some_nodes1(1, circuit.nNets, circuit.nets, statistics.currentWL,
                                                             &changedNodes, static_cast<int>(changedNodes.size()),
                                                             circuit.tableOfConnections, circuit.placement);
                isInterchanged = true;
                
                // bins interchanging
                pBin tmpBin;
                int lastBinRowIdx = Chains[currChain][k+1].binRowIdx;
                int lastBinColIdx = Chains[currChain][k+1].binColIdx;
                tmpBin = circuit.arrOfBins[lastBinRowIdx][lastBinColIdx];
                for (int s = k+1; s > 0; s--)
                {
                  circuit.arrOfBins[Chains[currChain][s].binRowIdx][Chains[currChain][s].binColIdx] = 
                  circuit.arrOfBins[Chains[currChain][s-1].binRowIdx][Chains[currChain][s-1].binColIdx];
                }
                circuit.arrOfBins[Chains[currChain][0].binRowIdx][Chains[currChain][0].binColIdx] = tmpBin;
                break;
              }
              else
              {
                if( bestNewWL == 0.0 )
                {
                  bestNewWL  = newWL;
                  bestRowIdx = Chains[currChain][k+1].binRowIdx;
                  bestColIdx = Chains[currChain][k+1].binColIdx;
                }
                else if(newWL < bestNewWL)
                {
                  bestNewWL  = newWL;
                  bestRowIdx = Chains[currChain][k+1].binRowIdx;
                  bestColIdx = Chains[currChain][k+1].binColIdx;
                }
                ApplyBackwardInterchanging(k+1, Chains[currChain], circuit.arrOfBins, circuit.placement);
              }
            } // try to add new neighbour and calculate benefit
            
            if (isInterchanged) break;
            else // add best neighbour
            {
              Chains[currChain][k+1].binRowIdx = bestRowIdx;
              Chains[currChain][k+1].binColIdx = bestColIdx;
            }
          }
          if (isInterchanged) break;
        } // REPEAT sizeOfChain times

      } // FOR each cluster
    } // FOR each cluster

    cout << statistics.currentWL << "\t";
    cout << (1 - statistics.currentWL / oldWL)*100 << "%\n";

    // check legality of circuit.arrOfBins
    errorCode = CheckBins(circuit);
    if (errorCode != OK) 
      return errorCode;

  } while((1 - statistics.currentWL / oldWL) > stoppingCriteriaValue );
    
  return errorCode;
}
