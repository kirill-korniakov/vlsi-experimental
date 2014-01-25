/*  
* GESW.cpp
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/


//FIXME: disabled because of compilation error
#if 0

#include "GESW.h"
#include "Legalization.h"
#include "mpi.h"
//#include <math.h>

using namespace GlobalExpert;

WindowWithNotOrderedCells *slidingWindow;
double currWL;

// ---------------------------------------------------------------------------
double __cdecl function(const double * y, int num_of_function)
{
  double res = 0.0;    
  slidingWindow->ChangeCoords(y);


  switch (num_of_function)
  {
  case 0: // constraint 1 - geometry
    res = slidingWindow->g1WindowBounds();
    break;

  case 1: // constraint 2 - overlaps      
    res = slidingWindow->g2Overlaps();
    break;

  case 2: // criterion - HPWL
    //res = slidingWindow->RecalcWL(currWL);
    res = slidingWindow->CalcWL();
    break;

  default:
    res = 0;
    break;
  }

  return res;
}


//#define MAX_PATH 260
//static char DllWorkingDir[MAX_PATH];
//static char ExternalWorkingDir[MAX_PATH];

void GlobalExpertOptimization(HDPGrid &_grid, int _argc, char** _argv)
{
  MPI_Init(&_argc, &_argv);
  currWL = Utils::CalculateHPWL(_grid.Design(), false);
  printf("WL before GE: %f\n", currWL);
  double x_array[numOfRows * 2];
  int siteIndexes[numOfRows];
  HCell capturedCells[numOfCells];

  for (int step = 0; step < numOfSteps; step++)
  {
    double wlBeforeGE;
    //-------------------Parameters	for Global Expert------------------------------
    GlobalExpertSolver GE_solver;
    GE_solver.NumOfConstraints = 2;
    GE_solver.OrderOfConstraints = new int[2]; //2 for general task
    GE_solver.OrderOfConstraints[0] = 0;
    GE_solver.OrderOfConstraints[1] = 1;       //
    GE_solver.SetMultiFunction(function);
    GE_solver.NumOfCriteria = 1;
    GE_solver.OrderOfCriteria = new int[1];
    GE_solver.OrderOfCriteria[0] = 2;
    GE_solver.IterMax = 50;
    GE_solver.L = 1;
    GE_solver.MethodID = 1;
    GE_solver.m = 12;
    GE_solver.r = 1.9;
    GE_solver.qReservParam = 0.01; // 1000.0 - for tasks with fixed order of cells
    GE_solver.eps = 0.001;         // 0.0001
    GE_solver.EnableOptionalLoggers();

    //----------------Generating slidingWindow-------------------------------------                
    int firstRowIdx;
    bool isWindowCreated = false;
    int nFoundCells = numOfCells;
    firstRowIdx = std::rand() % (_grid.NumRows() - numOfRows); //9circuit.nRows;        
    int nRows = std::min(_grid.NumRows() - firstRowIdx, numOfRows);
    GetStartPoint(siteIndexes, nRows, _grid);
    isWindowCreated = GetWindowParams(firstRowIdx, nRows, nFoundCells, capturedCells, siteIndexes,
      x_array, _grid);

    if (nFoundCells <= 1)
      continue;

    slidingWindow = new WindowWithNotOrderedCells(nRows, firstRowIdx, x_array, nFoundCells, capturedCells, _grid);
    slidingWindow->PlaceCells();
    //wlBeforeGE = slidingWindow->RecalcWL(currWL);
    wlBeforeGE = slidingWindow->CalcWL();

    printf("bounds penalty: %f\n", slidingWindow->g1WindowBounds());
    printf("overlaps: %f\n", slidingWindow->g2Overlaps());
    printf("recalc WL: %f\n", wlBeforeGE);
    printf("nCells = %d\n", nFoundCells);        

    //-------------------------------------------------------------------------------------------
    GE_solver.N = nFoundCells;
    GE_solver.a = new double[GE_solver.N];
    GE_solver.b = new double[GE_solver.N];
    GE_solver.firstPoint = new double [GE_solver.N];

    for (int i = 0; i < GE_solver.N; ++i)
    {
      GE_solver.a[i] = 0;
      GE_solver.b[i] = slidingWindow->GetMaxCoord(); //slidingWindow->CalcFreeSpace(circuit);
      GE_solver.firstPoint[i] = slidingWindow->GetCellCoord(i);
    }

    //double* y = new double[GE_solver.N];
    //double* f = new double[GE_solver.NumOfConstraints + GE_solver.NumOfCriteria];
    TGlobExpResults searchResults;
    searchResults.foundY = new double [GE_solver.N];
    char error_msg[MAX_PATH_LENGTH];
    GE_solver.processLogging = true;

    printf("before\n");
    //_getch();

    if (false/*!GE_solver.Solve(searchResults, error_msg)*/)
    {
      printf("\n\n----- Error: %s", error_msg);
    }
    else
    {
      if (searchResults.maximumIndex == 3)
      {
        printf("\n\n+++++ Minimum was found!\n");

        printf("y* = [ ");
        for (int i = 0; i < GE_solver.N; ++i)
        {
          printf("%.3f", searchResults.foundY[i]);
          if (i < GE_solver.N - 1)
          {
            printf(", ");
          }
        }
        printf(" ]\nf* = %lf\n", searchResults.foundF);

        // update current placement
        if (searchResults.foundF > wlBeforeGE)
        {
          slidingWindow->ChangeCoords(GE_solver.firstPoint);
        }

        else
        {
          slidingWindow->ChangeCoords(searchResults.foundY);
        }
      }
      else
      {
        printf("\n\n----- Minimum was not found!\n");
        slidingWindow->ChangeCoords(GE_solver.firstPoint);
      }
    }

    slidingWindow->PlaceCells();
    currWL = slidingWindow->CalcWL();
    printf("bounds: %f\n", slidingWindow->g1WindowBounds());
    printf("overlaps: %f\n", slidingWindow->g2Overlaps());
    //delete [] GE_solver.firstPoint;
    //delete [] x_array;
    //delete [] siteIndexes;
    //delete [] capturedCells;

    //delete [] GE_solver.OrderOfConstraints;
    //delete [] GE_solver.OrderOfCriteria;
    //delete [] GE_solver.a;
    //delete [] GE_solver.b;

    printf("WL: %f\n", currWL);
    _grid.FindCellsPositions();
    //printf("WL: %f\n", Utils::CalculateHPWL(_grid.Design(), true));        
    CheckOverlaps(_grid, 100, true);
    delete slidingWindow;
    slidingWindow = NULL;
    //_getch();
  }
  //_getch();
}

void GlobalExpertOptimizationDebug(HDPGrid &_grid, int _argc, char** _argv)
{
  //MPI_Init(&_argc, &_argv);
  currWL = Utils::CalculateHPWL(_grid.Design(), false);
  printf("WL before GE: %f\n", currWL);
  //double x_array[numOfRows * 2];
  //int siteIndexes[numOfRows];
  //HCell capturedCells[numOfCells];

  for (int step = 0; step < numOfSteps; step++)
  {
    //double wlBeforeGE;
    //-------------------Parameters	for Global Expert------------------------------
    GlobalExpertSolver GE_solver;    
  }
}

#endif
