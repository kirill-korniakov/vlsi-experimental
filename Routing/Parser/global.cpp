/* 
 * global.cpp
 * this is a part of itlDragon
 * Copyright (C) 2005-2006, ITLab, Kornyakov, Kurina, Zhivoderov
 * email: kirillkornyakov@yandex.ru
 * email: woody-quaker@yandex.ru
 * email: nina.kurina@gmail.com
 */

#include "global.h"
#include "parser.h"

Options gOptions;

const char* resultFileName = "itlDragon results.txt";

Tccout ccout;

// for time measurement
const double CLOCK_RATE = 2.8E9;  // clock rate of our processors
__int64 rngBSTotalTime = 0;
__int64 rngCATotalTime = 0;
__int64 rngLITotalTime = 0;

int rnCATotalCount = 0;

void InitializeStatistics(Statistics& statistics)
{
  statistics.statisticsFileName[0] = '\0';

  statistics.currentWL = 0.0;

  statistics.directedBinSwappingWLI = 0.0;
  statistics.cellAnnealingWLI       = 0.0;
  statistics.localImprovementWLI    = 0.0;

  statistics.totalWT                = 0.0;
  statistics.recursiveBisectionWT   = 0.0;
  statistics.binSwappingWT          = 0.0;
  statistics.adjustmentStepWT       = 0.0;
  statistics.directedBinSwappingWT  = 0.0;
  statistics.cellAnnealingWT        = 0.0;
  statistics.localImprovementWT     = 0.0;
}

ITLDRAGON_ERROR InitializeCircuit(Circuit& circuit)
{
  ITLDRAGON_ERROR errorCode = OK;

  cout << "itlDragon started to parse \"" << gOptions.benchmarkName << "\"\n";
  errorCode = ParseAux(gOptions.benchmarkName, circuit);
  if (errorCode != OK) return errorCode;
  cout << "Files parsed successfully.\n";

  if(!gOptions.isLEFDEFinput)
  {
  // calculating sizes of circuit
  circuit.height = circuit.nRows * circuit.rows[0].height;
  circuit.width  = circuit.rows[0].siteWidth * circuit.rows[circuit.nRows/2].numSites;
  
  circuit.shiftX = static_cast<int>(circuit.rows[circuit.nRows/2].subrowOrigin);
  circuit.shiftY = circuit.rows[0].coordinate;
  }

  circuit.rowTerminalBorders = new std::vector<double>[circuit.nRows];
  
  MakeTableOfConnections(circuit);

  return errorCode;
}

void SetDefaultKeysValues()
{
  gOptions.isLEFDEFinput         = false;
  gOptions.doRecursiveBisection  = true;
  gOptions.doBinSwapping         = true;
  gOptions.doDirectedBinSwapping = true;
  gOptions.doCellAnnealing       = true;
  gOptions.doOverlapRemoving     = true;
  gOptions.doDetailedPlacement   = true;
  gOptions.doCheckLegality       = false;

  gOptions.doLog                 = true;
  gOptions.doTest                = false;

  gOptions.doDumpGP              = false;
  gOptions.onlyGP                = false;

  gOptions.benchmarkName[0] = '\0';
  gOptions.plName[0] = '\0';
  gOptions.configName[0] = '\0';
}

void CheckCode(ITLDRAGON_ERROR errorCode)
{
  if (errorCode == OK) return;

  char errorMessage[256];
  PrintErrorMessage( errorMessage, errorCode );
  cout << errorMessage;

  char errorString[128];
  if (errorCode)
  {
    FILE *resultFile;
    resultFile = fopen(resultFileName, "a");
    strcpy( errorString, "benchmark: \t%s\t " );
    strcat( errorString, errorMessage);
    if(resultFile)
    {
      fprintf( resultFile, errorString, gOptions.benchmarkName);
    }
    fclose (resultFile);
    Exit();
  }
}

void PrintErrorMessage(char* errorMsg, int errorCode)
{
  if (errorCode != 0)
  {
    char errorCodeStr[256];
    int firstDigit   = static_cast<int>( errorCode / 100 );
    int errorSubCode = errorCode % 100;
    char stageName[128];
    switch (firstDigit)
    {
      case 0: strcpy( stageName, "parsing"); break;
      case 1: strcpy( stageName, "global placement"); break;
      case 2: strcpy( stageName, "bin-swapping"); break;
      case 3: strcpy( stageName, "adjustment step"); break;
      case 4: strcpy( stageName, "directed bin-swapping"); break;
      case 5: strcpy( stageName, "cell annealing"); break;
      case 6: strcpy( stageName, "overlap_removing"); break;
      case 7: strcpy( stageName, "detailed placement"); break;
      case 8: strcpy( stageName, "legality cheking"); break;
      case 9: strcpy( stageName, "output"); break;
    }
    strcpy( errorMsg, "Program aborted because of error during ");
    strcat( errorMsg, stageName );
    strcat( errorMsg, ". Error code: " );
    itoa( errorCode, errorCodeStr, 10 );
    strcat( errorCodeStr, "\n" );
    strcat( errorMsg, errorCodeStr );
  }
  else
    strcpy( errorMsg, "OK\n" );
  //strcpy( errorMsg, "\nNo errors detected while programm execution\nDeleting memory...\n" );
}

ITLDRAGON_ERROR Initialization(Circuit& circuit, Statistics& statistics)
{
  ITLDRAGON_ERROR errorCode = OK;

  ccout.Start();
  
  // initialize VSL stream for random numbers
  //InitializeStream(5574564);
  //srand(14580);
  //InitializeStream();
  //srand(clock());
  
  InitializeStatistics(statistics);
  errorCode = InitializeCircuit(circuit);
  if (errorCode != OK) return errorCode;

  for (int i = 0; i < NUM_STAGES; ++i)
    for (int j = 0; j < MAX_NUM_PARAMS; ++j)
      gOptions.innerParameters[i][j] = -1.0;  
  
  return OK;
}

void FreeMemory(Circuit circuit)
{     
  // deleting memory
  if (circuit.rows)     { delete []circuit.rows;      }
  if (circuit.nets)     { delete []circuit.nets;      }
  if (circuit.nodes)    { delete []circuit.nodes;     }
  if (circuit.terminals){ delete []circuit.terminals; }
  if (circuit.weights)  { delete []circuit.weights;   }
  if (circuit.placement){ delete []circuit.placement; }
  
  // delete bins
  if(circuit.arrOfBins)
  {
    for(int i = 0; i < circuit.nBinRows; ++i)
      for(int j = 0; j < circuit.nBinCols; ++j)
        delete circuit.arrOfBins[i][j];
    for(int i = 0; i < circuit.nBinRows; i++ )
      delete[] circuit.arrOfBins[i];
    delete[] circuit.arrOfBins;
  }
}

void Exit()
{
  //FreeMemory(); // we do not free memory
  //DeinitializeStream();
  exit(0);
}

void MakeTableOfConnections(Circuit& circuit)
{
  circuit.tableOfConnections = new std::vector<int>[circuit.nNodes + circuit.nTerminals];
  for (int i = 0; i < circuit.nNets; ++i)
  {
    for (int j = 0; j < circuit.nets[i].numOfPins; ++j)
      circuit.tableOfConnections[circuit.nets[i].arrPins[j].cellIdx].push_back(i);
  }
}