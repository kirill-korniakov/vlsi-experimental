/* 
* main.cpp
* this is a part of itlDragon
* Copyright (C) 2005-2006, ITLab, Kornyakov, Kurina, Zhivoderov
* email: kirillkornyakov@yandex.ru
* email: woody-quaker@yandex.ru
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
  {// if itlDragon runned with -check key
    cout << "Pin-to-Pin HPWL: " << statistics.currentWL << "\n\n";  
    errorCode = CheckLegalityOfPlacement(circuit);
    CheckCode(errorCode);
    Exit();
  }

  PrintToTmpPL(circuit, statistics);

  startTime = clock();

  //*************** G L O B A L   P L A C E M E N T ********************//
  if (!gOptions.doRecursiveBisection)  
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

    // vertical and horizontal distance between centers of adjacent bins
    circuit.binsHeight = circuit.height;
    circuit.binsWidth  = circuit.width;

    // place all cells into the center of the circuit
    for(int i = 0; i < circuit.nNodes; ++i)
    {
      circuit.placement[i].xCoord = circuit.width/2;
      circuit.placement[i].yCoord = circuit.height/2;
    }

    // do recursive bisection
    errorCode = RecursiveBisection(circuit, statistics);
    CheckCode(errorCode);
    
    cout << "G L O B A L   P L A C E M E N T   S U C C E E D\n";
    cout << "Bins height:\t" << circuit.binsHeight << "\n";
    cout << "Bins width:\t"  << circuit.binsWidth  << "\n"; 
  
    PrintToTmpPL(circuit, statistics);
  
    //***************** A D J U S T M E N T   S T E P *********************//
    cout << "\nitlDragon started   A D J U S T M E N T   S T E P\n";
    int counter = 0;

    statistics.currentWL = cf_recalc_all(1, circuit.nNets, circuit.nets, circuit.placement);
    cout << "WL before AS: " << statistics.currentWL << "\n";

    do
    {
      counter++;
      oldWL = cf_recalc_all(1, circuit.nNets, circuit.nets, circuit.placement);

      // perform adjustment
      errorCode = AdjustmentStep(circuit);
      CheckCode(errorCode);

      newWL = cf_recalc_all(1, circuit.nNets, circuit.nets, circuit.placement);

      cout << "WL after iteration #" << counter << ":\t" << newWL << "\t"
           << "Reduction: " << (1 - newWL/oldWL)*100 << "%\n";
    } while ((counter == 1) || (1 - newWL/oldWL > 0.0001));

    statistics.currentWL = cf_recalc_all(1, circuit.nNets, circuit.nets, circuit.placement);
    PrintToTmpPL(circuit, statistics);
    cout << "A D J U S T M E N T   S T E P   S U C C E E D\n";
    
    //***************** A D J U S T  B I N S  T O  M A C R O S **************//
    cout<<"\nitlDragon started   A D J U S T  B I N S  T O  M A C R O S\n";
    oldWL = statistics.currentWL = cf_recalc_all(1, circuit.nNets, circuit.nets, circuit.placement);
    //PrintToPL("before_AdjustBinsToMacros", circuit, statistics);
    errorCode = AdjustBinsToMacros(circuit);
    CheckCode(errorCode);
    statistics.currentWL = cf_recalc_all(1, circuit.nNets, circuit.nets, circuit.placement);
    cout << "WL before bins adjusting to macros: " << oldWL << "\n";
    cout << "WL after  bins adjusting to macros: " << statistics.currentWL << "\n";
    cout << "Reduction of WL: " << (1 - statistics.currentWL / oldWL) * 100 << "%\n";
    PrintToTmpPL(circuit, statistics);
    
    //*************** D I R E C T E D   B I N - S W A P P I N G *************//
    //gOptions.doDirectedBinSwapping = false;
    if (gOptions.doDirectedBinSwapping)
    {
      cout << "\nitlDragon started   D I R E C T E D   B I N - S W A P P I N G\n";

      oldWL = statistics.currentWL;
      
      start = clock();
      errorCode = DirectedBinSwapping(circuit, statistics, gOptions.innerParameters[2]);
      CheckCode(errorCode);
      finish = clock();
      statistics.directedBinSwappingWT = (double)(finish - start) / CLOCKS_PER_SEC;

      cout << "Directed bin-swapping was successfully finished!\n";
      cout << "Directed bin-swapping work time: " << statistics.directedBinSwappingWT << "\n";

      statistics.currentWL = cf_recalc_all(1, circuit.nNets, circuit.nets, circuit.placement);
      cout << "Total reduction: " << (1 - statistics.currentWL/oldWL)*100 << "%\n";
    }
    /*PrintToPL("before_EqualizeRows", circuit, statistics);
    errorCode = EqualizeRowLengths(circuit);
    CheckCode(errorCode);*/
  }
   // end of RecursiveBisection
  //EqualizeRowLengths(circuit);
  // dump GP if needed (this procedure will slightly change WL)
  if (gOptions.doDumpGP)
  {
    // make coordinates integer for future comparison
    for (int i = 0; i < circuit.nNodes; ++i)
      circuit.placement[i].xCoord = dtoi(circuit.placement[i].xCoord);

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
  /*for (int rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
  {
    cout << static_cast<int>(circuit.arrOfBins[rowIdx][0]->nodes.size()) << endl;
  }*/
  
  //*************** C E L L   A N N E A L I N G **************//
  //gOptions.doCellAnnealing = false;
  if (gOptions.doCellAnnealing)
  {
    cout<<"\nitlDragon started   C E L L   A N N E A L I N G\n";
    statistics.currentWL = cf_recalc_all(1, circuit.nNets, circuit.nets, circuit.placement);
    cout << "WL before bins width adjusting: " << statistics.currentWL << "\n";

    double* rowLengths;  // sum lengths of cells in rows  
    rowLengths = new double [circuit.nBinRows];

    // we calculate initial values for lengths of rows and lengths of bins
    for (int rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
    {
      rowLengths[rowIdx] = 0;
      for (int colIdx = 0; colIdx < circuit.nBinCols; ++colIdx)
      {
        circuit.arrOfBins[rowIdx][colIdx]->sumLength = 0;
        for (int i = 0; i < static_cast<int>(circuit.arrOfBins[rowIdx][colIdx]->nodes.size()); ++i)
        {
          rowLengths[rowIdx] += circuit.nodes[circuit.arrOfBins[rowIdx][colIdx]->nodes[i]].width;
          circuit.arrOfBins[rowIdx][colIdx]->sumLength += 
            circuit.nodes[circuit.arrOfBins[rowIdx][colIdx]->nodes[i]].width; 
        }
      }
    }
    
    int rowIdx;
    double binHeight = circuit.rows[0].height;
    double leftBorder;
    double rightBorder;
    
    for (int rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
      circuit.rowTerminalBorders[rowIdx].push_back(0.0);
    
    for (int i = 0; i < circuit.nTerminals; ++i)
    {
      rowIdx = static_cast<int>((circuit.placement[i + circuit.nNodes].yCoord - 
                0.5 * circuit.terminals[i].height) / binHeight);
      
      leftBorder  = circuit.placement[i + circuit.nNodes].xCoord - 0.5 * circuit.terminals[i].width;
      rightBorder = circuit.placement[i + circuit.nNodes].xCoord + 0.5 * circuit.terminals[i].width;
      
      if (rowIdx < 0 || rowIdx >= circuit.nBinRows || leftBorder >= circuit.width || rightBorder < 0)
        continue;
      
      for (int j = rowIdx; j < min(rowIdx + circuit.terminals[i].height / binHeight, circuit.nBinRows); ++j)
      {
        circuit.rowTerminalBorders[j].push_back(leftBorder);
        circuit.rowTerminalBorders[j].push_back(rightBorder);
      }
    }
    
    for (int rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
    {
      sort(circuit.rowTerminalBorders[rowIdx].begin(), circuit.rowTerminalBorders[rowIdx].end());
      circuit.rowTerminalBorders[rowIdx].push_back(circuit.width);
    }
    
    AdjustBinWidths(circuit, rowLengths);
    delete [] rowLengths;
    PrintToTmpPL(circuit, statistics);

    statistics.currentWL = cf_recalc_all(1, circuit.nNets, circuit.nets, circuit.placement);
    cout << "WL after  bins width adjusting: " << statistics.currentWL << "\n";

    errorCode = SimulatedAnnealing(circuit, statistics);
    CheckCode(errorCode);
    PrintToTmpPL(circuit, statistics);
    PrintToPL("after CA", circuit, statistics);
    cout << "C E L L   A N N E A L I N G   S U C C E E D\n";
  }
  //***************** O V E R L A P  R E M O V I N G *******************//
  if (gOptions.doOverlapRemoving)
  {
    //DistribCellsAmongBins( circuit);
    cout<<"\nitlDragon started   O V E R L A P   R E M O V I N G\n";

    int counter = 0;
    statistics.currentWL = oldWL = cf_recalc_all( 1, circuit.nNets, circuit.nets, circuit.placement);
    cout << "WL before OR: " << statistics.currentWL << "\n";
    errorCode = OverlapRemoving(circuit);
    newWL = cf_recalc_all( 1, circuit.nNets, circuit.nets, circuit.placement);
    CheckCode(errorCode);
    cout << "WL after iteration #" << counter << ":\t" << newWL << "\t"
      << "Reduction: " << (1 - newWL / oldWL)*100 << "%\n";
    
    PrintToTmpPL(circuit, statistics);
    cout << "O V E R L A P   R E M O V I N G   S U C C E E D\n";
  }
  
  // update nets' lengths
  statistics.currentWL = cf_recalc_all(1, circuit.nNets, circuit.nets, circuit.placement);

  //*************** D E T A I L E D   P L A C E M E N T **************//
  if (gOptions.doDetailedPlacement/* = false*/)
  {
    cout<<"\nitlDragon started   D E T A I L E D   P L A C E M E N T\n";

    Legalize(circuit);
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
