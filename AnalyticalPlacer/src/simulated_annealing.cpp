/*   
 * simulated_annealing.cpp
 * this is a part of itlDragon
 * Copyright (C) 2005, ITLab, Nina Kurina
 * email: nina.kurina@gmail.com
 * last modified 29 September, 2006
 */

#include "..\include\simulated_annealing.h"
#include <algorithm>

#include <conio.h>
#include <math.h>     // for math functions exp(), log()
#include <time.h>     // for timing
#include <stdlib.h>
#include <fstream>    // for file streams
#include <iostream>   // for output of additional information

using namespace std;

extern __int64 rdtsc(); 

// default values for SA parameters
const double DFLT_INIT_TEMP = 100; // initial temperature
const double DFLT_LOWER_TEMP = 0.0000001; // temperature at which algorithm stops 
const int DFLT_ATT = 300; // determines the number of tries 
const double DFLT_DUMPING_FACTOR = 0.95; // determines the temperature schedule
const double DFLT_PART_OF_MOVE_DISPLACING = 0.1; // the percentage of displacing move type
const double DFLT_VRL = 0.1; // spans of range limiter window
const double DFLT_HRL = 0.1;
const double DFLT_BIN_UNBALANCE = 0.5;  // allowable unbalance of sum length of cells in a bin
const double DFLT_ROW_UNBALANCE = 0.01;  // allowable unbalance of sum length of cells in a row

// parameters for wl improvement control
const double DFLT_IMPR_THRESHOLD = 0.01;
const int    DFLT_NUM_OF_TLEVELS = 5;
const double DFLT_TLEVEL_START_IMPR_CONTROL = 10;

const double DESIRED_AP = 0.3;
const double INIT_COST_FACTOR = 1;

const double eps = 1e-10;

double desiredBinLength = 0; // desired sum length of cells in a bin
double* desiredRowLength = 0; // desired sum length of cells in a row
double oldCurrX, oldCurrY; // old coordinates of current cell
double oldCurrBinLength, oldNewBinLength; // old lengths of bins that take part in a move
double oldCurrRowLength, oldNewRowLength; // old lengths of rows that take part in a move
double* rowLengths = NULL;  // sum lengths of cells in rows  
int currBinRowIdx = -1;
int currBinColIdx = -1; 

MULTIPLACER_ERROR errorCode = OK;

inline MULTIPLACER_ERROR TryDisplace( const int currCellIdx, 
           const int currBinRowIdx, const int currBinColIdx,
           const int newBinRowIdx, const int newBinColIdx, 
           pBin** const arrOfBins, Place* const placement, Node* const nodes )                    
{
  // we get coordinates of the new bin's center
  static int tempCellIdx;
  static double tempCellWidth;
  //tempCellIdx   = arrOfBins[newBinRowIdx][newBinColIdx]->nodes[0];
  tempCellWidth = nodes[currCellIdx].width;
  
  // we change coordinates of the node with index currCellIdx
  placement[currCellIdx].xCoord = arrOfBins[newBinRowIdx][newBinColIdx]->xCoord;
  placement[currCellIdx].yCoord = arrOfBins[newBinRowIdx][newBinColIdx]->yCoord;
  arrOfBins[currBinRowIdx][currBinColIdx]->sumLength -= tempCellWidth;
  arrOfBins[newBinRowIdx][newBinColIdx]->sumLength   += tempCellWidth;
  rowLengths[currBinRowIdx] -= tempCellWidth;
  rowLengths[newBinRowIdx]  += tempCellWidth;
  return OK;
}

inline MULTIPLACER_ERROR TryExchange( const int currCellIdx, const int newCellIdx, 
           const int currBinRowIdx, const int currBinColIdx,
           const int newBinRowIdx, const int newBinColIdx, 
           pBin** const arrOfBins, Place* const placement, Node* const nodes )           
{    
  // we exchange coordinates of 2 cells with indexes currCellIdx and newCellIdx
  static double tempx;
  static double tempy;
  static double widthDifference;

  // exchange coordinates
  tempx = placement[currCellIdx].xCoord;
  tempy = placement[currCellIdx].yCoord;
  placement[currCellIdx].xCoord = placement[newCellIdx].xCoord;
  placement[currCellIdx].yCoord = placement[newCellIdx].yCoord;
  placement[newCellIdx].xCoord = tempx;
  placement[newCellIdx].yCoord = tempy;
  
  // recalc widths
  widthDifference = nodes[currCellIdx].width - nodes[newCellIdx].width;
  arrOfBins[currBinRowIdx][currBinColIdx]->sumLength -= widthDifference;
  arrOfBins[newBinRowIdx][newBinColIdx]->sumLength   += widthDifference;
  rowLengths[currBinRowIdx] -= widthDifference;
  rowLengths[newBinRowIdx]  += widthDifference;
  return OK;
}
 
inline MULTIPLACER_ERROR RejectDisplace( const int currCellIdx, 
           const int currBinRowIdx, const int currBinColIdx,
           const int newBinRowIdx, const int newBinColIdx, 
           pBin** const arrOfBins, Place* const placement )
{
  placement[currCellIdx].xCoord = oldCurrX;
  placement[currCellIdx].yCoord = oldCurrY;
  arrOfBins[currBinRowIdx][currBinColIdx]->sumLength = oldCurrBinLength;
  arrOfBins[newBinRowIdx][newBinColIdx]->sumLength   = oldNewBinLength;
  rowLengths[currBinRowIdx] = oldCurrRowLength;
  rowLengths[newBinRowIdx]  = oldNewRowLength;
  return OK;
}

inline MULTIPLACER_ERROR RejectExchange( const int currCellIdx, int newCellIdx, 
           const int currBinRowIdx, const int currBinColIdx,
           const int newBinRowIdx, const int newBinColIdx, 
           pBin** const arrOfBins, Place* const placement )
{
  // we exchange coordinates of 2 cells with indexes currCellIdx and newCellIdx
  static double tempx;
  static double tempy;
 
  // exchange coordinates
  tempx = placement[currCellIdx].xCoord;
  tempy = placement[currCellIdx].yCoord;
  placement[currCellIdx].xCoord = placement[newCellIdx].xCoord;
  placement[currCellIdx].yCoord = placement[newCellIdx].yCoord;
  placement[newCellIdx].xCoord = tempx;
  placement[newCellIdx].yCoord = tempy;
 
  // recalc widths
  arrOfBins[currBinRowIdx][currBinColIdx]->sumLength = oldCurrBinLength;
  arrOfBins[newBinRowIdx][newBinColIdx]->sumLength   = oldNewBinLength;
  rowLengths[currBinRowIdx] = oldCurrRowLength;
  rowLengths[newBinRowIdx]  = oldNewRowLength;
  return OK;
}

inline int IsBinBalanceBetter( const int binRowIdx, const int binColIdx, const double oldLength, 
           pBin** const arrOfBins, const double binUnbalance )
{
  static double newLength;
  static double upperBound = desiredBinLength*(1 + binUnbalance);
  static double lowerBound = desiredBinLength*(1 - binUnbalance);

  newLength = arrOfBins[binRowIdx][binColIdx]->sumLength;
  if (newLength >= oldLength)
  {
    if (newLength > upperBound) return false;
  }
  else if (newLength < lowerBound) return false;
  
  return true;  
}

inline int IsRowBalanceBetter( const int rowIdx, const double oldLength, const double rowUnbalance )
{
  static double newLength;
  double upperBound = desiredRowLength[rowIdx]*(1 + rowUnbalance);
  double lowerBound = desiredRowLength[rowIdx]*(1 - rowUnbalance);

  newLength = rowLengths[rowIdx];
  if (newLength >= oldLength)
  {
    if (newLength > upperBound) return false;
  }
  else if (newLength < lowerBound) return false;
  
  return true;
}

inline MULTIPLACER_ERROR GenerateRandomNeighbour( const int currBinRowIdx, const int currBinColIdx,
                            const int numOfBinRows, const int numOfBinCols, 
                            int* const horOffset, int* const vertOffset,
                            const int idxOfRandNum,
                            int& newBinRowIdx, int& newBinColIdx )
{
  // we randomly pick a new bin considering range limiters, 
  // current bin is the center of the bounding window
  newBinColIdx = min(max(0, currBinColIdx + horOffset[idxOfRandNum]),  numOfBinCols - 1);
  newBinRowIdx = min(max(0, currBinRowIdx + vertOffset[idxOfRandNum]), numOfBinRows - 1);
  return OK;
}

inline MULTIPLACER_ERROR GenerateTopologicalNeighbour( const int currCellIdx, const int numOfBinCols,
                                 const int numOfNets, Net* const nets,
                                 vector<int>* tableOfConnections, pBin** const arrOfBins,
                                 const double binHeight, Place* placement,
                                 int& newBinRowIdx, int& newBinColIdx, const int numOfNodes,
                                 float* const randNet, float* const randPin, const int idxOfRandNum )
{
  static int netIdx;
  static int pinIdx;
  static double binCenterXCoord;
  static double neighbourCellXCoord, neighbourCellYCoord;
  static int neighbourCellIdx;
  
  netIdx = static_cast<int>( randNet[idxOfRandNum] * tableOfConnections[currCellIdx].size() );
  pinIdx = static_cast<int>( randPin[idxOfRandNum] * nets[netIdx].numOfPins );
  neighbourCellIdx = nets[netIdx].arrPins[pinIdx].cellIdx;
  
  if (neighbourCellIdx >= numOfNodes)
  { // if neighbour is a terminal
    newBinRowIdx = currBinRowIdx; 
    newBinColIdx = currBinColIdx;
    return OK;
  }

  neighbourCellYCoord = placement[neighbourCellIdx].yCoord;
  newBinRowIdx = static_cast<int>( neighbourCellYCoord / binHeight );
  neighbourCellXCoord = placement[neighbourCellIdx].xCoord;

  static int left, right, middle;
  //int counter;
  left  = 0;
  right = numOfBinCols - 1;
  while (true)
  {
    middle = (left + right) / 2;
    /*counter = newBinRowIdx;
    if (arrOfBins[newBinRowIdx][middle]->isOnMacroBlock == true)
    { 
      counter = 0;
      while (arrOfBins[counter][middle]->isOnMacroBlock == true)
        counter++;
    }*/
    binCenterXCoord = arrOfBins[newBinRowIdx][middle]->xCoord;
    if (binCenterXCoord < neighbourCellXCoord - eps)
    {
      left = middle + 1;
    }
    else if (binCenterXCoord > neighbourCellXCoord + eps)
    {
      right = middle - 1;
    }
    else
    {
      newBinColIdx = middle;
      return OK;
    }
  }
  return OK;
}

inline MULTIPLACER_ERROR GenerateOptTopologicalNeighbour( const int currCellIdx, const int numOfBinCols, const int numOfBinRows,
                                         const int numOfNets, Net* const nets,
                                         vector<int>* tableOfConnections, pBin** const arrOfBins,
                                         const double binHeight, Place* placement,
                                         int& newBinRowIdx, int& newBinColIdx, const int numOfNodes,
                                         float* const randNet, float* const randPin, const int idxOfRandNum )
{
  static int netIdx;
  static double binCenterXCoord;
  static double neighbourCellXCoord, neighbourCellYCoord;
  static int neighbourCellIdx;
  static int idx;
  static double xOpt, yOpt;

  netIdx = static_cast<int>( randNet[idxOfRandNum] * tableOfConnections[currCellIdx].size() );
  
  vector<double> xSeries;
  vector<double> ySeries;
  for (int i=0; i<nets[netIdx].numOfPins; ++i)
  { 
    neighbourCellIdx = nets[netIdx].arrPins[i].cellIdx;
    if (neighbourCellIdx == currCellIdx) continue;

    xSeries.push_back(placement[neighbourCellIdx].xCoord);
    ySeries.push_back(placement[neighbourCellIdx].yCoord);        
  }
  //1.2 Calculate the series' medians
  sort(xSeries.begin(), xSeries.end());
  idx = static_cast<int>(xSeries.size() / 2);
  if (idx == 0) xOpt = xSeries[0];
  else          xOpt = (xSeries[idx] + xSeries[idx-1]) / 2;

  sort(ySeries.begin(), ySeries.end());
  idx = static_cast<int>(ySeries.size() / 2);
  if (idx == 0) yOpt = ySeries[idx];
  else          yOpt = (ySeries[idx] + ySeries[idx-1]) / 2;

  newBinRowIdx = min(max(static_cast<int>( yOpt / binHeight ), 0), numOfBinRows - 1);
  newBinColIdx = 0;
  static int left, right, middle;
  //int counter = 0;
  left  = 0;
  right = numOfBinCols - 1;
  while (!(right == left + 1))
  {
    middle = (left+right)/2;
    /*counter = newBinRowIdx;
    if (arrOfBins[newBinRowIdx][middle]->isOnMacroBlock == true)
    { 
      counter = 0;
      while (counter < numOfBinRows && arrOfBins[counter][middle]->isOnMacroBlock == true)
        counter++;
    }*/
    binCenterXCoord = arrOfBins[newBinRowIdx][middle]->xCoord;
    if (binCenterXCoord < xOpt - eps)
    {
      left = middle;
    }
    else if (binCenterXCoord > xOpt + eps)
    {
      right = middle;
    }
    else
    {
      left = right = middle;
      break;
    }
  }
  
  double leftX;
  double rightX;
  
  /*counter = newBinRowIdx;
  if (arrOfBins[newBinRowIdx][left]->isOnMacroBlock == true)
  {
    counter = 0;
    while (counter < numOfBinRows && arrOfBins[counter][left]->isOnMacroBlock == true)
      counter++;
  }*/
  leftX = arrOfBins[newBinRowIdx][left]->xCoord;
  
  /*counter = newBinRowIdx;
  if (arrOfBins[newBinRowIdx][right]->isOnMacroBlock == true)
  {
    counter = 0;
    while (counter < numOfBinRows && arrOfBins[counter][right]->isOnMacroBlock == true)
      counter++;
  }*/
  rightX = arrOfBins[newBinRowIdx][right]->xCoord;
  
  if ( xOpt - leftX > 
       rightX - xOpt  )
  {
    newBinColIdx = right;
  }
  else
    newBinColIdx = left;
}

inline MULTIPLACER_ERROR GenerateOptimalNeighbour( int currCellIdx, const int numOfBinCols, const int numOfBinRows, 
                             const int numOfNets, Net* const nets, 
                             vector<int>* tableOfConnections, pBin** const arrOfBins,
                             const double binHeight, Place* placement,  
                             int& newBinRowIdx, int& newBinColIdx )
{
  static double xOpt;
  static double yOpt;
  static double binCenterXCoord;
  static double leftBorder;
  static double halfBinWidth;

  GetOptimalXY41( currCellIdx, numOfNets, nets, 
                  placement, tableOfConnections, xOpt, yOpt );

  newBinRowIdx = min(max(static_cast<int>( yOpt / binHeight ), 0), numOfBinRows - 1);
  newBinColIdx = 0;
  static int left, right, middle;
  //int counter = 0;
  left  = 0;
  right = numOfBinCols - 1;
  while (!(right == left + 1))
  {
    middle = (left+right)/2;
    /*counter = newBinRowIdx;
    if (arrOfBins[newBinRowIdx][middle]->isOnMacroBlock == true)
    { 
      counter = 0;
      while (counter < numOfBinRows && arrOfBins[counter][middle]->isOnMacroBlock == true)
        counter++;
    }*/
    binCenterXCoord = arrOfBins[newBinRowIdx][middle]->xCoord;
    if (binCenterXCoord < xOpt - eps)
    {
      left = middle;
    }
    else if (binCenterXCoord > xOpt + eps)
    {
      right = middle;
    }
    else
    {
      left = right = middle;
      break;
    }
  }
  
  double leftX;
  double rightX;
  
  /*counter = newBinRowIdx;
  if (arrOfBins[newBinRowIdx][left]->isOnMacroBlock == true)
  {
    counter = 0;
    while (counter < numOfBinRows && arrOfBins[counter][left]->isOnMacroBlock == true)
      counter++;
  }*/
  leftX = arrOfBins[newBinRowIdx][left]->xCoord;
  
  /*counter = newBinRowIdx;
  if (arrOfBins[newBinRowIdx][right]->isOnMacroBlock == true)
  {
    counter = 0;
    while (counter < numOfBinRows && arrOfBins[counter][right]->isOnMacroBlock == true)
      counter++;
  }*/
  rightX = arrOfBins[newBinRowIdx][right]->xCoord;
  
  if ( xOpt - leftX > 
       rightX - xOpt  )
  {
    newBinColIdx = right;
  }
  else
    newBinColIdx = left;

  return OK;
}

MULTIPLACER_ERROR SimulatedAnnealing(Circuit& circuit, Statistics& statistics)
{
  pBin** const arrOfBins = circuit.arrOfBins;
  // parameters of SA
  double initTemp = DFLT_INIT_TEMP; 
  double lowerTemp = DFLT_LOWER_TEMP; 
  double dumpingFactor = DFLT_DUMPING_FACTOR;
  int ATT = DFLT_ATT;
  double partOfMoveDisplace = DFLT_PART_OF_MOVE_DISPLACING;
  // next 2 parameters are spans of range limiter window
  int vertRangeLimiter = max(static_cast<int>(DFLT_VRL * circuit.nBinRows), 3); 
  int horRangeLimiter = max(static_cast<int>(DFLT_HRL * circuit.nBinCols), 3) ;
  double binUnbalance = DFLT_BIN_UNBALANCE; 
  double rowUnbalance = DFLT_ROW_UNBALANCE; 
  double imprThreshold = DFLT_IMPR_THRESHOLD;
  int numOfTLevels = DFLT_NUM_OF_TLEVELS;
  double TLevelStartImprControl = DFLT_TLEVEL_START_IMPR_CONTROL;
  
  if (gOptions.innerParameters[CA_IDX][0] != -1) initTemp = gOptions.innerParameters[CA_IDX][0];
  if (gOptions.innerParameters[CA_IDX][1] != -1) lowerTemp = gOptions.innerParameters[CA_IDX][1];
  if (gOptions.innerParameters[CA_IDX][2] != -1) dumpingFactor = gOptions.innerParameters[CA_IDX][2];
  if (gOptions.innerParameters[CA_IDX][3] != -1) ATT = static_cast<int>(gOptions.innerParameters[CA_IDX][3]);
  if (gOptions.innerParameters[CA_IDX][4] != -1) partOfMoveDisplace  = gOptions.innerParameters[CA_IDX][4];
  if (gOptions.innerParameters[CA_IDX][5] != -1) 
  { 
    if  (gOptions.innerParameters[CA_IDX][5] <= 1)
    {
      vertRangeLimiter = max(static_cast<int>(circuit.nBinRows*gOptions.innerParameters[CA_IDX][5]), 3);
    }
    else 
    {
      vertRangeLimiter = max(static_cast<int>(gOptions.innerParameters[CA_IDX][5]), 3);
    }
  }
  if (gOptions.innerParameters[CA_IDX][6] != -1)
  {
    if  ((gOptions.innerParameters[CA_IDX][6]) <= 1)
    {
      horRangeLimiter = max(static_cast<int>(circuit.nBinCols*gOptions.innerParameters[CA_IDX][6]), 3);
    }
    else 
    {
      horRangeLimiter = max(static_cast<int>(gOptions.innerParameters[CA_IDX][6]), 3);
    }
  }
  if (gOptions.innerParameters[CA_IDX][7] != -1) binUnbalance = gOptions.innerParameters[CA_IDX][7];
  if (gOptions.innerParameters[CA_IDX][8] != -1) rowUnbalance = gOptions.innerParameters[CA_IDX][8];
  if (gOptions.innerParameters[CA_IDX][9] != -1) imprThreshold = gOptions.innerParameters[CA_IDX][9];
  if (gOptions.innerParameters[CA_IDX][10] != -1) numOfTLevels = static_cast<int>(gOptions.innerParameters[CA_IDX][10]);
  if (gOptions.innerParameters[CA_IDX][11] != -1) TLevelStartImprControl = gOptions.innerParameters[CA_IDX][11];
  
  int numOfBins = 0;
  // height of bins which equals height of row
  double binHeight = circuit.rows[0].height;
  double rowWidth = circuit.rows[0].siteWidth * circuit.rows[circuit.nRows/2].numSites;
  double entireNodesWidth = 0.0;
  double* availableRowLengths = new double [circuit.nBinRows];
  double leftX;
  double rightX;
  
  for (int i = 0; i < circuit.nNodes; ++i)
  {
    entireNodesWidth += circuit.nodes[i].width;
  }
  
  rowLengths = new double [circuit.nBinRows];
  // we calculate initial values for lengths of circuit.rows
  int rowIdx = 0;
  for (rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
  {
    rowLengths[rowIdx] = 0;
    for (int colIdx = 0; colIdx < circuit.nBinCols; ++colIdx)
    {
      for (int i = 0; i < static_cast<int>(arrOfBins[rowIdx][colIdx]->nodes.size()); ++i)
      {
        rowLengths[rowIdx] += circuit.nodes[arrOfBins[rowIdx][colIdx]->nodes[i]].width;
      }
      if (arrOfBins[rowIdx][colIdx]->isOnMacroBlock == false)
        ++numOfBins;
    }
  }  
  desiredBinLength = entireNodesWidth / numOfBins;

  desiredRowLength = new double[circuit.nBinRows];
  /*for (rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
  {
    desiredRowLength[rowIdx] = entirecircuit.nodesWidth / circuit.nBinRows;
  }*/
  
  
  for (rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
  {
    desiredRowLength[rowIdx] = circuit.rows[rowIdx].siteWidth * circuit.rows[rowIdx].numSites;
  }
 
  /*for (int i = 0; i < circuit.nNodes; ++i)
  {
    if (circuit.nodes[i].height / binHeight > 1)
    {
      rowIdx = static_cast<int>((circuit.placement[i].yCoord - 
                 0.5 * circuit.nodes[i].height) / binHeight);
      
      if (rowIdx < 0 || rowIdx >= circuit.nBinRows) continue;
      
      for (int j = 0; j < circuit.nodes[i].height / binHeight; ++j)
        desiredRowLength[rowIdx + j] -= circuit.nodes[i].width;
    }
  }*/
  for (int i = 0; i < circuit.nTerminals; ++i)
  {
    //if (circuit.terminals[i].height / binHeight > 1)
    {
      rowIdx = static_cast<int>((circuit.placement[i + circuit.nNodes].yCoord - 
                0.5 * circuit.terminals[i].height) / binHeight);
      
      leftX  = circuit.placement[i + circuit.nNodes].xCoord - 0.5 * circuit.terminals[i].width;
      rightX = circuit.placement[i + circuit.nNodes].xCoord + 0.5 * circuit.terminals[i].width;
      
      if (rowIdx < 0 || rowIdx >= circuit.nBinRows || leftX >= circuit.width || rightX < 0)
        continue;
  
      for (int j = rowIdx; j < min(rowIdx + circuit.terminals[i].height / binHeight, circuit.nBinRows); ++j)
        desiredRowLength[j] -= circuit.terminals[i].width;
    }
  }
  double totalSpaceInRows = 0.0;
  for (rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
  {
    totalSpaceInRows += desiredRowLength[rowIdx];
  }
  for (rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
  {
    if (desiredRowLength[rowIdx] < rowLengths[rowIdx])
    {
      cout << rowIdx << "\t" << desiredRowLength[rowIdx] << "\t" << rowLengths[rowIdx] << "\t>\n";
      //printf("%d\t%f\t%f\t>\n", rowIdx, desiredRowLength[rowIdx], rowLengths[rowIdx]);
    }
    //else
    //{
    //  cout << rowIdx << "\t" << desiredRowLength[rowIdx] << "\t" << rowLengths[rowIdx] << "\t<=\n";
    //  //printf("%d\t%f\t%f\t<=\n", rowIdx, desiredRowLength[rowIdx], rowLengths[rowIdx]);
    //}
    
    availableRowLengths[rowIdx] = desiredRowLength[rowIdx];
    desiredRowLength[rowIdx] = (desiredRowLength[rowIdx] / totalSpaceInRows) *
                               (entireNodesWidth);
  }
  
  /*cout << "RNG Test\n";
  float *test = new float[1000000];
  __int64 startTime  = 0;
  __int64 finishTime = 0;
  for (int i = 1; i < 9; ++i)
  {
    startTime = rdtsc();
    GetFloatRandomNumbers(test, 100000 * i, 0.0, 1.0);
    finishTime = rdtsc();
    cout << "N = " << 100000 * i << "\t";
    cout << static_cast<double>((finishTime - startTime) / 100000 / i) << endl;
  }
  delete []test;*/
  
  double T;  // temperature
  double delta; // change in wire length

  
  int currCellIdxInBin = -1;
  int currCellIdx = -1;
  int newBinRowIdx = -1; 
  int newBinColIdx = -1; 
  int newCellIdxInBin = -1;
  int newCellIdx = -1;
  
  // number of cells that changed their positions after the move
  int numOfChangedNodes = 0; 
  
  int nodesIdx[2];
  double newWL  = 0;
  double currWL = 0;
  double initWL = 0;
  // relative  improvement of wirelength after simulated annealing
  double relativeImprovement;

  currWL = cf_recalc_all(1, circuit.nNets, circuit.nets, circuit.placement); 
  initWL = currWL;  
  cout << "\nWire length before simulated-annealing:\t " << initWL << "\n";
  cout << "\nIter\tT\ttry\tgen\taccept\tAP\tWL\t\tReduction\tdisplace" << "\n";
   
  // random numbers
  int* randCurrBinRow;
  int* randCurrBinCol;
  float* randNewBinRow;
  float* randNewBinCol;
  float* randCurrIdxInBin;
  float* randNewIdxInBin;
  int* horOffset;
  int* vertOffset;
  float* prob3;
  float* prob4; 
  float* randNet;
  float* randPin;

  // number of iterations at constant temperature
  int numOfIterAtConstTemp = numOfBins * ATT * 2;
  int numOfRandomNumbers = 200000;

  randCurrBinRow=new int[numOfRandomNumbers];
  randCurrBinCol=new int[numOfRandomNumbers];
  randNewBinRow=new float[numOfRandomNumbers];
  randNewBinCol=new float[numOfRandomNumbers];
  randCurrIdxInBin=new float[numOfRandomNumbers];
  randNewIdxInBin=new float[numOfRandomNumbers];
  horOffset=new int[numOfRandomNumbers];
  vertOffset=new int[numOfRandomNumbers];
  prob3=new float[numOfRandomNumbers];
  prob4=new float[numOfRandomNumbers];
  randNet=new float[numOfRandomNumbers];
  randPin=new float[numOfRandomNumbers];

  __int64 rngStartTime  = 0;
  __int64 rngFinishTime = 0;
  
  int idxOfRandNum = -1;
  int triesCounter, genCounter, acceptCounter, displaceCounter;
  double AP = 1.0; // SA acceptance probability

  int TLevelCounter = 0;
  double wlBefore, wlAfter;
  double sumImpr = 0;

  int numOfWarmUpIterations = 10;
  int temperIterCounter= 0; 
  double costFactor = INIT_COST_FACTOR;
  int frequencyOfBWA = numOfIterAtConstTemp / 150;

  int flagForDumpingFactor = 0;

  int a = 0; // random number for new neighbour generation

  int offset1 = horRangeLimiter/2;
  int offset2 = vertRangeLimiter/2;

  T = initTemp;

  clock_t start, finish;
  start = clock();      // start timer
  // we lower the temperature multiplying it by the dumpingFactor
  // the algorithm stops when temperature becomes < lowerTemp
  while (T >= lowerTemp)
  {
    temperIterCounter++;
    if (temperIterCounter < numOfWarmUpIterations + 2)
    {
      numOfIterAtConstTemp = numOfBins * ATT;
    }
    if (temperIterCounter == numOfWarmUpIterations + 2)
    {
      numOfIterAtConstTemp = numOfBins * ATT * 2;
    }
    idxOfRandNum = -1;

    if (T < TLevelStartImprControl)
    {
      TLevelCounter++;
    }
    wlBefore = currWL;

    triesCounter = 0;
    genCounter = 0;
    acceptCounter = 0;
    displaceCounter = 0;

    /*for (int rowIdx = 0; rowIdx < circuit.nRows; ++rowIdx)
    {
      if (errorCode = CheckRowLength(circuit, availableRowLengths[rowIdx], rowIdx) != OK) CheckCode(errorCode);
    }*/
    
    // number of iterations of inner loop  = numOfIterAtConstTemp
    for (int i = 0; i < numOfIterAtConstTemp; ++i)
    {
      //cout << i << "\t";
      
      triesCounter ++;
      idxOfRandNum ++;
      if (idxOfRandNum % numOfRandomNumbers == 0) 
      {
        idxOfRandNum = 0;
        
        rnCATotalCount += numOfRandomNumbers;
        rngStartTime = rdtsc();  
      
        GetIntegerRandomNumbers(randCurrBinRow, numOfRandomNumbers, 0, circuit.nBinRows);
        GetIntegerRandomNumbers(randCurrBinCol, numOfRandomNumbers, 0, circuit.nBinCols);
        GetFloatRandomNumbers(randNewBinRow, numOfRandomNumbers, 0.0, 1.0);
        GetFloatRandomNumbers(randNewBinCol, numOfRandomNumbers, 0.0, 1.0);
        GetFloatRandomNumbers(randCurrIdxInBin, numOfRandomNumbers, 0.0, 1.0);
        GetFloatRandomNumbers(randNewIdxInBin, numOfRandomNumbers, 0.0, 1.0);
        GetIntegerRandomNumbers(horOffset, numOfRandomNumbers, -offset1, offset1);
        GetIntegerRandomNumbers(vertOffset, numOfRandomNumbers, -offset2, offset2);
        GetFloatRandomNumbers(prob3, numOfRandomNumbers, 0.0, 1.0);
        GetFloatRandomNumbers(prob4, numOfRandomNumbers, 0.0, 1.0); 
        GetFloatRandomNumbers(randNet, numOfRandomNumbers, 0.0, 0.999f);
        GetFloatRandomNumbers(randPin, numOfRandomNumbers, 0.0, 0.999f); 
       
        rngFinishTime = rdtsc();  
        rngCATotalTime += rngFinishTime - rngStartTime;
      }
      newBinRowIdx = -1;
      newBinColIdx = -1;
      // we pick a random bin
      currBinRowIdx = randCurrBinRow[idxOfRandNum];
      currBinColIdx = randCurrBinCol[idxOfRandNum];
      // if the bin is empty we choose another 
      if (arrOfBins[currBinRowIdx][currBinColIdx]->nodes.size() == 0)
      {
        continue;
      }
      // we pick a cell in the bin
      if (randCurrIdxInBin[idxOfRandNum] == 1.0)
      {
        currCellIdxInBin = static_cast<int>(arrOfBins[currBinRowIdx][currBinColIdx]->nodes.size() - 1);
      }
      else
      {
        currCellIdxInBin = static_cast<int>(floor(randCurrIdxInBin[idxOfRandNum]*
          arrOfBins[currBinRowIdx][currBinColIdx]->nodes.size()));
      }
      currCellIdx = arrOfBins[currBinRowIdx][currBinColIdx]->nodes[currCellIdxInBin];
      
      /*cout << "cellIdx = " << currCellIdx << "\t(" << circuit.placement[currCellIdx].xCoord << ";" <<
              circuit.placement[currCellIdx].yCoord << ")\tbin[" << currBinRowIdx << "][" << currBinColIdx << "]\t(" <<
              arrOfBins[currBinRowIdx][currBinColIdx]->xCoord << ";" << arrOfBins[currBinRowIdx][currBinColIdx]->yCoord << ")" << endl;*/
      
      // we try to generate a trial state
      a = rand() % 10;
      if ((a>=0)&&(a<=5))
      {
        errorCode = GenerateRandomNeighbour( currBinRowIdx, currBinColIdx,
                                 circuit.nBinRows, circuit.nBinCols,
                                 horOffset, vertOffset,
                                 idxOfRandNum,
                                 newBinRowIdx, newBinColIdx );
      }else if ((a>=6)&&(a<=7))
      {
        errorCode = GenerateTopologicalNeighbour( currCellIdx, circuit.nBinCols, circuit.nNets, circuit.nets,
                                 circuit.tableOfConnections, arrOfBins, binHeight, circuit.placement,
                                 newBinRowIdx, newBinColIdx, circuit.nNodes, 
                                 randNet, randPin, idxOfRandNum );
      }else if ((a>=8)&&(a<=9))
      {
        errorCode = GenerateOptimalNeighbour( currCellIdx, circuit.nBinCols, circuit.nBinRows, circuit.nNets, circuit.nets,
                                 circuit.tableOfConnections, arrOfBins, binHeight, circuit.placement,
                                 newBinRowIdx, newBinColIdx );
      }
      
      if ((newBinRowIdx == currBinRowIdx) && (newBinColIdx == currBinColIdx) ||
          arrOfBins[newBinRowIdx][newBinColIdx]->isOnMacroBlock == true)
      {
        continue;
      }
      //save old values
      oldCurrX = circuit.placement[currCellIdx].xCoord;
      oldCurrY = circuit.placement[currCellIdx].yCoord;
      oldCurrBinLength = arrOfBins[currBinRowIdx][currBinColIdx]->sumLength;
      oldNewBinLength = arrOfBins[newBinRowIdx][newBinColIdx]->sumLength;
      oldCurrRowLength = rowLengths[currBinRowIdx];
      oldNewRowLength = rowLengths[newBinRowIdx];
    
      // if the bin contains only one cell we will perform exchange!
      if ((arrOfBins[newBinRowIdx][newBinColIdx]->nodes.size() == 0)||(prob3[idxOfRandNum] < partOfMoveDisplace)) 
      { 
        // we try to perform displace 
        TryDisplace( currCellIdx, currBinRowIdx, currBinColIdx,
              newBinRowIdx, newBinColIdx, arrOfBins, circuit.placement, circuit.nodes );
        newCellIdx = -1;
      }
      else
      {
        // we try to perform exchange
        // we randomly choose a cell in the new bin 
        // it`s guaranteed that new bin is not empty
        if (randNewIdxInBin[idxOfRandNum] == 1.0)
        {
          newCellIdxInBin = static_cast<int>(arrOfBins[newBinRowIdx][newBinColIdx]->nodes.size() - 1);
        }
        else
        {
          newCellIdxInBin = static_cast<int>(floor(randNewIdxInBin[idxOfRandNum]*
            arrOfBins[newBinRowIdx][newBinColIdx]->nodes.size()));
        }
        newCellIdx = arrOfBins[newBinRowIdx][newBinColIdx]->nodes[newCellIdxInBin];         
        TryExchange( currCellIdx, newCellIdx, currBinRowIdx, currBinColIdx,
          newBinRowIdx, newBinColIdx, arrOfBins, circuit.placement, circuit.nodes );
      } 
      // if the move violates row or bin balance that we want to keep, we reject the move
      if ((!IsRowBalanceBetter(currBinRowIdx, oldCurrRowLength, rowUnbalance)) || 
        (!IsRowBalanceBetter(newBinRowIdx, oldNewRowLength, rowUnbalance)) || 
        (!IsBinBalanceBetter(currBinRowIdx, currBinColIdx, oldCurrBinLength, arrOfBins, binUnbalance)) || 
        (!IsBinBalanceBetter(newBinRowIdx, newBinColIdx, oldNewBinLength, arrOfBins, binUnbalance))) 
      {
        // we find out what type of move took place
        if (newCellIdx == -1) // displace
        {
          RejectDisplace( currCellIdx, currBinRowIdx, currBinColIdx,
           newBinRowIdx, newBinColIdx, arrOfBins, circuit.placement );
        } 
        else // exchange
        {
          RejectExchange( currCellIdx, newCellIdx, currBinRowIdx, currBinColIdx,
           newBinRowIdx, newBinColIdx, arrOfBins, circuit.placement );
        }
        continue;
      }
      /*if (temperIterCounter > 8) 
        if (CheckRowLength(circuit, availableRowLengths[855], 855))
          cout << currBinRowIdx << "\t" << newBinRowIdx;*/
      // trial state(new neighbour) was successfully generated
      genCounter ++;
      nodesIdx[0] = currCellIdx;
      if (newCellIdx == -1)  // displace
      {
        numOfChangedNodes = 1;
      }
      else  // exchange
      {
        numOfChangedNodes = 2;
        nodesIdx[1] = newCellIdx;
      }
      // we count new value of wire length but don't update lengths of circuit.nets
      newWL = cf_recalc_some_nodes(0, circuit.nNets, circuit.nets,
        currWL, nodesIdx, numOfChangedNodes, circuit.tableOfConnections, circuit.placement);
      delta = newWL - currWL; 
      if ((delta < 0) ||(prob4[idxOfRandNum] < exp(-delta * costFactor / T)))  //if SA acceptance criteria is met
      //we accept trial state, it becomes new current state
      {
        acceptCounter ++;
        // we recount wirelength and update lengths of circuit.nets
        currWL = cf_recalc_some_nodes(1, circuit.nNets, circuit.nets,
          currWL, nodesIdx,numOfChangedNodes, circuit.tableOfConnections, circuit.placement); 
        // we update contents of bins, i.e. bin.circuit.nodes
        if (newCellIdx == -1) // displace
        {
          arrOfBins[newBinRowIdx][newBinColIdx]->nodes.push_back(currCellIdx);
          newCellIdxInBin = static_cast<int>(arrOfBins[newBinRowIdx][newBinColIdx]->nodes.size()) - 1;
          arrOfBins[currBinRowIdx][currBinColIdx]->nodes[currCellIdxInBin] = 
            arrOfBins[currBinRowIdx][currBinColIdx]->nodes.back();
          arrOfBins[currBinRowIdx][currBinColIdx]->nodes.pop_back();
          displaceCounter++;
        }
        else // exchange
        {
          arrOfBins[newBinRowIdx][newBinColIdx]->nodes[newCellIdxInBin] = currCellIdx;
          arrOfBins[currBinRowIdx][currBinColIdx]->nodes[currCellIdxInBin] = newCellIdx;
        }
      }
      else 
      //we reject trial state and stay in current state
      {
        if (newCellIdx == -1) // displace
        {
          RejectDisplace(currCellIdx, currBinRowIdx, currBinColIdx,
           newBinRowIdx, newBinColIdx, arrOfBins, circuit.placement);
        } 
        else  // exchange
        {
          RejectExchange(currCellIdx, newCellIdx, currBinRowIdx, currBinColIdx,
           newBinRowIdx, newBinColIdx, arrOfBins, circuit.placement);
        }
      }
      if (acceptCounter % frequencyOfBWA == 0)
      {
        // we recalculate coordinates of bins' centers
        // bin's width correlates with sum length of cells in this bin
        AdjustBinWidths(circuit, rowLengths);
        // update currWL and lengths of circuit.nets
        currWL = cf_recalc_all(1, circuit.nNets, circuit.nets, circuit.placement); 
      }
    } // for (int i = 0; i < numOfIterAtConstTemp; ++i)

    AP = static_cast<double>(acceptCounter)/genCounter;
    if (temperIterCounter<= numOfWarmUpIterations)
    {
       costFactor *= exp(2 * (AP - DESIRED_AP));
    }
    
    cout << "\n" << temperIterCounter;
    cout << "\t" << T;
    cout << "\t" << triesCounter;
    cout << "\t" << genCounter;
    cout << "\t" << acceptCounter;
    cout << "\t" << AP;
    cout << "\t" << currWL;
    cout << "\t" << (1 - currWL/wlBefore) * 100 << "%";
    cout << "\t\t" << static_cast<double>(displaceCounter)/acceptCounter;
    
    wlAfter = currWL;
    // check stopping criteria
    if (T < TLevelStartImprControl)
    {
      sumImpr += ((wlBefore - wlAfter)/wlBefore) * 100;
      if (TLevelCounter%numOfTLevels == 0)
      {
        if (sumImpr < imprThreshold)
        {
          break; // temperature loop stops
        }
        sumImpr = 0;
      }
    }
    // we lower the temperature according to the schedule
    if (temperIterCounter> numOfWarmUpIterations)
    {
      if ((AP<0.1)&&(flagForDumpingFactor == 0))
      {
        dumpingFactor -= 0.025;
        flagForDumpingFactor = 1;
      }
      if ((AP<0.01)&&(flagForDumpingFactor == 1))
      {
        dumpingFactor -= 0.025;
        flagForDumpingFactor = 2;
      }
      T = dumpingFactor * T;
    }
  } // while(T >= lowerTemp)

  // we recalculate coordinates of bins' centers
  // after the last temperature iteration
  //AdjustBinWidths(circuit, rowLengths );
  // update currWL and lengths of circuit.nets
  currWL = cf_recalc_all( 1, circuit.nNets, circuit.nets, circuit.placement);
  
  finish = clock();       // stop timer
  statistics.cellAnnealingWT = (double)(finish - start)/CLOCKS_PER_SEC;         
  relativeImprovement = ((initWL - currWL)/initWL)*100;
  
  cout << "\nWire length before simulated-annealing:\t " << initWL;
  cout << "\nWire length after simulated-annealing:\t "  << currWL;
  cout << "\nRelative improvement %:\t" << relativeImprovement;
  cout << "\nDuration of simulated annealing phase:\t" << statistics.cellAnnealingWT << "\n";
  
  for (rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
  {
    if (availableRowLengths[rowIdx] < rowLengths[rowIdx])
    {
      cout << rowIdx << "\t" << availableRowLengths[rowIdx] << "\t" << rowLengths[rowIdx] << "\t>\n";
    }
  }
  
  delete [] randCurrBinRow;
  delete [] randCurrBinCol;
  delete [] randNewBinRow;
  delete []  randNewBinCol;
  delete []  randCurrIdxInBin;
  delete []  randNewIdxInBin;
  delete []  horOffset;
  delete []  vertOffset;
  delete []  prob3;
  delete []  prob4;
  delete []  rowLengths;
  delete []  desiredRowLength;
  delete []  availableRowLengths;

  return 0;
}
