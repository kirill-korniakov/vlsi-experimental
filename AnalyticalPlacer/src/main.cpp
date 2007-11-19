/* 
* main.cpp
* this is a part of itlDragon
* Copyright (C) 2005-2006, ITLab, Kornyakov, Kurina, Zhivoderov
* email: kirillkornyakov@yandex.ru
* email: zhivoderov.a@gmail.com
* email: nina.kurina@gmail.com
*/

#include <iostream>
#include <conio.h>
#include <algorithm>
#include "..\include\parser.h"
#include "..\include\global_placement.h"
#include "..\include\directed_bin_swapping.h"
#include "..\include\simulated_annealing.h"
#include "..\include\overlap_removing.h"
#include "..\include\detailed_placement.h"
#include "..\include\output.h"
#include "..\include\random_numbers_generation.h"
#include "..\include\check_legality.h"
#include "..\include\errors.h"
#include "..\include\global.h"

using namespace DetailedPlacementNS;

void main(int argc, char* argv[])
{  
  Circuit    circuit;
  Statistics statistics;

  ITLDRAGON_ERROR errorCode = OK;

  clock_t startTime  = 0;
  clock_t finishTime = 0;
  clock_t start  = 0;
  clock_t finish = 0;
  double oldWL = 0.0;
  double newWL = 0.0;

  CMDParse(argc, argv);

  // initialize all data structures, parse benchmark and so on
  errorCode = Initialization(circuit, statistics);
  CheckCode(errorCode);

  // we shift point of origin to the bottom left corner of placement area
  ShiftCoords(circuit);
  statistics.currentWL = cf_recalc_all(1, circuit.nNets, circuit.nets, circuit.placement);
  
  PrintCircuitInfo(circuit);

  if (gOptions.doCheckLegality)
  {// if itlDragon is run with -check key
    cout << "Pin-to-Pin HPWL: " << statistics.currentWL << "\n\n";  
    errorCode = CheckLegalityOfPlacement(circuit);
    CheckCode(errorCode);
    Exit();
  }

  if (gOptions.doConvertToRouter == true)
  {
    cout << "Save nets to router format..." << endl;
    PrintNetsToRouterFormat(circuit);
    cout << "File " << gOptions.GRFileName << " written successful" << endl;
    Exit();
  }

  PrintToTmpPL(circuit, statistics);

  startTime = clock();

  //*************** G L O B A L   P L A C E M E N T ********************//
  if (!gOptions.doGlobalPlacement)  
  { 
    if (gOptions.doOverlapRemoving) // otherwise executed with key -dp
    {
      errorCode = CreateBinGrid(circuit);
      CheckCode(errorCode);
    }
  }
  else
  {
    cout << "itlDragon started   G L O B A L   P L A C E M E N T\n";
    GlobalPlacement(circuit);
    cout << "G L O B A L   P L A C E M E N T   S U C C E E D\n";
    PrintToTmpPL(circuit, statistics);
  }

  // dump GP if needed
  if (gOptions.doDumpGP)
  {
    statistics.currentWL = cf_recalc_all(1, circuit.nNets, circuit.nets, circuit.placement);

    finishTime = clock();
    statistics.totalWT = (double)(finishTime - startTime) / CLOCKS_PER_SEC;

    char ts[256];
    strcpy(ts, gOptions.benchmarkName);
    PrintToPL(strcat(ts,"_GP.pl"), circuit, statistics);
    if (gOptions.onlyGP)
    {
      CheckCode(EXIT_AFTER_RB_NEEDED);
    }
  }
    
  // update nets' lengths
  statistics.currentWL = cf_recalc_all(1, circuit.nNets, circuit.nets, circuit.placement);

  //*************** D E T A I L E D   P L A C E M E N T **************//
  if (gOptions.doDetailedPlacement/* = false*/)
  {
    cout<<"\nitlDragon started   D E T A I L E D   P L A C E M E N T\n";

    Legalize(circuit);
    errorCode = CheckLegalityOfPlacement(circuit);
    if (errorCode != OK)
    {
      for (int i = 0; i < circuit.nNodes; ++i)
      {
        circuit.placement[i].xCoord = circuit.placement[i].xCoord - fmod(circuit.placement[i].xCoord, circuit.rows[0].siteWidth);
      }
      Legalize(circuit);
    }
    PrintToTmpPL(circuit);
    //statistics.currentWL = cf_recalc_all(0, circuit.nNets, circuit.nets, circuit.placement);
    //cout << statistics.currentWL;
    //CheckOverlaps(circuit);
    errorCode = CheckLegalityOfPlacement(circuit);
    
    CheckCode(errorCode);
    
    char ts[256];
    strcpy(ts, gOptions.benchmarkName);
    statistics.currentWL = cf_recalc_all(0, circuit.nNets, circuit.nets, circuit.placement);
    if (gOptions.doOverlapRemoving) // otherwise executed with key -dp
    {
      PrintToPL(strcat(ts, "_BEFORE_DP.pl"), circuit, statistics);
    }

    errorCode =  DetailedPlacement( circuit, statistics);
    CheckCode(errorCode);
    cout << "D E T A I L E D   P L A C E M E N T   S U C C E E D\n";
  }

  statistics.currentWL = cf_recalc_all(0, circuit.nNets, circuit.nets, circuit.placement);

  finishTime = clock();
  statistics.totalWT = (double)(finishTime - startTime) / CLOCKS_PER_SEC;

  // checking the legality of resulting placement
  errorCode = CheckLegalityOfPlacement(circuit);
  CheckCode(errorCode);

  //*************************** O U T P U T ************************//
  cout << "\nWork Time:\t " << statistics.totalWT << "\n";
  cout << "\nWire length:\t " << statistics.currentWL << "\n";

  PrintToPL(strcat(gOptions.benchmarkName,".pl"), circuit, statistics);
  //PrintPLT (strcat(gOptions.benchmarkName,"t"), circuit, statistics);
  gOptions.benchmarkName[strlen(gOptions.benchmarkName) - 3] = '\0';

  PrintResultString(argc, argv, statistics, errorCode);

  if (gOptions.doTest)
  {
    CreateHTMLReport(statistics.currentWL, statistics.totalWT, errorCode);
  }

  Exit();
}
