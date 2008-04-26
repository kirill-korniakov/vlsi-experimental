/* 
* main.cpp
* this is a part of itlAnalyticalPlacer
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
#include "..\include\overlap_removing.h"
#include "..\include\detailed_placement.h"
#include "..\include\output.h"
#include "..\include\random_numbers_generation.h"
#include "..\include\check_legality.h"
#include "..\include\errors.h"
#include "..\include\global.h"

using namespace DetailedPlacementNS;

static  char help[] = "This example demonstrates use of the TAO package to \n\
                      solve an unconstrained minimization problem on a single processor. \n";

int main(int argc, char* argv[])
{  
  Circuit    circuit;
  Statistics statistics;

  MULTIPLACER_ERROR errorCode = OK;

  clock_t startTime  = 0;
  clock_t finishTime = 0;
  clock_t start  = 0;
  clock_t finish = 0;
  double oldWL = 0.0;
  double newWL = 0.0;
  char ts[256];

  CMDParse(argc, argv);

  // initialize all data structures, parse benchmark and so on
  errorCode = Initialization(circuit, statistics);
  CheckCode(errorCode);

  PetscInitialize(&argc,&argv,(char *)0,help);
  TaoInitialize(&argc,&argv,(char *)0,help);

  int info;       // used to check for functions returning nonzeros
  int size,rank;  // number of processes running
  info = MPI_Comm_size(PETSC_COMM_WORLD,&size); CHKERRQ(info);
  info = MPI_Comm_rank(PETSC_COMM_WORLD,&rank); CHKERRQ(info);

  if (size >1) {
    if (rank == 0)
      PetscPrintf(PETSC_COMM_SELF,"This application is intended for single processor use!\n");
    SETERRQ(1,"Incorrect number of processors");
  }

  // we shift point of origin to the bottom left corner of placement area
  ShiftCoords(circuit);
  statistics.currentWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);
  
  PrintCircuitInfo(circuit);

  if (gOptions.doCheckLegality)
  {// if itlAnalyticalPlacer is run with -check key
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
      //errorCode = CreateBinGrid(circuit);
      CheckCode(errorCode);
    }
  }
  else
  {
    cout << "itlAnalyticalPlacer started   G L O B A L   P L A C E M E N T\n";
#ifdef RECURSIVE_BISECTION
    RecursiveBisection(circuit, statistics);
#endif
    GlobalPlacement(circuit);
    cout << "G L O B A L   P L A C E M E N T   S U C C E E D\n";
    PrintToTmpPL(circuit, statistics);
  }

  // dump GP if needed
  if (gOptions.doDumpGP)
  {
    statistics.currentWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);

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
  statistics.currentWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);

  //*************** D E T A I L E D   P L A C E M E N T **************//
  if (gOptions.doDetailedPlacement/* = false*/)
  {
    //ExportDEF("dma.def", circuit);
    cout << "\nitlAnalyticalPlacer started   D E T A I L E D   P L A C E M E N T\n";

    cout << "currWL before legalization: " << statistics.currentWL << endl;
    Legalize(circuit);
    errorCode = CheckLegalityOfPlacement(circuit);
    if (errorCode != OK)
    {
      double halfWidth;
      for (int i = 0; i < circuit.nNodes; ++i)
      {
        halfWidth = 0.5 * circuit.nodes[i].width;
        circuit.placement[i].xCoord = circuit.placement[i].xCoord - 
                                      fmod(circuit.placement[i].xCoord - halfWidth, circuit.rows[0].siteWidth);
      }
      Legalize(circuit);
    }
    PrintToTmpPL(circuit);
    //statistics.currentWL = cf_recalc_all(0, circuit.nNets, circuit.nets, circuit.placement);
    //cout << statistics.currentWL;
    //CheckOverlaps(circuit);
    errorCode = CheckLegalityOfPlacement(circuit);
    
    CheckCode(errorCode);
    
    strcpy(ts, gOptions.benchmarkName);
    statistics.currentWL = cf_recalc_all(0, circuit.nNets, circuit.nets, circuit.placement);
    //if (gOptions.doOverlapRemoving) // otherwise executed with key -dp
    {
      PrintToPL(strcat(ts, "_BEFORE_DP.pl"), circuit, statistics);
      strcpy(ts, gOptions.benchmarkName);
    }

    errorCode =  DetailedPlacement( circuit, statistics);
    CheckCode(errorCode);
    cout << "D E T A I L E D   P L A C E M E N T   S U C C E E D\n";
  }

  statistics.currentWL = cf_recalc_all(0, circuit.nNets, circuit.nets, circuit.placement);

  ComputeNetWeights(circuit);
  DumpNetWeights(strcat(ts, ".nwts"), circuit);

  // checking the legality of resulting placement
  errorCode = CheckLegalityOfPlacement(circuit);
  CheckCode(errorCode);

  finishTime = clock();
  statistics.totalWT = (double)(finishTime - startTime) / CLOCKS_PER_SEC;

  //*************************** O U T P U T ************************//
  cout << "\nWork Time:\t " << statistics.totalWT << "\n";
  cout << "\nWire length:\t " << statistics.currentWL << "\n";

  PrintToPL(strcat(gOptions.benchmarkName,".pl"), circuit, statistics);
  //PrintPLT (strcat(gOptions.benchmarkName,"t"), circuit);
  gOptions.benchmarkName[strlen(gOptions.benchmarkName)-3] = '\0';

  PrintResultString(argc, argv, statistics, errorCode);

  if (gOptions.doTest)
  {
    CreateHTMLReport(statistics.currentWL, statistics.totalWT, errorCode);
  }
  
  /* Finalize TAO */
  TaoFinalize();
  PetscFinalize();

  Exit();

  return 0;
}