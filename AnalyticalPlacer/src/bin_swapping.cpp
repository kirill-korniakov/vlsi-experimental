/* bin_swapping.cpp 
 * this is a part of itlDragon
 * Copyright (C) 2005, ITLab, Aleksey Bader, Kirill Kornyakov, Svetlana Gagarinova, Nina Kurina
 * email: bader@newmail.ru
 * email: kirillkornyakov@yandex.ru
 * email: svetlana.gagarinova@cs.vmk.unn.ru
 * email: nina.kurina@gmail.com
 * last modified 29 September, 2006
 */

#include "..\include\bin_swapping.h"
#include "..\include\global_placement.h" //for merge two bins
#include <math.h>    //for exp for simmulated annealing
#include <time.h>
#include <iostream>   // for output of additional information
using namespace std;

const double DFLT_INIT_TEMP = 100;
const double DFLT_LOWER_TEMP = 0.01; 
const double DFLT_DUMPING_FACTOR  = 0.95;
const int    DFLT_ATT = 1;

// default parameters for wl improvement control
const double DFLT_IMPR_THRESHOLD = 0.01;
const int    DFLT_NUM_OF_TLEVELS = 5;
const double DFLT_TLEVEL_START_IMPR_CONTROL = 10;

const double DESIRED_AP = 0.3;
const double INIT_COST_FACTOR = 100;

const float  DFLT_EXCHANGE_PROB = 1.0f;

extern __int64 rngBSTotalTime;
extern __int64 rdtsc();

/// bin-swapping method
/// it is used for improving current circuit.placement by swapping bins
/// we use simulated annealing as optimization method
/// \param statistics.currentWL current wirelength
/// \param numOfRows number of rows in the bin grid
/// \param numOfCols number of columns in the bin grid
/// \param circ bin grid
/// \param circuit.nNets number of circuit.nets in circuit
/// \param circuit.nets array of circuit.nets
/// \param circuit.placement current circuit.placement
/// \param tableOfConnections list of circuit.nets for each node
/// \return new wirelength

double BinSwapping(Circuit& circuit, Statistics& statistics, double* paramsOfBS)
{
  double initTemp = DFLT_INIT_TEMP;
  double lowerTemp = DFLT_LOWER_TEMP;
  double dumpingFactor = DFLT_DUMPING_FACTOR;
  int ATT = DFLT_ATT;

  if(paramsOfBS[0]!= -1) initTemp = paramsOfBS[0];
  if(paramsOfBS[1]!= -1) lowerTemp = paramsOfBS[1];
  if(paramsOfBS[2]!= -1) dumpingFactor = paramsOfBS[2];
  if(paramsOfBS[3]!= -1) ATT = static_cast<int>(paramsOfBS[3]);

  double imprThreshold = DFLT_IMPR_THRESHOLD;
  int numOfTLevels = DFLT_NUM_OF_TLEVELS;
  double TLevelStartImprControl = DFLT_TLEVEL_START_IMPR_CONTROL;
  int numOfBins = circuit.nBinRows * circuit.nBinCols;
  if ( numOfBins >= 16 )
  {
    ATT = 10;
  }
  if ( numOfBins <= 16 )
  {
    TLevelStartImprControl = initTemp / 2;
  }

  double newWL=statistics.currentWL; //trial wirelength
  int currBinRowIdx, currBinColIdx, newBinRowIdx, newBinColIdx;  // indexes of bins
  double oldX, oldY; // old coordinates of first bin
  double tmpX, tmpY; // old coordinates of second bin
  double delta;  // change in wire length after bins exchange 
  double T;  //temperature
  int horRangeLimiter = 5;
  int vertRangeLimiter = static_cast<int>(max(horRangeLimiter * circuit.nBinRows / circuit.nBinCols, 3)); 
 
  int leftOffset = horRangeLimiter/2;
  int rightOffset = horRangeLimiter - leftOffset - 1;
  int downOffset = vertRangeLimiter/2;
  int upOffset = vertRangeLimiter - downOffset - 1;
  int sizeOfCurrBin = 0;        
  int sizeOfNewBin = 0;        
  
  pBin tempBinPointer;

  int triesCounter, genCounter, acceptCounter;
  double AP = 1.0; // SA acceptance probability
 
  // for wl improvement control
  int TLevelCounter = 0;
  double wlBefore;
  double sumImpr = 0;
  
  // random numbers
  int* randCurrBinRow; 
  int* randCurrBinCol;
  float* randNewBinRow; 
  float* randNewBinCol; 
  float* prob1; 
  float* prob2;  
  float* prob3;
  float* prob4;  
  
  __int64 rngStartTime  = 0;
  __int64 rngFinishTime = 0;
  clock_t startTime   = 0;
  clock_t finishTime  = 0;
  startTime  = clock();
  cout << "ATT=" << ATT;
  cout << "\nIter\tT\ttry\tgen\taccept\tAP\tWL" << "\n" << "\n";

  int numOfIterAtConstTemp = circuit.nBinRows * circuit.nBinCols * ATT * 2;
  int idxOfRandNum = -1;

  randCurrBinRow = new int[numOfIterAtConstTemp];
  randCurrBinCol = new int[numOfIterAtConstTemp];
  randNewBinRow = new float[numOfIterAtConstTemp];
  randNewBinCol = new float[numOfIterAtConstTemp];
  prob1 = new float[numOfIterAtConstTemp];
  prob2  = new float[numOfIterAtConstTemp];
  prob3  = new float[numOfIterAtConstTemp];
  prob4  = new float[numOfIterAtConstTemp];

  int numOfWarmUpIterations = 10;
  int temperIterCounter = 0; 
  double costFactor = INIT_COST_FACTOR;

  int flagForDumpingFactor = 0;
  
  //simulated annealing begins
  T = initTemp;
  while (T >= lowerTemp )
  {   
    temperIterCounter ++;
    if ( temperIterCounter < numOfWarmUpIterations + 2 )
    {
      numOfIterAtConstTemp  = numOfBins * ATT;
    }
    if ( temperIterCounter == numOfWarmUpIterations + 2 )
    {
      numOfIterAtConstTemp  = numOfBins * ATT * 2;
    }
    rngStartTime = rdtsc(); 

    GetIntegerRandomNumbers( randCurrBinRow, numOfIterAtConstTemp, 0, circuit.nBinRows);
    GetIntegerRandomNumbers( randCurrBinCol, numOfIterAtConstTemp, 0, circuit.nBinCols);
    GetFloatRandomNumbers( randNewBinRow, numOfIterAtConstTemp, 0.0, 1.0);
    GetFloatRandomNumbers( randNewBinCol, numOfIterAtConstTemp, 0.0, 1.0);
    GetFloatRandomNumbers( prob1, numOfIterAtConstTemp, 0.0, 1.0);
    GetFloatRandomNumbers( prob2, numOfIterAtConstTemp, 0.0, 1.0);
    GetFloatRandomNumbers( prob3, numOfIterAtConstTemp, 0.0, 1.0);
    GetFloatRandomNumbers( prob4, numOfIterAtConstTemp, 0.0, 1.0);

    rngFinishTime = rdtsc();
    rngBSTotalTime += rngFinishTime - rngStartTime; 

    if (T <= TLevelStartImprControl)
    {
      TLevelCounter++;
    }
    wlBefore = cf_recalc_all(0, circuit.nNets, circuit.nets, circuit.placement);

    idxOfRandNum = -1; 
    triesCounter = 0;
    genCounter = 0;
    acceptCounter = 0;

    for (int i = 0; i < numOfIterAtConstTemp; i ++)
    {
      idxOfRandNum ++;
      triesCounter ++;
      genCounter ++;
      currBinRowIdx = randCurrBinRow[idxOfRandNum];
      currBinColIdx = randCurrBinCol[idxOfRandNum];
               
      if ( prob1[idxOfRandNum] < 0.5 ) // go left
        newBinColIdx = max(0, currBinColIdx-(1+static_cast<int>(randNewBinCol[idxOfRandNum]*(leftOffset-1))));
      else // go right
        newBinColIdx = min(circuit.nBinCols - 1, currBinColIdx + 
        static_cast<int>( randNewBinCol[idxOfRandNum]*rightOffset ) );
      if ( prob2[idxOfRandNum] < 0.5 ) // go down
        newBinRowIdx = max(0, currBinRowIdx - (1 + 
        static_cast<int>( randNewBinRow[idxOfRandNum]*(downOffset-1) ) ) );
      else // go up
        newBinRowIdx = min( circuit.nBinRows - 1, currBinRowIdx + 
        static_cast<int>( randNewBinRow[idxOfRandNum]*upOffset ) );

      sizeOfCurrBin = static_cast<int>( circuit.arrOfBins[currBinRowIdx][currBinColIdx]->nodes.size() );
      sizeOfNewBin = static_cast<int>( circuit.arrOfBins[newBinRowIdx][newBinColIdx]->nodes.size() );

      if (((currBinRowIdx != newBinRowIdx)||(currBinColIdx != newBinColIdx ))&& (sizeOfCurrBin && sizeOfNewBin))
      {
        oldX = circuit.placement[circuit.arrOfBins[currBinRowIdx][currBinColIdx]->nodes[0]].xCoord;
        oldY = circuit.placement[circuit.arrOfBins[currBinRowIdx][currBinColIdx]->nodes[0]].yCoord;
        tmpX = circuit.placement[circuit.arrOfBins[newBinRowIdx][newBinColIdx]->nodes[0]].xCoord;
        tmpY = circuit.placement[circuit.arrOfBins[newBinRowIdx][newBinColIdx]->nodes[0]].yCoord;
        
        // virtually swapping bins (change only coordinates of elements)
        for (int j = 0; j < sizeOfCurrBin; j ++ )
        {
          circuit.placement[circuit.arrOfBins[currBinRowIdx][currBinColIdx]->nodes[j]].xCoord = tmpX;
          circuit.placement[circuit.arrOfBins[currBinRowIdx][currBinColIdx]->nodes[j]].yCoord = tmpY;
        }
        if (prob4[idxOfRandNum] < DFLT_EXCHANGE_PROB)
        {
          for (int j = 0; j < sizeOfNewBin; j ++)
          {
            circuit.placement[circuit.arrOfBins[newBinRowIdx][newBinColIdx]->nodes[j]].xCoord = oldX;
            circuit.placement[circuit.arrOfBins[newBinRowIdx][newBinColIdx]->nodes[j]].yCoord = oldY;        
          }
        }
        // recalc wirelength without changing of structure
        newWL = rude_cf_recalc_some_nets( 0, circuit.nets, statistics.currentWL, 
            &circuit.arrOfBins[currBinRowIdx][currBinColIdx]->
            nets.front(), static_cast<int>(circuit.arrOfBins[currBinRowIdx][currBinColIdx]->nets.size()), 
            &circuit.arrOfBins[newBinRowIdx][newBinColIdx]->
            nets.front(), static_cast<int>(circuit.arrOfBins[newBinRowIdx][newBinColIdx]->nets.size()), 
            circuit.placement );

        delta = newWL - statistics.currentWL;
        // is trial swap succeed?
        if ((delta < 0)||(prob3[idxOfRandNum]<exp(-delta * costFactor / T)))
        {
          acceptCounter++;
          if (prob4[idxOfRandNum] < DFLT_EXCHANGE_PROB)
          {
            // change bins
            circuit.arrOfBins[currBinRowIdx][currBinColIdx]->xCoord = tmpX;
            circuit.arrOfBins[currBinRowIdx][currBinColIdx]->yCoord = tmpY;
            circuit.arrOfBins[newBinRowIdx][newBinColIdx]->xCoord   = oldX;
            circuit.arrOfBins[newBinRowIdx][newBinColIdx]->yCoord   = oldY;
            
            tempBinPointer = circuit.arrOfBins[currBinRowIdx][currBinColIdx];
            circuit.arrOfBins[currBinRowIdx][currBinColIdx] = circuit.arrOfBins[newBinRowIdx][newBinColIdx];
            circuit.arrOfBins[newBinRowIdx][newBinColIdx] = tempBinPointer;
            
            // last recalc wirelength  
            statistics.currentWL = rude_cf_recalc_some_nets(1, circuit.nets, statistics.currentWL,
              &circuit.arrOfBins[currBinRowIdx][currBinColIdx]->nets.front(),
              static_cast<int>(circuit.arrOfBins[currBinRowIdx][currBinColIdx]->nets.size()), 
              &circuit.arrOfBins[newBinRowIdx][newBinColIdx]->nets.front(),
              static_cast<int>(circuit.arrOfBins[newBinRowIdx][newBinColIdx]->nets.size()), 
              circuit.placement);
          }
          else
          {
            MoveCluster(circuit.arrOfBins, newBinRowIdx, newBinColIdx, currBinRowIdx, currBinColIdx, circuit);
            // last recalc wirelength  
            statistics.currentWL = rude_cf_recalc_some_nets(1, circuit.nets, statistics.currentWL,
              &circuit.arrOfBins[newBinRowIdx][newBinColIdx]->nets.front(),
              static_cast<int>(circuit.arrOfBins[newBinRowIdx][newBinColIdx]->nets.size()), 
              circuit.placement);
          }
          
        }
        else
        {  
          // rollback
          for (int j = 0; j < sizeOfCurrBin; j ++ )
          {
            circuit.placement[circuit.arrOfBins[currBinRowIdx][currBinColIdx]->nodes[j]].xCoord = oldX;
            circuit.placement[circuit.arrOfBins[currBinRowIdx][currBinColIdx]->nodes[j]].yCoord = oldY;
          }
          if (prob4[idxOfRandNum] < DFLT_EXCHANGE_PROB)
          {
            for(int j = 0; j < sizeOfNewBin; j ++ )
            {
              circuit.placement[circuit.arrOfBins[newBinRowIdx][newBinColIdx]->nodes[j]].xCoord = tmpX;
              circuit.placement[circuit.arrOfBins[newBinRowIdx][newBinColIdx]->nodes[j]].yCoord = tmpY;
            }
          }
        }
      }  
    } // for (int i = 0; i < numOfIterAtConstTemp; i ++ )

    AP = static_cast<double>(acceptCounter)/genCounter;
    
    if (temperIterCounter <= numOfWarmUpIterations)
    {
     costFactor = costFactor*exp((12 - temperIterCounter) * (AP - DESIRED_AP));
    } // if ( temperIterCounter <= numOfWarmUpIterations )
    cout << temperIterCounter;
    cout << "\t" << T;
    cout << "\t" << triesCounter;
    cout << "\t" << genCounter;
    cout << "\t" << acceptCounter;
    cout << "\t" << AP;
    cout << "\t" << statistics.currentWL << "\n";

    // stopping criteria
    if ( T <= TLevelStartImprControl)
    {      
      sumImpr += ((wlBefore - statistics.currentWL)/wlBefore) * 100;
      if ( TLevelCounter%numOfTLevels == 0 )
      {
        //sumImpr /= NUM_OF_TLEVELS;
        if ( sumImpr < imprThreshold )
        {
          break; // temperature loop stops
        }
        sumImpr = 0;
      }
    }   
    if ( temperIterCounter> numOfWarmUpIterations )
    {
      if ((AP<0.1)&&(flagForDumpingFactor == 0))
      {
        dumpingFactor -= 0.025;
        flagForDumpingFactor = 1;
      }
      if ( (AP<0.01)&&(flagForDumpingFactor == 1) )
      {
        dumpingFactor -= 0.025;
        flagForDumpingFactor = 2;
      }
      T = dumpingFactor * T; 
    }
  } // while ( T >= lowerTemp ) 
  
  finishTime = clock();
  statistics.binSwappingWT += (double)(finishTime - startTime)/CLOCKS_PER_SEC;

  delete [] randCurrBinRow;
  delete [] randCurrBinCol;
  delete [] randNewBinRow;
  delete [] randNewBinCol;
  delete [] prob1;
  delete [] prob2;
  delete [] prob3;
  delete [] prob4;

  return statistics.currentWL;
}
