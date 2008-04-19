/*  
 * detailed_placement.cpp
 * this is a part of itlAnalyticalPlacer
 * Copyright (C) 2005, ITLab, Zhivoderov
 * email: zhivoderov.a@gmail.com
 */

#include "..\include\detailed_placement.h"
#include "stdlib.h"
#include <algorithm>
#include <conio.h>

using namespace std;

extern __int64 rngLITotalTime;
extern __int64 rdtsc();

extern str* tableOfNames;
extern Node* terminals;

int HOR_SEARCH_QUANT = 4;
int HOR_SEARCH_QUANT_FACTORIAL;
//double HOR_SEARCH_PROBABILITY = 0.7; //probability of horizontal search

int siteWidth;
int siteHeight;
int numOfSites;
int rowLen;

double* oldXCoord;
double* best_xCoord;
int *nodesIdx;
int** arrOfSites;
RowElement* bufForChange;
RowElement* bufForChange2;

const int DFLT_SIZE_OF_CHAIN = 5; // parameter for GlobalMultiSwap

int DetailedPlacement(Circuit& circuit, Statistics& statistics)
{
  //int pivotVert;     //the row of the base element
  //int pivotHoriz;    //the position of the base element in the row pivotVert
  ////double prob;
  double bestWL;
  double oldWL;
  int rowIdx;
  //double statistics.currentWL;
  double stoppingCriteriaValue;
  double wlDecreaseGS;
  double wlDecreaseVS;
  double wlDecreaseHS;
  double wlBeforeStage;
  clock_t start_time, finish_time;
  clock_t timeGS, timeVS, timeHS, timeStart, timeFinish;
  timeGS = 0;
  timeVS = 0;
  timeHS = 0;
  start_time = clock();
  
  siteWidth  = circuit.rows[0].siteWidth;
  siteHeight = circuit.rows[0].height;
  numOfSites = circuit.rows[circuit.nRows/2].numSites;
  rowLen     = siteWidth * numOfSites;
  //*******************Convert to new structures*******************//
  //int *freeSpaceInRow;       //free space in each row
  int *numOfCellsInRow;        //Quantity of cells in each row
  RowElement **greedy_array;        //the main array of cells

  numOfCellsInRow = new int[circuit.nRows];
  arrOfSites    = new int*[circuit.nRows];
  greedy_array  = new RowElement*[circuit.nRows];
  for (int j = 0; j < circuit.nRows; ++j)
  {
    numOfCellsInRow[j] = 0;
    arrOfSites[j]     = new int[(int)(1.1*numOfSites)];
  }
  for (int k = 0; k < circuit.nNodes; ++k)
  {
    for (int j = circuit.nRows - 1; j >= 0 ; j--)
      if (circuit.rows[j].coordinate == circuit.placement[k].yCoord - 0.5 * circuit.nodes[k].height)
      {
        numOfCellsInRow[j]++;
      }
  }
  for (int i = circuit.nNodes; i < circuit.nNodes + circuit.nTerminals; ++i)
  {
    rowIdx = static_cast<int>((circuit.placement[i].yCoord - 
               0.5 * circuit.terminals[i - circuit.nNodes].height) / siteHeight);
    
    if (rowIdx < 0 || rowIdx >= circuit.nRows) continue;
    
    for (int j = rowIdx; j < rowIdx + circuit.terminals[i - circuit.nNodes].height / siteHeight; ++j)
    {
      numOfCellsInRow[j]++;
    }
  }
  //Forming arrOfSites
  int currSite = 0;
  
  //*******************END OF CONVERTION***********************************//
  
  //**For horizontal search**//
  HOR_SEARCH_QUANT           = static_cast<int>( (gOptions.innerParameters[DP_IDX][1] == -1.0) ? 4 : gOptions.innerParameters[DP_IDX][1] );
  HOR_SEARCH_QUANT_FACTORIAL = Factorial( HOR_SEARCH_QUANT );
  oldXCoord                  = new double[HOR_SEARCH_QUANT];
  best_xCoord                = new double[HOR_SEARCH_QUANT];
  nodesIdx                   = new int[HOR_SEARCH_QUANT];
  bufForChange               = new RowElement[HOR_SEARCH_QUANT],
  bufForChange2              = new RowElement[HOR_SEARCH_QUANT];
  //****//
  
  if (gOptions.innerParameters[DP_IDX][0] == -1.0) gOptions.innerParameters[DP_IDX][0] = 0.001;
  stoppingCriteriaValue = gOptions.innerParameters[DP_IDX][0];
  
  int *randomRow             = new int[circuit.nNodes];
  float *randomRowElement    = new float[circuit.nNodes];
  float *prob                = new float[circuit.nNodes];

  __int64 rngStartTime  = 0;
  __int64 rngFinishTime = 0;
  
  bestWL = oldWL = statistics.currentWL;
  cout << "[\n" ;
  cout << "Initial WL: " << bestWL << "\n";
  cout << "iter #\tWL before\tWL after\tReduction\n";

  double wlBeforeIteration = 0.0;
  double wlAfterIteration  = 0.0;
  wlDecreaseGS = 0.0;
  wlDecreaseVS = 0.0;
  wlDecreaseHS = 0.0;
  wlBeforeStage = 0.0;
  int iterCount = 1;
  do
  {
    rngStartTime = rdtsc();
    
    GetIntegerRandomNumbers( randomRow, circuit.nNodes, 0, circuit.nRows);
    GetFloatRandomNumbers( randomRowElement, circuit.nNodes, 0.0, 1.0);
    GetFloatRandomNumbers( prob, circuit.nNodes, 0.0, 1.0);
    
    rngFinishTime   = rdtsc();
    rngLITotalTime += rngFinishTime - rngStartTime;
    
    wlBeforeIteration = cf_recalc_all( 0, circuit.nNets, circuit.nets, circuit.placement);
    cout << iterCount++ << "\t";
    cout << wlBeforeIteration << "\t";
    wlBeforeStage = wlBeforeIteration;
    
    ///********************* G L O B A L   S W A P *******************///
    timeStart = clock();
    //Initial filling
    for (int i = 0; i < circuit.nRows; ++i)
    {
      for (int j = 0; j < numOfSites; ++j)
      {
        arrOfSites[i][j] = -1;
      }
    }
    int currRow;
    //Filling sites with cell indexes if they are occupied
    for (int i = 0; i < circuit.nNodes; ++i)
    {
      currRow = static_cast<int>(circuit.placement[i].yCoord / siteHeight);
      currSite = static_cast<int>((circuit.placement[i].xCoord - 0.5 * circuit.nodes[i].width) / siteWidth);
      for (int t = currSite; t < currSite + (circuit.nodes[i].width / siteWidth); ++t)
      {
        arrOfSites[currRow][t] = i;
      }
    }
    for (int i = 0; i < circuit.nTerminals; ++i)
    {
      if (circuit.terminals[i].height / siteHeight > 1)
      {
        currRow  = static_cast<int>((circuit.placement[i + circuit.nNodes].yCoord - 
                     0.5 * circuit.terminals[i].height) / siteHeight);
        currSite = static_cast<int>((circuit.placement[i + circuit.nNodes].xCoord - 
                     0.5 * circuit.terminals[i].width) / siteWidth);
                     
        for (int j = currRow; j < currRow + circuit.terminals[i].height / siteHeight; ++j)
        {
          if (j < 0 || j > circuit.nRows) continue;
          for (int siteIdx = currSite; siteIdx < min(currSite + circuit.terminals[i].width / siteWidth, numOfSites);
                siteIdx++)
          {
            if (siteIdx < 0 || siteIdx > numOfSites) continue;
              arrOfSites[j][siteIdx] = -2;    // site is marked as blocked
          }
        }
      }
    }
    //cout << "\nGLOBAL SWAP";
    for (int cellIdx = 0; cellIdx < circuit.nNodes; cellIdx++)
    {
      //cout << "cellIdx = " << cellIdx << endl;
      bestWL = GlobalSwap(cellIdx, numOfCellsInRow, arrOfSites, circuit, statistics);
    }
    timeFinish    = clock();
    timeGS       += timeFinish - timeStart;
    wlDecreaseGS += wlBeforeStage / bestWL - 1;
    wlBeforeStage = bestWL;
    timeStart     = clock();
    //*************************************************************//

    //**************** V E R T I C A L   S E A R C H **************//
    //cout << "\nVERTICAL SEARCH\n";
    for (int i = 0; i < 3; ++i)
      for (int cellIdx = 0; cellIdx < circuit.nNodes; cellIdx++)
      {
        bestWL = VerticalSearch(cellIdx, numOfCellsInRow, arrOfSites, circuit, statistics);
      }
    timeFinish    = clock();
    timeVS       += timeFinish - timeStart;
    wlDecreaseVS += wlBeforeStage / bestWL - 1;
    wlBeforeStage = bestWL;
    timeStart     = clock();
    //*************************************************************//
    //cout << "HORIZONTAL SEARCH" << "\n";
    for (int j = 0; j < circuit.nRows; ++j)
    {
      greedy_array[j] = new RowElement[numOfCellsInRow[j]];
      numOfCellsInRow[j]  = 0;
    }
    for (int k = 0; k < circuit.nNodes; ++k)
    {
      for (int j = circuit.nRows - 1; j >= 0 ; j--)
        if (circuit.rows[j].coordinate == circuit.placement[k].yCoord - 0.5 * circuit.nodes[k].height)
        {
          //Filling greedy_array
          greedy_array[j][numOfCellsInRow[j]].xCoord =
            circuit.placement[k].xCoord;
          //and numOfCellsInRow
          greedy_array[j][numOfCellsInRow[j]++].cellIdx = k;
        }
    }
    for (int i = circuit.nNodes; i < circuit.nNodes + circuit.nTerminals; ++i)
    {
      rowIdx = static_cast<int>((circuit.placement[i].yCoord - 
                 0.5 * circuit.terminals[i - circuit.nNodes].height) / siteHeight);
      
      if (rowIdx < 0 || rowIdx >= circuit.nRows) continue;
      
      for (int j = rowIdx; j < rowIdx + circuit.terminals[i - circuit.nNodes].height / siteHeight; ++j)
      {
        //cout << "i = " << i;
        //Filling greedy_array
        greedy_array[j][numOfCellsInRow[j]].xCoord =
          circuit.placement[i].xCoord;
        //and numOfCellsInRow
        greedy_array[j][numOfCellsInRow[j]++].cellIdx = -1;
      }
    }
        
    for (int j = 0; j < circuit.nRows; ++j)
      if ((numOfCellsInRow[j] != 0) && (numOfCellsInRow[j] != 1))
        QuickSortRowElement(greedy_array[j], numOfCellsInRow[j] - 1);
    //**************************************************************//
    
    //************** H O R I Z O N T A L   S E A R C H *************//
    
    if (iterCount % 2 > 0)
    {
      for (int i = 0; i < circuit.nRows; ++i)
      {
        for (int j = 0; j <= numOfCellsInRow[i] - HOR_SEARCH_QUANT; ++j)
        {
          bestWL = HorizontalSearch(RIGHT, greedy_array, numOfCellsInRow, i, j, circuit, statistics, arrOfSites);
        }
      }
    }
    else
    {
      for (int i = 0; i < circuit.nRows; ++i)
      {
        for (int j = numOfCellsInRow[i] - 1; j >= HOR_SEARCH_QUANT - 1; j--)
        {
          bestWL = HorizontalSearch(LEFT, greedy_array, numOfCellsInRow, i, j, circuit, statistics, arrOfSites);
        }
      }
    }
    timeFinish    = clock();
    timeHS       += timeFinish - timeStart;
    wlDecreaseHS += wlBeforeStage / bestWL - 1;
    timeStart     = clock();
    //**************************************************************//
    
    wlAfterIteration  = cf_recalc_all( 0, circuit.nNets, circuit.nets, circuit.placement);
    
    //cout << "3\n";
    for (int j = 0; j < circuit.nRows; ++j)
    {    
      delete []greedy_array[j];
    }
    //cout << "4\n";    
    cout << wlAfterIteration << "\t";
    cout << (1 - wlAfterIteration / wlBeforeIteration)*100 << "%\n";
    PrintToTmpPL(circuit, statistics);
  } while ((1 - wlAfterIteration / wlBeforeIteration) > stoppingCriteriaValue);
  
  #ifdef _OPT_REG_INFO_
    //Initial filling
    for (int i = 0; i < circuit.nRows; ++i)
    {
      for (int j = 0; j < numOfSites; ++j)
      {
        arrOfSites[i][j] = -1;
      }
    }
    int currRow;
    //Filling sites with unities if they are occupied
    for (int i = 0; i < circuit.nNodes; ++i)
    {
      currRow = static_cast<int>( floor(placement[i].yCoord / siteHeight) );
      currSite = static_cast<int>((placement[i].xCoord - 0.5 * circuit.nodes[i].width) / siteWidth);
      for (int t = currSite; t < currSite + (circuit.nodes[i].width / siteWidth); ++t)
      {
        arrOfSites[currRow][t] = i;
      }
    }
    double xOpt;
    double yOpt;
    int optimalRowIdx;
    int optimalSiteIdx;
    int currSiteIdx;
    int currRowIdx;
    int currCellWidthInSites;
    int varianceCounterX = 0;
    int varianceCounterY = 0;
    int currXOffset;
    int currYOffset;
    int maxXOffset = 0;
    int maxYOffset = 0;
    int numOfGood  = 0;
    const int OPTIMAL_REGION_HALF_WIDTH = 14, OPTIMAL_REGION_HALF_HEIGHT = 2;
    double sumX = 0;
    double sumY = 0;
    for (int i = 0; i < circuit.nNodes; ++i)
    {
      GetOptimalXY(&i, 1,
                placement,
                numOfNets, nets, 
                tableOfConnections,
                NULL, NULL,
                &xOpt, &yOpt,
                -1);
      optimalRowIdx  = static_cast<int>( floor(yOpt / siteHeight) );
      optimalSiteIdx = static_cast<int>( floor(xOpt / siteWidth) );
      currCellWidthInSites = circuit.nodes[i].width / siteWidth;
      
      if (optimalRowIdx  - OPTIMAL_REGION_HALF_HEIGHT < 0)
        optimalRowIdx = 0;
      else 
        if (optimalRowIdx  + OPTIMAL_REGION_HALF_HEIGHT < circuit.nRows)
          optimalRowIdx  -= OPTIMAL_REGION_HALF_HEIGHT;
        else
          optimalRowIdx = circuit.nRows - 2 * OPTIMAL_REGION_HALF_HEIGHT - 2;
      
      if (optimalSiteIdx - OPTIMAL_REGION_HALF_WIDTH < 0)
        optimalSiteIdx = 0;
      else
        if (optimalSiteIdx + OPTIMAL_REGION_HALF_WIDTH < numOfSites)
          optimalSiteIdx -= OPTIMAL_REGION_HALF_WIDTH;
        else
          optimalSiteIdx = numOfSites - 2 * OPTIMAL_REGION_HALF_WIDTH - 2;
            
      currSiteIdx = static_cast<int>( floor((placement[i].xCoord - 0.5 * circuit.nodes[i].width) / siteWidth) );
      currRowIdx  = static_cast<int>( floor(placement[i].yCoord / siteHeight) );
      if (currSiteIdx < optimalSiteIdx || currSiteIdx > optimalSiteIdx + 2 * OPTIMAL_REGION_HALF_WIDTH)
      {
        currXOffset = min( abs(currSiteIdx - optimalSiteIdx - 2 * OPTIMAL_REGION_HALF_WIDTH),
                           abs(currSiteIdx + currCellWidthInSites - optimalSiteIdx) );
        if (maxXOffset < currXOffset) maxXOffset = currXOffset;
        sumX += currXOffset;
        //cout << currXOffset << "\t" << currYOffset << "\n";
        varianceCounterX++;
      }
      if (currRowIdx  < optimalRowIdx  || currRowIdx  > optimalRowIdx  + 2 * OPTIMAL_REGION_HALF_HEIGHT)
      {
        currYOffset = min( abs( currRowIdx  - optimalRowIdx ),
                           abs( currRowIdx  - optimalRowIdx - 2 * OPTIMAL_REGION_HALF_HEIGHT) );
        if (maxYOffset < currYOffset) maxYOffset = currYOffset;
        sumY += currYOffset;
        //cout << currXOffset << "\t" << currYOffset << "\n";
        varianceCounterY++;
      }
      if (currSiteIdx >= optimalSiteIdx && currSiteIdx <= optimalSiteIdx + 2 * OPTIMAL_REGION_HALF_WIDTH &&
          currRowIdx  >= optimalRowIdx  && currRowIdx  <= optimalRowIdx  + 2 * OPTIMAL_REGION_HALF_HEIGHT)
        numOfGood++;
      
    }
    sumX /= varianceCounterX;
    sumY /= varianceCounterY;
    cout << "Maximum x-offset is\t" << maxXOffset << "\n";
    cout << "Maximum y-offset is\t" << maxYOffset << "\n";
    cout << "Average x-offset is\t" << sumX << "\n";
    cout << "Average y-offset is\t" << sumY << "\n";
    cout << "Number of circuit.nodes which are not in their optimal x-regions is\t" << varianceCounterX << "\n";
    cout << "Number of circuit.nodes which are not in their optimal y-regions is\t" << varianceCounterY << "\n";
    cout << "Number of circuit.nodes which are in their optimal regions is\t" << numOfGood << "\n";
  #endif
  
  delete []randomRowElement;
  delete []randomRow;
  delete []prob;
  
  cout << "]\n";
  finish_time = clock();
  statistics.localImprovementWT = (double)(finish_time - start_time)/CLOCKS_PER_SEC;

  cout << "Wire length before detailed placement: "
       << oldWL << "\n";
  cout << "Wire length after  detailed placement: "
       << bestWL << "\n";
  statistics.localImprovementWLI = 100*(1 - bestWL/oldWL);
  cout << "Improvement after  detailed placement: "
       << statistics.localImprovementWLI << "%\n";
  cout << "Time duration of detailed placement phase: "
       << (double)(finish_time - start_time) / CLOCKS_PER_SEC << " sec.\n";
  //final test for possible mistakes(bestWL's value shouldn't change)
  bestWL = cf_recalc_all( 1, circuit.nNets, circuit.nets, circuit.placement);
  cout << bestWL << "\n" << "\n";
  cout << "Time occupied by global swap:        " << (double)timeGS / CLOCKS_PER_SEC << " sec.\n";
  cout << "WL decrease gained by global swap:   " << 100 * wlDecreaseGS << "%\n\n";
  cout << "Time occupied by vertic search:      " << (double)timeVS / CLOCKS_PER_SEC << " sec.\n";
  cout << "WL decrease gained by vertic search: " << 100 * wlDecreaseVS << "%\n\n";
  cout << "Time occupied by horiz search:       " << (double)timeHS / CLOCKS_PER_SEC << " sec.\n";
  cout << "WL decrease gained by horiz search:  " << 100 * wlDecreaseHS << "%\n\n";
  
  //**For hor search**//
  delete []bufForChange;
  delete []bufForChange2;
  delete []oldXCoord;
  delete []best_xCoord;
  delete []nodesIdx;
  //****//
  
  for (int j = 0; j < circuit.nRows; ++j)
  {
    delete []arrOfSites[j];
  }
  delete []arrOfSites;
  delete []greedy_array;
  delete []numOfCellsInRow;
  return OK;
}

double HorizontalSearch(HorSearchDir direction, RowElement** greedy_array, int* numOfCellsInRow, int row, 
        int number, Circuit& circuit, Statistics& statistics, int** arrOfSites)
{
  //cout << "*";
  double leftBorder, rightBorder, spaceAverage = 0.0, space;
  double currWL = 0.0, bestWL = 0.0;
  double currSpace = 0.0;
  double ar[20];             //is used when calculating free space between cells
  static unsigned int permArray[10];
  bool isBetterOrderFound = false;
  for (int i = 0; i < HOR_SEARCH_QUANT; ++i)
  {
    permArray[i] = i;
  }
  int oldNumber = number;
  //choose direction
  switch(direction)
  {
    case RANDOM:
    {
      if ((double) (rand() / RAND_MAX) >= 0.5)
      {                      //right direction
        if (numOfCellsInRow[row] - 1 < (number + HOR_SEARCH_QUANT - 1)) 
        {
          number = numOfCellsInRow[row] - HOR_SEARCH_QUANT;
        }
      }
      else                     //left direction
      {
        if (number - HOR_SEARCH_QUANT + 1 >= 0)
        {
          number = number - HOR_SEARCH_QUANT + 1;     
        }
        else
        {
          number = 0;
        }
      }
      break;
    };
    case LEFT:
    {
      if (number - HOR_SEARCH_QUANT + 1 >= 0)
      {
        number = number - HOR_SEARCH_QUANT + 1;     
      }
      else
      {
        number = 0;
      }
      break;
    };
    case RIGHT:
    {
      if (numOfCellsInRow[row] - 1 < (number + HOR_SEARCH_QUANT - 1)) 
      {
        number = numOfCellsInRow[row] - HOR_SEARCH_QUANT;
      }
    };
  }
  leftBorder  = greedy_array[row][number].xCoord -
        0.5 * circuit.nodes[greedy_array[row][number].cellIdx].width;
  rightBorder = greedy_array[row][number + HOR_SEARCH_QUANT - 1].xCoord + 
        0.5 * circuit.nodes[greedy_array[row][number + HOR_SEARCH_QUANT - 1].cellIdx].width;
//int number is now the number of the left cell in the row for the permutation

  // save old coordinates
  for (int i = 0; i < HOR_SEARCH_QUANT; ++i)
  {
    if (greedy_array[row][number + i].cellIdx == -1) return statistics.currentWL;
    
    oldXCoord[i] = greedy_array[row][number + i].xCoord;
  }
  //for refilling arrOfSites:
  /*int leftSiteIdx  = static_cast<int>((oldXCoord[0] -
            0.5 * circuit.nodes[greedy_array[row][number].cellIdx].width) / siteWidth);
  int rightSiteIdx = static_cast<int>((oldXCoord[HOR_SEARCH_QUANT-1] + 
            0.5 * circuit.nodes[greedy_array[row][number + HOR_SEARCH_QUANT-1].cellIdx].width) / siteWidth - 1);*/
  //calculating free space between cells
  for (int j = 1; j < HOR_SEARCH_QUANT; ++j)
  {
    ar[2*j-1]=(greedy_array[row][number+j].xCoord - 
       0.5 * circuit.nodes[greedy_array[row][number+j].cellIdx].width);
    ar[2*j-2]=(greedy_array[row][number+j-1].xCoord + 
       0.5 * circuit.nodes[greedy_array[row][number+j-1].cellIdx].width);
    currSpace += ar[2*j-1] - ar[2*j-2];
  }
  //spaceAverage = 0.0;///= (HOR_SEARCH_QUANT - 1);

  for (int k = 0; k < HOR_SEARCH_QUANT; ++k)
  {
    nodesIdx[k] = greedy_array[row][number + k].cellIdx;
  }

  bestWL = currWL = statistics.currentWL;
  int cellNumber;
  int avr_numOfSites = (int)(currSpace / siteWidth / (HOR_SEARCH_QUANT - 1));
  int srp_numOfSites = (int)(currSpace / siteWidth) % (HOR_SEARCH_QUANT - 1);
  for (int i = 0; i < HOR_SEARCH_QUANT_FACTORIAL; ++i)    // exhaustive search
  {
    NextPermutation( permArray, HOR_SEARCH_QUANT);
    cellNumber = number + permArray[0];
    circuit.placement[greedy_array[row][cellNumber].cellIdx].xCoord = 
      leftBorder + 0.5 * circuit.nodes[greedy_array[row][cellNumber].cellIdx].width;
    for (int j = 1; j < HOR_SEARCH_QUANT - 1; ++j)
    {
      cellNumber = number + permArray[j-1];
      if (j <= srp_numOfSites)
        space = siteWidth * (avr_numOfSites + 1);
      else
        space = siteWidth * avr_numOfSites;  
      circuit.placement[greedy_array[row][number + permArray[j]].cellIdx ].xCoord = 
          circuit.placement[greedy_array[row][cellNumber].cellIdx].xCoord +
          0.5 * circuit.nodes[greedy_array[row][cellNumber].cellIdx].width +
          space +
          0.5 * circuit.nodes[greedy_array[row][number + permArray[j]].cellIdx].width;
    }
    cellNumber = number + permArray[HOR_SEARCH_QUANT-1];
    circuit.placement[greedy_array[row][cellNumber].cellIdx ].xCoord = rightBorder -
        0.5 * circuit.nodes[greedy_array[row][cellNumber].cellIdx].width;

    currWL = cf_recalc_some_nodes(1, circuit.nNets, circuit.nets, currWL, 
      nodesIdx, HOR_SEARCH_QUANT, circuit.tableOfConnections, circuit.placement);
    if (currWL < bestWL)
    {
      bestWL = currWL;

      for (int j = 0; j < HOR_SEARCH_QUANT; ++j)
      {
        best_xCoord[j] = circuit.placement[ greedy_array[row][number + j].cellIdx ].xCoord;
      }

      isBetterOrderFound = true;
    }
  }

  if (isBetterOrderFound == true)
  {
    //changing circuit.placement:
    for (int j = 0; j < HOR_SEARCH_QUANT; ++j)
    {
      circuit.placement[ greedy_array[row][number + j].cellIdx ].xCoord = best_xCoord[j];
      bufForChange[j].cellIdx = j;
      bufForChange[j].xCoord  = best_xCoord[j];
    }
    //changing greedy_array:
    InsertSortRowElement(bufForChange, HOR_SEARCH_QUANT);
    for (int j = 0; j < HOR_SEARCH_QUANT; ++j)
    {
      bufForChange2[j].cellIdx =
        greedy_array[row][number + bufForChange[j].cellIdx].cellIdx;
      bufForChange2[j].xCoord  = bufForChange[j].xCoord;
    }
    for (int j = 0; j < HOR_SEARCH_QUANT; ++j)
    {
      greedy_array[row][number + j].cellIdx = bufForChange2[j].cellIdx;
      greedy_array[row][number + j].xCoord  = bufForChange2[j].xCoord;
    }
    /*if (circuit.placement[greedy_array[row][number].cellIdx].xCoord - 0.5 * circuit.nodes[greedy_array[row][number].cellIdx].width >= numOfSites*siteWidth )
    {
      cout << "BUG" << "\n";
      getch();
    }*/
    //int currSite;
    //int currIdx;
    //for (int j = leftSiteIdx; j <= rightSiteIdx; ++j)
    //  arrOfSites[row][j] = -1;
    ////Filling sites with unities if they are occupied
    //for (int i = 0; i < HOR_SEARCH_QUANT; ++i)
    //{
    //  currIdx  = greedy_array[row][number + i].cellIdx;
    //  currSite = static_cast<int>((circuit.placement[currIdx].xCoord - 0.5 * circuit.nodes[currIdx].width) / siteWidth);
    //  for (int t = currSite; t < currSite + (circuit.nodes[currIdx].width / siteWidth); ++t)
    //  {
    //    arrOfSites[row][t] = currIdx;
    //  }
    //}
  }else
  {
    for (int i = 0; i < HOR_SEARCH_QUANT; ++i)
      circuit.placement[ greedy_array[row][number + i].cellIdx ].xCoord = oldXCoord[i];
  }
  currWL = cf_recalc_some_nodes( 1, circuit.nNets, circuit.nets, currWL, 
      nodesIdx, HOR_SEARCH_QUANT, circuit.tableOfConnections, circuit.placement);
  statistics.currentWL = bestWL;
  return bestWL;
}

double VerticalSearch(int currCellIdx, int* numOfCellsInRow, int** arrOfSites, Circuit& circuit, Statistics& statistics)
{
  int j, t, k;
  //int numOfConnectedNets;  //number of nets which current cell belongs to
  int currCellWidthInSites = circuit.nodes[currCellIdx].width / siteWidth;
  const int zoneSize = 7;
  const int ROW_NUMBER_IN_VERT_SEARCH = 6;
  double yOpt;
  double oldWL = statistics.currentWL;
  bool   isNextIteration = false;
  int    leftSiteOfSearchZone;
  int    rightSiteOfSearchZone;
  int    destination = 0;
  int    newRowIdx;
  int    oldNewRowIdx;
  int    currRow;
  int    currSite;  //the left site of the current cell
  yOpt = GetOptimalY(&currCellIdx, 1,
              circuit.placement,
              circuit.nNets, circuit.nets, 
              circuit.tableOfConnections,
              NULL, -1);
  
  //      1.3 IF current y-coordinate is equal to the median
  //          continue;
  currRow = static_cast<int>(floor(circuit.placement[currCellIdx].yCoord/siteHeight));
  if (floor(yOpt/siteHeight) == currRow) return statistics.currentWL;
  //      1.4 IF current y-coordinate is less than the median
  //          choose the next row
  //          ELSE choose the previous row
  if (floor(yOpt/siteHeight) <  currRow) destination = -1;
  else destination = 1;
  //      1.5 Assign newRowIdx the index of this row
  oldNewRowIdx = -1;
  for (int rowLevelCounter = 1; rowLevelCounter <= ROW_NUMBER_IN_VERT_SEARCH; rowLevelCounter++)
  {
    isNextIteration = false;
    newRowIdx = currRow + destination * rowLevelCounter;
    if (newRowIdx == circuit.nRows)    newRowIdx--;
    if (newRowIdx < 0)             newRowIdx = 0;
    if (newRowIdx == currRow)      return statistics.currentWL;
    if (newRowIdx == oldNewRowIdx) return statistics.currentWL;
    oldNewRowIdx = newRowIdx;
    //  2 Define the search zone:
    //      currCellWidthInSites to the left and to the right of current cell
    
    currSite = static_cast<int>( floor((circuit.placement[currCellIdx].xCoord - 0.5 * circuit.nodes[currCellIdx].width) / siteWidth) );
    
    leftSiteOfSearchZone  = currSite - ((zoneSize - 1) / 2) * currCellWidthInSites;  
    if (leftSiteOfSearchZone < 0) leftSiteOfSearchZone = 0;
    rightSiteOfSearchZone = currSite + (zoneSize - 1) * currCellWidthInSites - 1;
    if (rightSiteOfSearchZone >= numOfSites)
    {
      rightSiteOfSearchZone = numOfSites - 1;
    }
    //3 Try to put current cell into the new row
    int rowFreeSpace = 0;  //entire free space in new row
    int maxFreeSpace = 0;  //length of maximum continual free space region
    int maxFreeSpace_siteIdx = 0; //index of site from which starts maximum continual 
                    //free space region
    int currFreeSpace = 0;
    double benefit = 0.0;
    double oldX;
    double oldY;
    //bool isBetterPositionFound = false;
    //3.1 Calculate free space inside the search zone
    for (j = leftSiteOfSearchZone; j < rightSiteOfSearchZone; ++j)
    {    
      //if the current site is free
      //cout << newRowIdx << "\t" << j << "\n";
      if (arrOfSites[newRowIdx][j] == -1) 
      {
        rowFreeSpace++;
        currFreeSpace++;
      }
      else
      {
        if (currFreeSpace > maxFreeSpace)
        {
          maxFreeSpace  = currFreeSpace;
          maxFreeSpace_siteIdx = j - currFreeSpace;
        }
        currFreeSpace = 0;
      }
    }
    if (currFreeSpace > maxFreeSpace) //this may happen when last site in new row is free
    {
      maxFreeSpace  = currFreeSpace;
      maxFreeSpace_siteIdx = rightSiteOfSearchZone - currFreeSpace;
    }
       
    //if there is enough space, place cell currCellIdx there and BREAK
    if (rowFreeSpace >= (currCellWidthInSites))
    {
      //3.1.1 If there is enough space in a continual free space region
      if (maxFreeSpace >= (currCellWidthInSites)) 
      {
        //3.1.1.1 Place current cell there and look at the benefit
        oldX = circuit.placement[currCellIdx].xCoord;
        oldY = circuit.placement[currCellIdx].yCoord;
        circuit.placement[currCellIdx].xCoord = maxFreeSpace_siteIdx * siteWidth + 0.5 * circuit.nodes[currCellIdx].width;
        circuit.placement[currCellIdx].yCoord = (newRowIdx + 0.5 ) * siteHeight;
        benefit = oldWL - cf_recalc_some_nodes( 0, circuit.nNets, circuit.nets, statistics.currentWL, 
                  &currCellIdx, 1, circuit.tableOfConnections, circuit.placement);
        //3.1.1.2 IF benefit>0 apply changes and BREAK
        if (benefit > -1.0) 
        { 
          statistics.currentWL = cf_recalc_some_nodes( 1, circuit.nNets, circuit.nets, statistics.currentWL, 
                  &currCellIdx, 1, circuit.tableOfConnections, circuit.placement);
          /*change arrOfSites*/
          for (t = maxFreeSpace_siteIdx; t < maxFreeSpace_siteIdx + currCellWidthInSites; ++t)
          {
            arrOfSites[newRowIdx][t] = currCellIdx;
          }
          for (t = currSite;
             t < currSite + currCellWidthInSites; ++t)
          {
            arrOfSites[currRow][t] = -1;
          }
          numOfCellsInRow[newRowIdx]++;
          numOfCellsInRow[currRow]--;
          /*isBetterPositionFound = true;
          break;*/
          return statistics.currentWL;
        }
        //ELSE perform rollback
        else
        {
          circuit.placement[currCellIdx].xCoord = oldX;
          circuit.placement[currCellIdx].yCoord = oldY;
        }
      }
      //3.1.2 If there is still enough space but the region is not continual
      else
      {
        int currIdx;
        int* allPossibleIdx;  //cells which will be possibly shifted
        int shift;        //value of shift (how much space does not suffice in the first free space region)
        int numOfCells = 0;   //number of cells which will be possibly shifted
        int firstFreeSiteIdx;
        int numOfShiftedCells;
        double* oldXCoords;   //for rollback
        currFreeSpace = 0;
        t = leftSiteOfSearchZone;
        //find the first free site from the left
        while (arrOfSites[newRowIdx][t] != -1) ++t;
        
        firstFreeSiteIdx = t;
        //find the next cell after the first free region from the left
        for (t; t <= rightSiteOfSearchZone; ++t)
        {
          if (arrOfSites[newRowIdx][t] != -2) 
          {
            isNextIteration = true;
            break;
          }
          if (arrOfSites[newRowIdx][t] == -1) 
          {
            currFreeSpace++;
            continue;
          }
          break;
        }
        if (isNextIteration == true) continue;
        
        //fill allPossibleIdx[] in
        //numOfCells will become the number of cells which will be shifted for sure
        allPossibleIdx = new int[zoneSize * currCellWidthInSites];
        for (k = 0; k < zoneSize * currCellWidthInSites; ++k)
          allPossibleIdx[k] = -1;
        for (k = t; k <= rightSiteOfSearchZone; ++k)
        {
          if (arrOfSites[newRowIdx][k] == -2) 
          {
            isNextIteration = true;
            break;
          }
          if (arrOfSites[newRowIdx][k] != -1) 
          {
            if (numOfCells == 0 || allPossibleIdx[numOfCells - 1] != arrOfSites[newRowIdx][k])
              allPossibleIdx[numOfCells++] = arrOfSites[newRowIdx][k];
          }
        }
        if (isNextIteration == true)
        {
          delete []allPossibleIdx;
          continue;
        }
        oldXCoords = new double[numOfCells];
        
        shift = currCellWidthInSites - currFreeSpace;
        oldXCoords[0] = circuit.placement[allPossibleIdx[0]].xCoord;
        //3.1.2.1 Perform shift
        circuit.placement[allPossibleIdx[0]].xCoord += shift * siteWidth;
        k = 1;
        int cellWidth1, cellWidth2;
        //perform shift while there are overlaps
        while (allPossibleIdx[k] != -1)
        {
          currIdx = allPossibleIdx[k];
          cellWidth1 = circuit.nodes[currIdx].width;
          cellWidth2 = circuit.nodes[allPossibleIdx[k - 1]].width;
          if (circuit.placement[currIdx].xCoord - circuit.placement[allPossibleIdx[k - 1]].xCoord -
                0.5 * cellWidth1 - 0.5 * cellWidth2 < 0)
          {
            oldXCoords[k] = circuit.placement[currIdx].xCoord;
            circuit.placement[currIdx].xCoord = circuit.placement[allPossibleIdx[k - 1]].xCoord +
                0.5 * cellWidth1 + 0.5 * cellWidth2;
          }
          else
          {
            break;
          }
          ++k;
        }
        numOfShiftedCells = k;
        //3.1.2.2 Place current cell into the released region and look at the benefit
        oldX = circuit.placement[currCellIdx].xCoord;
        oldY = circuit.placement[currCellIdx].yCoord;
        circuit.placement[currCellIdx].xCoord = firstFreeSiteIdx * siteWidth + 0.5 * circuit.nodes[currCellIdx].width;
        circuit.placement[currCellIdx].yCoord = circuit.placement[allPossibleIdx[0]].yCoord;
        allPossibleIdx[numOfCells] = currCellIdx;   //for  cf_recalc_some_nodes(...)
        benefit = oldWL - cf_recalc_some_nodes( 0, circuit.nNets, circuit.nets, statistics.currentWL,
                  allPossibleIdx, numOfCells + 1, circuit.tableOfConnections, circuit.placement);
        //3.1.2.3 IF benefit>0 apply changes and BREAK
        if (benefit > -1.0) 
        {   
          statistics.currentWL = cf_recalc_some_nodes( 1, circuit.nNets, circuit.nets, statistics.currentWL,
                  allPossibleIdx, numOfCells + 1, circuit.tableOfConnections, circuit.placement);
          /*change arrOfSites*/
          for (t = firstFreeSiteIdx; t < firstFreeSiteIdx + currCellWidthInSites; ++t)
          {
            arrOfSites[newRowIdx][t] = currCellIdx;
          }
          int currSite2;
          for (t = 0; t < numOfCells; ++t)
          {
            currIdx = allPossibleIdx[t];
            currSite2 = static_cast<int>((circuit.placement[currIdx].xCoord - 0.5 * circuit.nodes[currIdx].width) / siteWidth);
            for (k = currSite2; k < currSite2 + (circuit.nodes[currIdx].width / siteWidth); ++k)
            {
              arrOfSites[newRowIdx][k] = currIdx;
            }
          }
          for (t = currSite;t < currSite + currCellWidthInSites; ++t)
          {
            arrOfSites[currRow][t] = -1;
          }
          numOfCellsInRow[newRowIdx]++;
          numOfCellsInRow[currRow]--;
          /*isBetterPositionFound = true;
          break;*/
          delete []oldXCoords;
          delete []allPossibleIdx;
          return statistics.currentWL;
        }
        //ELSE perform rollback
        else
        {
          allPossibleIdx[numOfCells] = -1;
          numOfCells = 0;
          while (numOfCells < numOfShiftedCells)
          {
            currIdx = allPossibleIdx[numOfCells];
            circuit.placement[currIdx].xCoord = oldXCoords[numOfCells];
            numOfCells++;
          }
          circuit.placement[currCellIdx].xCoord = oldX;
          circuit.placement[currCellIdx].yCoord = oldY;
        }
        delete []oldXCoords;
        delete []allPossibleIdx;
      }
    }
     
    int leftFreeSpace  = 0;
    int rightFreeSpace = 0;
    int leftTrialFreeSpace  = 0;
    int rightTrialFreeSpace = 0;
    int trialCellIdx;
    int trialCellWidthInSites;
    int trialNumOfAvailableSites;
    int currNumOfAvailableSites;
    double oldTrialX;
    double oldTrialY;
    int nodesIdx[2];    //indexes of exchanged cells
    nodesIdx[0] = currCellIdx;
    //2.2.1 Calculate currNumOfAvailableSites - sum of free sites on the left and on the right 
    //of current cell + its width(currCellWidthInSites)
    //int currRow;
    //int currSite;
    //int trialRow;
    int trialSite;
    currRow  = static_cast<int>( floor(circuit.placement[currCellIdx].yCoord / siteHeight) );
    currSite = static_cast<int>( floor((circuit.placement[currCellIdx].xCoord - 0.5 * circuit.nodes[currCellIdx].width) / siteWidth) );
    
    j = currSite - 1;
    while (j >= 0 && arrOfSites[currRow][j] == -1) { leftFreeSpace++; j--; }
    
    j = currSite + currCellWidthInSites;
    while (j < numOfSites && arrOfSites[currRow][j] == -1) { rightFreeSpace++; ++j; }
    currNumOfAvailableSites = leftFreeSpace + currCellWidthInSites + rightFreeSpace;
    
    //FOR each cell from the search zone
    benefit = 0.0;
    for (j = leftSiteOfSearchZone; j < rightSiteOfSearchZone; ++j)
    {
      if (arrOfSites[newRowIdx][j] == -2) break;
      //  3.2 Try to exchange current cell with a cell from the search zone
      if (arrOfSites[newRowIdx][j] != -1) 
      {
        leftTrialFreeSpace  = 0;
        rightTrialFreeSpace = 0;
        trialCellIdx = arrOfSites[newRowIdx][j];
        trialCellWidthInSites = circuit.nodes[trialCellIdx].width / siteWidth;
        nodesIdx[1] = trialCellIdx;
        trialSite = static_cast<int>( floor((circuit.placement[trialCellIdx].xCoord - 0.5 * circuit.nodes[trialCellIdx].width) / siteWidth) );
        
        j = trialSite + circuit.nodes[trialCellIdx].width;    //we shall not watch this trial cell at the next cycle pass
        
        if (circuit.placement[trialCellIdx].xCoord - 0.5 * circuit.nodes[trialCellIdx].width < leftSiteOfSearchZone  * siteWidth ||
            circuit.placement[trialCellIdx].xCoord + 0.5 * circuit.nodes[trialCellIdx].width > rightSiteOfSearchZone * siteWidth)
            continue;
        //  3.2.1 Calculate trialNumOfAvailableSites - sum of free sites on the left and on the right 
        //        of the trial cell + its width(trialCellWidthInSites)
        //        Note: If the trial cell doesn't fully belong to the search zone
        //              we look for another trial cell
        k = trialSite - 1;
        while (k >= leftSiteOfSearchZone && arrOfSites[newRowIdx][k] == -1) { leftTrialFreeSpace++; k--; }
        
        k = trialSite + trialCellWidthInSites;
        while (k <= rightSiteOfSearchZone && arrOfSites[newRowIdx][k] == -1) 
        { rightTrialFreeSpace++; ++k; }
        
        trialNumOfAvailableSites = leftTrialFreeSpace + trialCellWidthInSites + rightTrialFreeSpace;
        // 3.2.3 Check on the possibility of exchange and perform it if 
        //       the transformation is possible (Note: we don't make shift)
        if (trialNumOfAvailableSites >= currCellWidthInSites && currNumOfAvailableSites >= trialCellWidthInSites)
        {
          // 3.2.3.1 save old coordinates of exchanging cells
          oldX = circuit.placement[currCellIdx].xCoord;
          oldY = circuit.placement[currCellIdx].yCoord;
          oldTrialX = circuit.placement[trialCellIdx].xCoord;
          oldTrialY = circuit.placement[trialCellIdx].yCoord;
          // 3.2.3.2 perform exchange (so far change only circuit.placement structure)
          // IF there is not enough space to the right of each cell
          //    we place exchanging cell right before next cell
          // Note: currSite and trialSite will become the left sites of newly placed cells
          if (currCellWidthInSites > trialCellWidthInSites + rightTrialFreeSpace)
          {
            trialSite -= (currCellWidthInSites - (trialCellWidthInSites + rightTrialFreeSpace));
          }
          // ELSE their left borders will replace each other and currSite remains the same
          circuit.placement[currCellIdx].xCoord = ( trialSite + 0.5 * currCellWidthInSites) * siteWidth;
          circuit.placement[currCellIdx].yCoord = oldTrialY;
          
          // IF there is not enough space to the right of each cell
          //    we place exchanging cell right before next cell
          if (trialCellWidthInSites > currCellWidthInSites + rightFreeSpace)
          {
            currSite -= (trialCellWidthInSites - (currCellWidthInSites + rightFreeSpace));
          }
          // ELSE their left borders will replace each other and currSite remains the same
          circuit.placement[trialCellIdx].xCoord = ( currSite + 0.5 * trialCellWidthInSites) * siteWidth;
          circuit.placement[trialCellIdx].yCoord = oldY;            
        
          // 3.2.3.3 Calculate the benefit (difference between oldWL and newWL)
          benefit = oldWL - cf_recalc_some_nodes( 0, circuit.nNets, circuit.nets, statistics.currentWL,
                  nodesIdx, 2, circuit.tableOfConnections, circuit.placement);
          // 3.2.3.4 IF (benefit>0) apply changes and BREAK
          if (benefit > 0.0)
          {
            statistics.currentWL = cf_recalc_some_nodes( 1, circuit.nNets, circuit.nets, statistics.currentWL,
                  nodesIdx, 2, circuit.tableOfConnections, circuit.placement);
            // change arrOfSites
            //   calculate the left sites of the exchanged cells
            //   fill the corresponding number of sites with cell indexes  
            k = static_cast<int>(oldX / siteWidth - 0.5 * currCellWidthInSites);
            for (;k < oldX / siteWidth + 0.5 * currCellWidthInSites; ++k)
            {
              arrOfSites[currRow][k] = -1;
            }
            for (k = currSite; k < currSite + trialCellWidthInSites; ++k)
            {
              arrOfSites[currRow][k] = trialCellIdx;
            }
            
            k = static_cast<int>(oldTrialX / siteWidth - 0.5 * trialCellWidthInSites);
            for (;k < oldTrialX / siteWidth + 0.5 * trialCellWidthInSites; ++k)
            {
              arrOfSites[newRowIdx][k] = -1;
            }
            for (k = trialSite; k < trialSite + currCellWidthInSites; ++k)
            {
              arrOfSites[newRowIdx][k] = currCellIdx;
            }
            
            return statistics.currentWL;
            break;
          }
          //ELSE return cells to their old places
          else
          {
            circuit.placement[currCellIdx].xCoord = oldX;
            circuit.placement[currCellIdx].yCoord = oldY;
            circuit.placement[trialCellIdx].xCoord = oldTrialX;
            circuit.placement[trialCellIdx].yCoord = oldTrialY;
            currSite = static_cast<int>( floor((circuit.placement[currCellIdx].xCoord - 0.5 * circuit.nodes[currCellIdx].width) / siteWidth) );
          }
        }
        //ELSE continue
      }
    }
  }
  return statistics.currentWL;
}

double GlobalSwap(int currCellIdx, int* numOfCellsInRow, int** arrOfSites,
              Circuit& circuit, Statistics& statistics)
{
  int i, j, t, k;
  //int numOfConnectedNets;  //number of circuit.nets which current cell belongs to
  const int OPTIMAL_REGION_HALF_WIDTH = 14, OPTIMAL_REGION_HALF_HEIGHT = 2;
  int currCellWidthInSites = circuit.nodes[currCellIdx].width / siteWidth;
  //quit if the current cell is wider than the optimal region:
  if (currCellWidthInSites > 2 * OPTIMAL_REGION_HALF_WIDTH + 1) return statistics.currentWL;
  double xOpt, yOpt;
  double oldWL = statistics.currentWL;
  int optimalRowIdx, optimalSiteIdx;  //index of the row and index of the
                    //site which define optimal region
                    //(left lower site of the region)
  GetOptimalXY(&currCellIdx, 1,
              circuit.placement,
              circuit.nNets, circuit.nets, 
              circuit.tableOfConnections,
              NULL, NULL,
              &xOpt, &yOpt,
              -1);
  
  //1.3 Find the optimal region borders
  //1.3.1 Find the site which corresponds to numbers from p. 1.2
  optimalRowIdx  = static_cast<int>( floor(yOpt / siteHeight) );
  optimalSiteIdx = static_cast<int>( floor(xOpt / siteWidth) );
  
  //1.3.2 Assign the optimal region to rectangular 5x3 centered in site from p. 1.3.1.
  //The optimal region is completely defined by coordinates of its lower left 
  //corner and its dimensions.
  //optimalRowIdx will now become the index of the lower row of the optimal region
  //optimalSiteIdx will now become the index of the left site of the optimal region
  if (optimalRowIdx  - OPTIMAL_REGION_HALF_HEIGHT < 0)
    optimalRowIdx = 0;
  else 
    if (optimalRowIdx  + OPTIMAL_REGION_HALF_HEIGHT < circuit.nRows)
      optimalRowIdx  -= OPTIMAL_REGION_HALF_HEIGHT;
    else
      optimalRowIdx = circuit.nRows - 2 * OPTIMAL_REGION_HALF_HEIGHT - 2;
  
  if (optimalSiteIdx - OPTIMAL_REGION_HALF_WIDTH < 0)
    optimalSiteIdx = 0;
  else
    if (optimalSiteIdx + OPTIMAL_REGION_HALF_WIDTH < numOfSites)
      optimalSiteIdx -= OPTIMAL_REGION_HALF_WIDTH;
    else
      optimalSiteIdx = numOfSites - 2 * OPTIMAL_REGION_HALF_WIDTH - 2;
        
  //IF current cell already belongs to its optimal region
  int currSiteIdx = static_cast<int>( floor((circuit.placement[currCellIdx].xCoord - 0.5 * circuit.nodes[currCellIdx].width) / siteWidth) );
  int currRowIdx  = static_cast<int>( floor(circuit.placement[currCellIdx].yCoord / siteHeight) );
  if (currSiteIdx >= optimalSiteIdx && currSiteIdx <= optimalSiteIdx + 2 * OPTIMAL_REGION_HALF_WIDTH &&
      currRowIdx  >= optimalRowIdx  && currRowIdx  <= optimalRowIdx  + 2 * OPTIMAL_REGION_HALF_HEIGHT)
  //   do not make any changes
    return statistics.currentWL;
  //2 Try to move cell currCellIdx to its optimal region
  //2.1 try to find free space for cell currCellIdx
  int rowFreeSpace = 0;  //entire free space in current row within the optimal region
  int maxFreeSpace = 0;  //length of maximum continual free space region
  int maxFreeSpace_siteIdx = 0; //index of site from which starts maximum continual 
                                //free space region
  int currFreeSpace = 0;
  double benefit = 0.0;
  double oldX;
  double oldY;
  bool isBetterPositionFound = false;
  bool isNextIteration = false;
  optimalRowIdx += OPTIMAL_REGION_HALF_HEIGHT;
  //FOR each row from optimal region
  for (i = 0; i <= OPTIMAL_REGION_HALF_HEIGHT && i >= -OPTIMAL_REGION_HALF_HEIGHT; i = -i, i>=0 ? i++ : 1)
  {
    rowFreeSpace = 0;
    maxFreeSpace = 0;
    maxFreeSpace_siteIdx = 0;
    currFreeSpace = 0;
    benefit = 0.0;
    isNextIteration = false;
    //2.1.1 calculate free space in optimal region
    for (j = optimalSiteIdx; j < optimalSiteIdx + 2 * OPTIMAL_REGION_HALF_WIDTH + 1; ++j)
    {
      //if the current site is free
      if (arrOfSites[optimalRowIdx + i][j] == -1) 
      {
        rowFreeSpace++;
        currFreeSpace++;
      }
      else
      {
        if (currFreeSpace > maxFreeSpace)
        {
          maxFreeSpace  = currFreeSpace;
          maxFreeSpace_siteIdx = j - currFreeSpace;
        }
        currFreeSpace = 0;
      }
    }
    if (currFreeSpace > maxFreeSpace) //this may happen when last site in current row is free
    {
      maxFreeSpace  = currFreeSpace;
      maxFreeSpace_siteIdx = optimalSiteIdx + 2 * OPTIMAL_REGION_HALF_WIDTH + 1 - currFreeSpace;
    }
    //2.2.2 if there is enough space, place cell currCellIdx there and BREAK
    if (rowFreeSpace >= (currCellWidthInSites))
    {
      //if there is enough space in a continual free space region
      if (maxFreeSpace >= (currCellWidthInSites)) 
      {
        //then place the current cell there and look at the benefit
        oldX = circuit.placement[currCellIdx].xCoord;
        oldY = circuit.placement[currCellIdx].yCoord;
        circuit.placement[currCellIdx].xCoord = maxFreeSpace_siteIdx * siteWidth + 0.5 * circuit.nodes[currCellIdx].width;
        circuit.placement[currCellIdx].yCoord = circuit.rows[optimalRowIdx + i].coordinate + 0.5 * siteHeight;
        benefit = oldWL - cf_recalc_some_nodes( 0, circuit.nNets, circuit.nets, statistics.currentWL, 
                  &currCellIdx, 1, circuit.tableOfConnections, circuit.placement);
        if (benefit > -1.0) 
        {   
          statistics.currentWL = cf_recalc_some_nodes( 1, circuit.nNets, circuit.nets, statistics.currentWL, 
                  &currCellIdx, 1, circuit.tableOfConnections, circuit.placement);
          /*change arrOfSites*/
          for (t = maxFreeSpace_siteIdx; t < maxFreeSpace_siteIdx + currCellWidthInSites; ++t)
          {
            arrOfSites[optimalRowIdx + i][t] = currCellIdx;
          }
          int oldRowIdx  = static_cast<int>( floor(oldY / siteHeight) );
          int oldSiteIdx = static_cast<int>( floor((oldX - 0.5 * circuit.nodes[currCellIdx].width) / siteWidth) );
          for (t = oldSiteIdx;
             t < oldSiteIdx + currCellWidthInSites; ++t)
          {
            arrOfSites[oldRowIdx][t] = -1;
          }
          numOfCellsInRow[optimalRowIdx + i]++;
          numOfCellsInRow[oldRowIdx]--;
          isBetterPositionFound = true;
          return statistics.currentWL;
        }
        else  //rollback
        {
          circuit.placement[currCellIdx].xCoord = oldX;
          circuit.placement[currCellIdx].yCoord = oldY;
        }
      }
      else  //if there is stil enough space but the region is not continual
      {
        int currIdx;
        int* allPossibleIdx;  //cells which will be possibly shifted
        int shift;        //value of shift (how much space does not suffice in the first free space region)
        int numOfCells = 0;   //number of cells which will be possibly shifted
        int firstFreeSiteIdx;
        int numOfShiftedCells;
        double* oldXCoords;   //for rollback
        currFreeSpace = 0;
        t = optimalSiteIdx;
        //find the first free site from the left
        while (arrOfSites[optimalRowIdx + i][t] != -1) ++t;
        
        firstFreeSiteIdx = t;
        //find the next cell after the first free region from the left
        for (t; t < optimalSiteIdx + 2 * OPTIMAL_REGION_HALF_WIDTH + 1; ++t)
        {
          if (arrOfSites[optimalRowIdx + i][t] == -1) 
          {
            currFreeSpace++;
            continue;
          }
          break;
        }
        
        //fill allPossibleIdx[] in
        //numOfCells will become the number of cells which will be shifted for sure
        allPossibleIdx = new int[2 * OPTIMAL_REGION_HALF_WIDTH + 1];
        for (k = 0; k < 2 * OPTIMAL_REGION_HALF_WIDTH + 1; ++k)
          allPossibleIdx[k] = -1;
        for (k = t; k < optimalSiteIdx + 2 * OPTIMAL_REGION_HALF_WIDTH + 1; ++k)
        {
          if (arrOfSites[optimalRowIdx + i][k] == -2) 
          {
            isNextIteration = true;
            break;
          }
          if (arrOfSites[optimalRowIdx + i][k] != -1) 
          {
            if (numOfCells == 0 || allPossibleIdx[numOfCells - 1] != arrOfSites[optimalRowIdx + i][k])
              allPossibleIdx[numOfCells++] = arrOfSites[optimalRowIdx + i][k];
          }
        }
        if (isNextIteration == true)
        {
          delete []allPossibleIdx;
          continue;
        }
        oldXCoords = new double[numOfCells];
        
        shift = currCellWidthInSites - currFreeSpace;
        oldXCoords[0] = circuit.placement[allPossibleIdx[0]].xCoord;
        circuit.placement[allPossibleIdx[0]].xCoord += shift * siteWidth;
        k = 1;
        int cellWidth1, cellWidth2;
        //perform shift while there are overlaps
        while (allPossibleIdx[k] != -1)
        {
          currIdx = allPossibleIdx[k];
          cellWidth1 = circuit.nodes[currIdx].width;
          cellWidth2 = circuit.nodes[allPossibleIdx[k - 1]].width;
          if (circuit.placement[currIdx].xCoord - circuit.placement[allPossibleIdx[k - 1]].xCoord -
                0.5 * cellWidth1 - 0.5 * cellWidth2 < 0)
          {
            oldXCoords[k] = circuit.placement[currIdx].xCoord;
            circuit.placement[currIdx].xCoord = circuit.placement[allPossibleIdx[k - 1]].xCoord +
                0.5 * cellWidth1 + 0.5 * cellWidth2;
          }
          else
          {
            break;
          }
          ++k;
        }
        numOfShiftedCells = k;
        //place the current cell into the released region and look at the benefit
        oldX = circuit.placement[currCellIdx].xCoord;
        oldY = circuit.placement[currCellIdx].yCoord;
        circuit.placement[currCellIdx].xCoord = firstFreeSiteIdx * siteWidth + 0.5 * circuit.nodes[currCellIdx].width;
        circuit.placement[currCellIdx].yCoord = circuit.placement[allPossibleIdx[0]].yCoord;
        allPossibleIdx[numOfCells] = currCellIdx;   //for  cf_recalc_some_nodes(...)
        benefit = oldWL - cf_recalc_some_nodes( 0, circuit.nNets, circuit.nets, statistics.currentWL,
                  allPossibleIdx, numOfCells + 1, circuit.tableOfConnections, circuit.placement);
        if (benefit > -1.0) 
        {   
          statistics.currentWL = cf_recalc_some_nodes( 1, circuit.nNets, circuit.nets, statistics.currentWL,
                  allPossibleIdx, numOfCells + 1, circuit.tableOfConnections, circuit.placement);
          /*change arrOfSites*/
          for (t = firstFreeSiteIdx; t < firstFreeSiteIdx + currCellWidthInSites; ++t)
          {
            arrOfSites[optimalRowIdx + i][t] = currCellIdx;
          }
          int currSite;
          for (t = 0; t < numOfCells; ++t)
          {
            currIdx = allPossibleIdx[t];
            currSite = static_cast<int>((circuit.placement[currIdx].xCoord - 0.5 * circuit.nodes[currIdx].width) / siteWidth);
            for (k = currSite; k < currSite + (circuit.nodes[currIdx].width / siteWidth); ++k)
            {
              arrOfSites[optimalRowIdx + i][k] = currIdx;
            }
          }
          int oldRowIdx  = static_cast<int>( floor(oldY / siteHeight) );
          int oldSiteIdx = static_cast<int>( floor((oldX - 0.5 * circuit.nodes[currCellIdx].width) / siteWidth) );
          for (t = oldSiteIdx;
             t < oldSiteIdx + currCellWidthInSites; ++t)
          {
            arrOfSites[oldRowIdx][t] = -1;
          }
          numOfCellsInRow[optimalRowIdx + i]++;
          numOfCellsInRow[oldRowIdx]--;
          isBetterPositionFound = true;
          delete []oldXCoords;
          delete []allPossibleIdx;
          return statistics.currentWL;
        }
        else   //rollback
        {
          allPossibleIdx[numOfCells] = -1;
          numOfCells = 0;
          while (numOfCells < numOfShiftedCells)
          {
            currIdx = allPossibleIdx[numOfCells];
            circuit.placement[currIdx].xCoord = oldXCoords[numOfCells];
            numOfCells++;
          }
          circuit.placement[currCellIdx].xCoord = oldX;
          circuit.placement[currCellIdx].yCoord = oldY;
        }
        delete []oldXCoords;
        delete []allPossibleIdx;
      }
    }
    if (isBetterPositionFound == true) return statistics.currentWL;
  }
  //2.2 if currCellIdx is not already placed try to exchange it with a cell from the optimal region
  int leftFreeSpace  = 0;
  int rightFreeSpace = 0;
  int leftTrialFreeSpace  = 0;
  int rightTrialFreeSpace = 0;
  int trialCellIdx;
  int trialCellWidthInSites;
  int trialNumOfAvailableSites;
  int currNumOfAvailableSites;
  double oldTrialX;
  double oldTrialY;
  int nodesIdx[2];    //indexes of exchanged cells
  nodesIdx[0] = currCellIdx;
  //2.2.1 Calculate currNumOfAvailableSites - sum of free sites on the left and on the right 
  //of current cell + its width(currCellWidthInSites)
  int currRow;
  int currSite;
  //int trialRow;
  int trialSite;
  currRow  = static_cast<int>( floor(circuit.placement[currCellIdx].yCoord / siteHeight) );
  currSite = static_cast<int>( floor((circuit.placement[currCellIdx].xCoord - 0.5 * circuit.nodes[currCellIdx].width) / siteWidth) );
  
  j = currSite - 1;
  while (j >= 0 && arrOfSites[currRow][j] == -1) { leftFreeSpace++; j--; }
  
  j = currSite + currCellWidthInSites;
  while (j < numOfSites && arrOfSites[currRow][j] == -1) { rightFreeSpace++; ++j; }
  currNumOfAvailableSites = leftFreeSpace + currCellWidthInSites + rightFreeSpace;
  
  optimalRowIdx -= OPTIMAL_REGION_HALF_HEIGHT;
  //FOR each cell from optimal region (further trial cell)
      //2.2.2 Calculate trialNumOfAvailableSites - sum of free sites on the left and on the right 
      //of the trial cell + its width(trialCellWidthInSites)
      // Note: If the trial cell doesn't fully belong to the optimal region
      //       we may try to make exchange but suppose that there is no free
      //       space on the left or on the right of it (as if we enlarged the optimal region)
  for (i = optimalRowIdx; i < optimalRowIdx + 2 * OPTIMAL_REGION_HALF_HEIGHT + 1; ++i)
  {
    benefit = 0.0;
    for (j = optimalSiteIdx; j < optimalSiteIdx + 2 * OPTIMAL_REGION_HALF_WIDTH + 1; ++j)
    {
      if (arrOfSites[i][j] == -2) break;
      //if the current site is not free
      if (arrOfSites[i][j] != -1) 
      {
        leftTrialFreeSpace  = 0;
        rightTrialFreeSpace = 0;
        trialCellIdx = arrOfSites[i][j];
        trialCellWidthInSites = circuit.nodes[trialCellIdx].width / siteWidth;
        nodesIdx[1] = trialCellIdx;
       // trialRow  = static_cast<int>( floor(circuit.placement[trialCellIdx].yCoord / siteHeight) );
        trialSite = static_cast<int>( floor((circuit.placement[trialCellIdx].xCoord - 0.5 * circuit.nodes[trialCellIdx].width) / siteWidth) );
        
        j = trialSite + circuit.nodes[trialCellIdx].width;    //we shall not watch this trial cell at the next cycle pass
        
        k = trialSite - 1;
        while (k >= optimalSiteIdx && arrOfSites[i][k] == -1) { leftTrialFreeSpace++; k--; }
        
        k = trialSite + trialCellWidthInSites;
        while (k <= optimalSiteIdx + 2 * OPTIMAL_REGION_HALF_WIDTH && arrOfSites[i][k] == -1) 
        { rightTrialFreeSpace++; ++k; }
        
        trialNumOfAvailableSites = leftTrialFreeSpace + trialCellWidthInSites + rightTrialFreeSpace;
        //2.2.3 Check on the possibility of exchange and perform it if 
        //      the transformation is possible (Note: we don't make shift)
        if (trialNumOfAvailableSites >= currCellWidthInSites && currNumOfAvailableSites >= trialCellWidthInSites)
        {
          //2.2.3.1 save old coordinates of exchanging cells
          oldX = circuit.placement[currCellIdx].xCoord;
          oldY = circuit.placement[currCellIdx].yCoord;
          oldTrialX = circuit.placement[trialCellIdx].xCoord;
          oldTrialY = circuit.placement[trialCellIdx].yCoord;
          //2.2.3.2 perform exchange (so far change only circuit.placement structure)
          // IF there is not enough space to the right of each cell
          //    we place exchanging cell right before next cell
          // Note: currSite and trialSite will become the left sites of newly placed cells
          if (currCellWidthInSites > trialCellWidthInSites + rightTrialFreeSpace)
          {
            trialSite -= (currCellWidthInSites - (trialCellWidthInSites + rightTrialFreeSpace));
          }
          // ELSE their left borders will replace each other and currSite remains the same
          circuit.placement[currCellIdx].xCoord = ( trialSite + 0.5 * currCellWidthInSites) * siteWidth;
          circuit.placement[currCellIdx].yCoord = oldTrialY;
          
          // IF there is not enough space to the right of each cell
          //    we place exchanging cell right before next cell
          if (trialCellWidthInSites > currCellWidthInSites + rightFreeSpace)
          {
            currSite -= (trialCellWidthInSites - (currCellWidthInSites + rightFreeSpace));
          }
          // ELSE their left borders will replace each other and currSite remains the same
          circuit.placement[trialCellIdx].xCoord = ( currSite + 0.5 * trialCellWidthInSites) * siteWidth;
          circuit.placement[trialCellIdx].yCoord = oldY;            
        
          //2.2.3.3 Calculate the benefit (difference between oldWL and newWL)
          benefit = oldWL - cf_recalc_some_nodes( 0, circuit.nNets, circuit.nets, statistics.currentWL,
                  nodesIdx, 2, circuit.tableOfConnections, circuit.placement);
          //2.2.3.4 IF (benefit>0) apply changes and BREAK
          if (benefit > -1.0)
          {
            statistics.currentWL = cf_recalc_some_nodes( 1, circuit.nNets, circuit.nets, statistics.currentWL,
                  nodesIdx, 2, circuit.tableOfConnections, circuit.placement);
            //         change arrOfSites
            //           fill the corresponding number of sites with cell indexes
            
            /*currRow  = static_cast<int>( floor(circuit.placement[currCellIdx].yCoord / siteHeight) );
            currSite = static_cast<int>( floor((circuit.placement[currCellIdx].xCoord - 0.5 * circuit.nodes[currCellIdx].width) / siteWidth) );*/
            k = static_cast<int>(oldX / siteWidth - 0.5 * currCellWidthInSites);
            for (;k < oldX / siteWidth + 0.5 * currCellWidthInSites; ++k)
            {
              arrOfSites[currRow][k] = -1;
            }
            for (k = currSite; k < currSite + trialCellWidthInSites; ++k)
            {
              arrOfSites[currRow][k] = trialCellIdx;
            }
            
            /*trialRow  = static_cast<int>( floor(circuit.placement[trialCellIdx].yCoord / siteHeight) );
            trialSite = static_cast<int>( floor((circuit.placement[trialCellIdx].xCoord - 0.5 * circuit.nodes[trialCellIdx].width) / siteWidth) );*/
            k = static_cast<int>(oldTrialX / siteWidth - 0.5 * trialCellWidthInSites);
            for (;k < oldTrialX / siteWidth + 0.5 * trialCellWidthInSites; ++k)
            {
              arrOfSites[i][k] = -1;
            }
            for (k = trialSite; k < trialSite + currCellWidthInSites; ++k)
            {
              arrOfSites[i][k] = currCellIdx;
            }
            
            isBetterPositionFound = true;
            return statistics.currentWL;
            //break;
          }
          //        ELSE return cells to their old places
          else
          {
            circuit.placement[currCellIdx].xCoord = oldX;
            circuit.placement[currCellIdx].yCoord = oldY;
            circuit.placement[trialCellIdx].xCoord = oldTrialX;
            circuit.placement[trialCellIdx].yCoord = oldTrialY;
            currSite = static_cast<int>( floor((circuit.placement[currCellIdx].xCoord - 0.5 * circuit.nodes[currCellIdx].width) / siteWidth) );
          }
        }
        //ELSE continue
      }
    }
    /*if (!CheckLegalityOfPlacement( numOfRows, rows, 
                                                 circuit.nNodes, circuit.nodes, 
                                                 circuit.placement, tableOfNames ))
        cout << "\n" << "currCellIdx = " << currCellIdx << "\n";*/
  }
  /*if (!CheckLegalityOfPlacement( numOfRows, rows, 
                                                 circuit.nNodes, circuit.nodes, 
                                                 circuit.placement, tableOfNames ))
        {cout << "\n" << "currCellIdx = " << currCellIdx << "\n"; getch(); }*/
  return statistics.currentWL;
}              

//void SingleSegmentClustering(RowElement** greedy_array, int* numOfCellsInRow,
//        Row* const rows, const int numOfRows, int numOfNodes,
//        Node* nodes, const int numOfNets, Net *const nets, 
//        Place *const placement,
//        vector<int>* tableOfConnections)
//{
//  int i, j, k, t;
//  int currRowElemIdx;
//  int currRowIdx;
//  int currCellIdx;
//  int numOfOldClusters;
//  int numOfNewClusters;
//  bool isAnyOverlap = false;
//  pCluster* oldCluster;
//  pCluster* newCluster;
//  pCluster* delOldCluster;
//  pCluster* delNewCluster;
//  int maxNumOfCellsInRow = 0;
//  int oldVectorSize;
//  vector<double> buf;  //for merging border lists
//  //double sum;
//  double leftBorder;
//  //double xOpt;
//  double x1, x2;
//  // Find maxNumOfCellsInRow
//  for (i = 0; i < numOfRows; ++i)
//  {
//    if (numOfCellsInRow[i] > maxNumOfCellsInRow) maxNumOfCellsInRow = numOfCellsInRow[i];
//  }
//  oldCluster    = new pCluster[maxNumOfCellsInRow];
//  newCluster    = new pCluster[maxNumOfCellsInRow];
//  delOldCluster = new pCluster[maxNumOfCellsInRow];
//  delNewCluster = new pCluster[maxNumOfCellsInRow];
//  for (i = 0; i < maxNumOfCellsInRow; ++i)
//  {
//    delOldCluster[i] = oldCluster[i] = new Cluster;
//    delNewCluster[i] = newCluster[i] = new Cluster;
//  }
//  // FOR each row currRowIdx
//  for (currRowIdx = 0; currRowIdx < 5/*numOfRows*/; currRowIdx++)
//  {
//    cout << "current row idx is " << currRowIdx << "\n";
//    // 1.  Assign oldCluster the cells in current row
//    for (currRowElemIdx = 0; currRowElemIdx < numOfCellsInRow[currRowIdx]; currRowElemIdx++)
//    {
//      oldCluster[currRowElemIdx] = delOldCluster[currRowElemIdx];
//      newCluster[currRowElemIdx] = delNewCluster[currRowElemIdx];
//      
//      oldCluster[currRowElemIdx]->boundsList.clear();
//      oldCluster[currRowElemIdx]->cellIdxs.clear();
//      newCluster[currRowElemIdx]->boundsList.clear();
//      newCluster[currRowElemIdx]->cellIdxs.clear();
//      
//      currCellIdx = greedy_array[currRowIdx][currRowElemIdx].cellIdx;
//      oldCluster[currRowElemIdx]->cellIdxs.push_back( currCellIdx );
//      oldCluster[currRowElemIdx]->width = nodes[currCellIdx].width;
//      
//      //xOpt = GetOptimalX(&currCellIdx, 1,
//      //              placement,
//      //              numOfNets, nets,
//      //              tableOfConnections,
//      //              &oldCluster[currRowElemIdx]->boundsList, -1);
//      x2 = -1000.0;
//      GetOptimalInterval(&currCellIdx, 1,
//                    placement,
//                    numOfNets, nets,
//                    tableOfConnections,
//                    &x1, &x2);
//      if (x2 == -1000.0) x2 = rowLen;
//      if (currRowElemIdx > 0 )
//      {
//        // IF (p(C') + w(C') <= r(C)) (see Brenner)
//        // Do not make overlap if possible
//        //if (oldCluster[currRowElemIdx-1]->leftX + oldCluster[currRowElemIdx-1]->width <= x2)
//        //{
//          oldCluster[currRowElemIdx]->leftX = static_cast<int>(
//                                                  max( oldCluster[currRowElemIdx-1]->leftX +
//                                                       oldCluster[currRowElemIdx-1]->width,
//                                                       floor( x1 / siteWidth )));
//        //}
//        //else
//        //{
//        //  oldCluster[currRowElemIdx]->leftX = static_cast<int>(floor( x1 / siteWidth ));
//        //}
//      }
//      else
//      {
//        oldCluster[currRowElemIdx]->leftX = static_cast<int>(floor( x1 / siteWidth ));
//      }
//      oldCluster[currRowElemIdx]->leftX = max(oldCluster[currRowElemIdx]->leftX, 0);
//      oldCluster[currRowElemIdx]->leftX = min(oldCluster[currRowElemIdx]->leftX,
//                                            static_cast<int>(rowLen - 
//                                            oldCluster[currRowElemIdx]->width));
//      placement[currCellIdx].xCoord = oldCluster[currRowElemIdx]->leftX + oldCluster[currRowElemIdx]->width / 2;
//    }
//    /*if (!CheckLegalityOfPlacement( numOfRows, rows, 
//                                                 numOfNodes, nodes,
//                                                 numOfNets, nets,
//                                                 placement, tableOfNames ))
//        cout << "\n" << "currCellIdx = " << currCellIdx << "\n";*/
//    numOfOldClusters = numOfCellsInRow[currRowIdx] - 1;
//    do
//    {
//      numOfNewClusters = 0;
//      // Initialize the first new cluster
//      newCluster[0] = oldCluster[0];
//      j = 1;
//      // WHILE we haven't seen all oldClusters
//      while (j <= numOfOldClusters)
//      {
//        // IF newCluster[numOfNewClusters] and oldCluster[j] has overlap
//        if ( oldCluster[j]->leftX -
//            (newCluster[numOfNewClusters]->leftX + newCluster[numOfNewClusters]->width) < 0)
//        {
//          // Cluster newCluster[numOfNewClusters] and oldCluster[j] to
//          // form new cluster newCluster[numOfNewClusters]
//          // Add new indexes of cells to newCluster
//          oldVectorSize = static_cast<int>( newCluster[numOfNewClusters]->cellIdxs.size() );
//          newCluster[numOfNewClusters]->cellIdxs.resize( oldVectorSize + 
//                oldCluster[j]->cellIdxs.size());
//          copy(oldCluster[j]->cellIdxs.begin(), oldCluster[j]->cellIdxs.end(), 
//               newCluster[numOfNewClusters]->cellIdxs.begin() + oldVectorSize);
//          // Calculate newCluster's width
//          newCluster[numOfNewClusters]->width += oldCluster[j]->width;
//          x2 = -1000.0;
//          /*GetOptimalInterval2(&newCluster[numOfNewClusters]->cellIdxs,
//                    placement,
//                    numOfNets, nets,
//                    tableOfConnections, NULL, NULL,
//                    &x1, &x2, -1);*/
//          GetOptimalInterval(&newCluster[numOfNewClusters]->cellIdxs,
//                    placement,
//                    numOfNets, nets,
//                    tableOfConnections,
//                    &x1, &x2);
//          if (x2 == -1000.0) x2 = rowLen;
//          if (numOfNewClusters > 0 )
//          {
//            // IF (p(C') + w(C') <= r(C))
//            // Do not make overlap if possible
//            if (newCluster[numOfNewClusters-1]->leftX + newCluster[numOfNewClusters-1]->width <= x2)
//            {
//              newCluster[numOfNewClusters]->leftX = static_cast<int>(
//                                                      max( newCluster[numOfNewClusters-1]->leftX +
//                                                           newCluster[numOfNewClusters-1]->width,
//                                                           floor( x1 / siteWidth )));
//            }
//            else
//            {
//              newCluster[numOfNewClusters]->leftX = static_cast<int>(floor( x1 / siteWidth ));
//            }
//          }
//          else
//          {
//            newCluster[numOfNewClusters]->leftX = static_cast<int>(floor( x1 / siteWidth ));
//          }
//          newCluster[numOfNewClusters]->leftX = max(newCluster[numOfNewClusters]->leftX, 0);
//          newCluster[numOfNewClusters]->leftX = min(newCluster[numOfNewClusters]->leftX,
//                                            static_cast<int>(rowLen -
//                                            newCluster[numOfNewClusters]->width));
//          leftBorder = newCluster[numOfNewClusters]->leftX;
//          for (t = 0; t < static_cast<int>(newCluster[numOfNewClusters]->cellIdxs.size()); ++t)
//          {
//            currCellIdx = newCluster[numOfNewClusters]->cellIdxs[t];
//            placement[currCellIdx].xCoord = leftBorder + nodes[currCellIdx].width / 2.0;
//            leftBorder += nodes[currCellIdx].width;
//          }
//        }
//        else
//        {
//          numOfNewClusters++;
//          newCluster[numOfNewClusters] = oldCluster[j];
//        }
//        ++j;
//      }
//      numOfOldClusters = numOfNewClusters;
//      
//      // Rearrange oldCluster:
//      for (k = 0; k <= numOfNewClusters; ++k)
//        oldCluster[k] = newCluster[k];
//      
//      // Determine if there is overlap among oldCluster
//      isAnyOverlap = false;
//      for (k = 1; k <= numOfNewClusters; ++k)
//      {
//         if ((oldCluster[k]->leftX) -
//             (oldCluster[k-1]->leftX + oldCluster[k-1]->width) < 0)
//         {
//           isAnyOverlap = true;
//           break;
//         }
//      }
//    } while (isAnyOverlap);
//    // Change placement[].xCoords of cells in current row according
//    // to oldCluster[i] (i = 0,1,...,numOfOldClusters)
//    /*for (k = 0; k <= numOfOldClusters; ++k)
//    {
//      leftBorder  = oldCluster[k]->leftX;
//      for (t = 0; t < static_cast<int>(oldCluster[k]->cellIdxs.size()); ++t)
//      {
//        currCellIdx = oldCluster[k]->cellIdxs[t];
//        placement[currCellIdx].xCoord = leftBorder + nodes[currCellIdx].width / 2.0;
//        leftBorder += nodes[currCellIdx].width;
//      }
//    }*/
//  }
//  for (i = 0; i < maxNumOfCellsInRow; ++i)
//  {
//    delete delOldCluster[i];
//    delete delNewCluster[i];
//  }
//  delete []oldCluster;
//  delete []newCluster;
//  delete []delOldCluster;
//  delete []delNewCluster;
//}
//

void QuickSortRowElement(RowElement* a, long N)
{
  long i = 0, j = N;
  RowElement temp, p;
  int rand0;
  GetIntegerRandomNumbers( &rand0, 1, 0, N);

  p.xCoord = a[rand0].xCoord;
  p.cellIdx = a[rand0].cellIdx;

  do {
    while ( a[i].xCoord < p.xCoord ) i++;
    while ( a[j].xCoord > p.xCoord ) j--;

    if (i <= j)
    {
      temp.xCoord   = a[i].xCoord;
      a[i].xCoord   = a[j].xCoord;
      a[j].xCoord   = temp.xCoord;

      temp.cellIdx = a[i].cellIdx; 
      a[i].cellIdx = a[j].cellIdx; 
      a[j].cellIdx = temp.cellIdx;

      i++; j--;
    }
  } while ( i<=j );

  if ( j > 0 ) QuickSortRowElement(a, j);
  if ( N > i ) QuickSortRowElement(a+i, N-i);
}

void InsertSortRowElement(RowElement* a, long DataCount)
{
  RowElement temp;
  for(int i = 1, j; i < DataCount ; i++)
  {
    temp = a[i];
    for (j = i - 1; j > -1; j--)
      if(a[j].xCoord > temp.xCoord)
      {
        a[j+1] = a[j];
      }
      else break;
    a[j+1] = temp;
  }
}

void NextPermutation (unsigned int array[], unsigned int len)
 
{
  unsigned int k = len - 2;

  while (k && ( array[k] > array[k + 1])) k--;

  if ((k == 0) && (array[0] > array[1]))
  {
    for (unsigned int i = 0; i < len / 2; ++i)
    {
      k = array[i];
      array[i] = array[len - 1 - i];
      array[len - 1 - i] = k;
    }
    return;
  }
  unsigned int t = k + 1;

  while ((t < len - 1) && (array[t + 1] > array[k])) ++t;

  unsigned int c = array[k];

  array[k] = array[t]; array[t] = c;

  unsigned int n = (len - k - 1)>>1;

  for (unsigned int i = 0; i < n; ++i)
  {
    c = array[k + 1 + i];
    array[k + 1 + i] = array[len - i - 1];
    array[len - i - 1] = c;

  }
}
int  Factorial (unsigned int a)
{
  int temp = 1;
  for (unsigned int i = 1; i <= a; ++i)
    temp *= i;
  return temp;
}

MULTIPLACER_ERROR Legalize(Circuit& circuit)
{
  const int NUM_OF_ROWS_IN_LEGALIZATION = circuit.nRows; // parameter for Legalization

  // Array of cells sorted by their x-coord
  RowElement *sortedCells;
  double *rightBordersOfRows;
  double *rowLengths;
  double circuitDiagonal = sqrt(circuit.height * circuit.height + circuit.width * circuit.width);
  double currX = 0.0;
  double currY = 0.0;
  double newX  = 0.0;
  double newY  = 0.0;
  double bestX = 0.0;
  double bestY = 0.0;
  double halfWidth = 0.0;  // half width of current cell
  double distance;
  double minDistance = 0.0;
  int currCell;
  int newRowIdx = 0;
  int currRowIdx = 0;
  int firstRowIdx = 0;  // index of the first row the cell could be potentially placed in
  int lastRowIdx = 0;   // index of the last  row the cell could be potentially placed in
  int i, j;
  
  siteWidth  = circuit.rows[0].siteWidth;
  siteHeight = circuit.rows[0].height;
  numOfSites = circuit.rows[circuit.nRows/2].numSites;
  rowLen     = siteWidth * numOfSites;
  
  rightBordersOfRows = new double[circuit.nRows];
  rowLengths = new double[circuit.nRows];
  sortedCells = new RowElement[circuit.nNodes];
  for (i = 0; i < circuit.nRows; ++i)
  {
    rightBordersOfRows[i] = 0.0;
    rowLengths[i] = 0.0;
  }
  for (i = 0; i < circuit.nNodes; ++i)
  {
    sortedCells[i].cellIdx = i;
    sortedCells[i].xCoord  = circuit.placement[i].xCoord;
  }
  QuickSortRowElement(sortedCells, circuit.nNodes - 1);
  for (i = 0; i < circuit.nNodes; ++i)
  {
    currCell  = sortedCells[i].cellIdx;
    halfWidth = 0.5 * circuit.nodes[currCell].width;
    currX     = circuit.placement[currCell].xCoord - fmod(circuit.placement[currCell].xCoord - halfWidth, siteWidth);
    currY     = circuit.placement[currCell].yCoord;
    currRowIdx  = static_cast<int>( floor(currY / siteHeight) );
    firstRowIdx = max(currRowIdx - NUM_OF_ROWS_IN_LEGALIZATION, 0);
    lastRowIdx  = min(currRowIdx + NUM_OF_ROWS_IN_LEGALIZATION, circuit.nRows - 1);
    minDistance = circuitDiagonal;
    
    for (j = firstRowIdx; j <= lastRowIdx; ++j)
    {
      newX = max(rightBordersOfRows[j] + halfWidth, currX);
      newX = min(circuit.width - halfWidth, newX);
      if (rowLengths[j] + 2 * halfWidth > circuit.width)
        continue;
      newY = (j + 0.5) * siteHeight;
      
      distance = sqrt((newX - currX) * (newX - currX) + (newY - currY) * (newY - currY));
      if (minDistance > distance)
      {
        minDistance = distance;
        bestX = newX;
        bestY = newY;
      }
    }
    circuit.placement[currCell].xCoord = bestX;
    circuit.placement[currCell].yCoord = bestY;
    newRowIdx = static_cast<int>( floor(bestY / siteHeight) );
    rightBordersOfRows[newRowIdx] = bestX + halfWidth;
    rowLengths[newRowIdx] += 2 * halfWidth;
  }

  RowElement *currRow;
  int numOfCellsInRow;
  for (i = 0; i < circuit.nRows; ++i)
  {
    if (CheckOverlaps(circuit, i) != OK)
    {
      numOfCellsInRow = 0;
      for (int k = 0; k < circuit.nNodes; ++k)
      {
        if (circuit.rows[i].coordinate == circuit.placement[k].yCoord - 0.5 * circuit.nodes[k].height)
        {
          ++numOfCellsInRow;
        }
      }
      currRow = new RowElement[numOfCellsInRow];
      numOfCellsInRow = 0;
      for (int k = 0; k < circuit.nNodes; ++k)
      {
        if (circuit.rows[i].coordinate == circuit.placement[k].yCoord - 0.5 * circuit.nodes[k].height)
        {
          currRow[numOfCellsInRow].xCoord =
            circuit.placement[k].xCoord + 0.5 * circuit.nodes[k].width;
          currRow[numOfCellsInRow++].cellIdx = k;
        }
      }
      if ((numOfCellsInRow != 0) && (numOfCellsInRow != 1))
        QuickSortRowElement(currRow, numOfCellsInRow - 1);
      
      for (j = numOfCellsInRow - 1; j > 0; --j)
      {
        if (currRow[j].xCoord - circuit.nodes[currRow[j].cellIdx].width < currRow[j-1].xCoord)
        {
          currRow[j-1].xCoord = currRow[j].xCoord - circuit.nodes[currRow[j].cellIdx].width;
          circuit.placement[currRow[j-1].cellIdx].xCoord = currRow[j-1].xCoord - 
            0.5 * circuit.nodes[currRow[j-1].cellIdx].width;
        }
        else
          break;
      }
      delete[] currRow;
    }
  }

  for (int k = 0; k < circuit.nNodes; ++k)
  {
    if (circuit.placement[k].xCoord - circuit.nodes[k].width / 2 < 0)
      break;
  }
  
  delete[] rowLengths;
  delete[] sortedCells;
  delete[] rightBordersOfRows;
    
  return OK;
}