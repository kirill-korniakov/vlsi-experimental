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
#include <direct.h>
#include "..\include\parser.h"
#include "..\include\output.h"
#include "..\include\check_legality.h"
#include "..\include\errors.h"
#include "..\include\global.h"
#include "..\include\abs_detailed_placement.h"
#include "GlobalExpertSolver.h"
#include "mpi.h"

#define numOfCells 5
#define numOfSteps 10
#define numOfRows 3

using namespace GlobalExpert;

GeneralWindow   *slidingWindow;
Circuit          circuit;
double          *x_array;      //array of window bounds
double           currWL;
int              nChangedNets; // number of changed nets (i.e. nets captured by window)
int             *changedNets;

// ---------------------------------------------------------------------------
double __cdecl function(const double * y, int num_of_function)
{
  double res = 0.0;

  for(int i = 0; i < slidingWindow->elementsNum; i++)
  {
	  slidingWindow->elements[i].XCoord = y[i];
    //slidingWindow->spaces[i] = y[i];  //for window with sorted cells
  }

  switch (num_of_function) {
  case 0: // constraint 1 - geometry
	  res = slidingWindow->g1WindowBounds(circuit); //slidingWindow->CalcBoundsPenalty(circuit); //
	  break;

  case 1: // constraint 2 - overlaps
	  res = slidingWindow->g2Overlaps(circuit); //slidingWindow->CalcWireLenght(circuit, x_array); //
	break;

  case 2: // criterion - HPWL
    res = currWL = slidingWindow->RecalcWl(circuit, currWL, changedNets, nChangedNets);
    //slidingWindow->CalcWL(circuit); 
  break;

  default:
    res = 0; break;
  }

  return res;
}


#define MAX_PATH 260
static char DllWorkingDir[MAX_PATH];
static char ExternalWorkingDir[MAX_PATH];

int ReadBounds(char fileName[], double *&boundsArray, char *&auxName, int &firstRow ) {
  FILE *f = fopen(fileName, "r");
  int stringSize = 100;
  int k1 = 0;
  int k2 = 0;

  if (f == NULL) {
    //cout<<"File with bounds was not found!\n";
    printf("File with bounds was not found!\n");
    return 0;
  }
  
  char *str = new char [stringSize];
	auxName = new char [stringSize];
  char *num = new char [stringSize];

  for (int i = 0; i < stringSize; i++) {
    str[i] = '\0';
    num[i] = '\0';
		auxName[i] = '\0';
  }

  fgets(str, stringSize - 1, f); //getting aux file name
	for(int i = 0; str[i] != '\n'; i++)
		auxName[i] = str[i];
	
	fgets(str, stringSize - 1, f); //getting number of rows

  while((str[k1] != ':') && (k1 < stringSize))
    k1++;

  k1 += 2;

  while ((k1 < stringSize) && (isdigit(str[k1]) ))
    num[k2++] = str[k1++];
  
  int rowsNum = atoi(num);  
  boundsArray = new double [rowsNum * 2];

  fgets(str, stringSize - 1, f); //getting index of first row
  for(int j = 0; j < k2 + 1; j++)
		num[j] = '\0';

  k1 = 0;
  k2 = 0;

  while((str[k1] != ':') && (k1 < stringSize))
	  k1++;

  k1 += 2;

  while ((k1 < stringSize) && (isdigit(str[k1]) ))
    num[k2++] = str[k1++];
  
  firstRow = atoi(num);

  for(int i = 0; i < rowsNum * 2;) { //getting bounds of rows
    for(int j = 0; j < k2 + 1; j++)
		num[j] = '\0';

    k1 = 0;
    k2 = 0;
    fgets(str, stringSize - 1, f);

    if(str[0] == 'e') {
      //cout<<"incorrect file\n";
      delete [] str;
      delete [] num;
      return 0;
    }

    k1 = 0;
    k2 = 0;
    
    while((k1 < stringSize) && (str[k1] != ':'))
      k1++;

    k1 += 3;

    while ((k1 < stringSize) && (isdigit(str[k1]) ))
      num[k2++] = str[k1++];
    
    boundsArray[i++] = atof(num); //first bound for this row    

		for(int j = 0; j < k2 + 1; j++)
			num[j] = '\0';

    k2 = 0;
    k1 += 2;

    while ((k1 < stringSize) && (isdigit(str[k1]) ))
      num[k2++] = str[k1++];

    boundsArray[i++] = atoi(num); //second bound for this row     
  }
  
  delete [] str;
  delete [] num;
  fclose(f);
  return rowsNum;
}

int Initialize(int task_number)
{

  Statistics statistics;
  int _argc = 3;
  char **_argv = new char* [_argc];
  _argv[0] = "detailed_placement";
  _argv[1] = "-f";
  int firstRow = 0;
  //_argv[2] = "abt01.aux";

  char abtBoundsFile[256]; // = "benchmark_info.txt";
  sprintf_s(abtBoundsFile, "abt0%d\\benchmark_info.txt", task_number);
  int rows_num = ReadBounds(abtBoundsFile, x_array, _argv[2], firstRow);
  MULTIPLACER_ERROR errorCode = OK;
  CMDParse(_argc, _argv);

  if (gOptions.calcTimingFileName[0] != '\0')
  {
    //if (gOptions.isLEFDEFinput)
    //ParseDEF(gOptions.calcTimingFileName, circuit);
    //else
    if (gOptions.defName[0] != '\0')
    {
      //ParseLEFDEF(gOptions.lefName, gOptions.defName, circuit);
      ParseAux(gOptions.benchmarkName, circuit);
      ParsePl(gOptions.calcTimingFileName, circuit);
      MakeTableOfConnections(circuit);
    }

    exit(1);
  }

  // initialize all data structures, parse benchmark and so on
  errorCode = Initialization(circuit, statistics);
  CheckCode(errorCode);

  // we shift point of origin to the bottom left corner of placement area
  ShiftCoords(circuit);
  statistics.currentWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);
	//cout<<"current WL: "<<statistics.currentWL<<endl;
  printf("current wire length: %f\n", statistics.currentWL);

  PrintCircuitInfo(circuit);

  if (gOptions.doCheckLegality)
  {// if itlAnalyticalPlacer is run with -check key
    //cout << "Pin-to-Pin HPWL: " << statistics.currentWL << "\n\n";  
    errorCode = CheckLegalityOfPlacement(circuit);
    CheckCode(errorCode);
    Exit();
  }

  if (gOptions.doConvertToRouter == true)
  {
    //cout << "Save nets to router format..." << endl;
    PrintNetsToRouterFormat(circuit);
    //cout << "File " << gOptions.GRFileName << " written successful" << endl;
    Exit();
  }

  if (gOptions.isLEFDEFinput && gOptions.convert2BookshelfName[0] != '\0')
  {
    LEFDEF2Bookshelf(gOptions.convert2BookshelfName, circuit);
    //cout << "Writing bookshelf from LEFDEF to " << gOptions.convert2BookshelfName << endl;
    Exit();
  }

  PrintToTmpPL(circuit, statistics);

  //*************************** O U T P U T ************************//  
  return 0;
}


int main(int argc, char* argv[])
{
  Initialize(2);
  MPI_Init(&argc, &argv);

  currWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);
  printf("WL before GE: %f\n", currWL);
  int *siteIndexes = NULL;
  int *cellIndexes = NULL;
  int firstRowIdx;

  for (int step = 0; step < numOfSteps; step++)
  {    
//-------------------Parameters	for Global Expert------------------------------
    TGlobExpSolverParams GE_solver;
    GE_solver.NumOfConstraints = 2;
    GE_solver.OrderOfConstraints = new int[2]; //2 for general task
    GE_solver.OrderOfConstraints[0] = 0;
    GE_solver.OrderOfConstraints[1] = 1;       //
    GE_solver.SetMultiFunction(function);
    GE_solver.NumOfCriteria = 1;
    GE_solver.OrderOfCriteria = new int[1];
    GE_solver.OrderOfCriteria[0] = 2;
    GE_solver.IterMax = 20000;
    GE_solver.L = 1;
    GE_solver.MethodID = 1;
    GE_solver.m = 12;
    GE_solver.r = 1.9;
    GE_solver.qReservParam = 0.01; // 1000.0 - for tasks with fixed order of cells
    GE_solver.eps = 0.001; // 0.0001
    GE_solver.EnableOptionalLoggers();

//----------------Generating slidingWindow-------------------------------------
    x_array = NULL;
    siteIndexes = NULL;
    cellIndexes = NULL;
    int nCells = numOfCells;
    bool isWindowCreated = false;
    firstRowIdx = rand() % circuit.nRows;
    int nRows = min(circuit.nRows - firstRowIdx, numOfRows);
    GetStartPoint(siteIndexes, nRows, circuit);
    isWindowCreated = GetWindowParams(firstRowIdx, nRows, nCells, siteIndexes,
                                      cellIndexes, x_array, circuit);

    if (isWindowCreated == false) // no cells were captured by this window
       continue;

    bool *nodeFromThisNet = new bool [circuit.nNets]; // == true if there is a cell form
                                                      //    this net in the window

    for (int l = 0; l < circuit.nNets; l++)
      nodeFromThisNet[l] = false;

    nChangedNets = 0;

    for (int netInd = 0; netInd < circuit.nNets; netInd++)
    {
      for (int j = 0; j < circuit.nets[netInd].numOfPins; j++)
      {
        for (int k = 0; k < nCells; k++)
        {
          if (cellIndexes[k] == circuit.nets[netInd].arrPins[j].cellIdx)
          {
            nodeFromThisNet[netInd] = true;
            break;
          }
        }

        if (nodeFromThisNet[netInd] == true)
        {
          nChangedNets++; // this net was captured by the window
          break;          // check others
        }
      }

      changedNets = new int [nChangedNets]; //indexes of nets captured by the window
      int currSize = 0;

      for (int k = 0; k < circuit.nNets; k++)
      {
        if (nodeFromThisNet[k] == true)
        {
          changedNets[currSize++] = k;
        }
      }
    }

    slidingWindow = new GeneralWindow(nRows, firstRowIdx, x_array, nCells, circuit, cellIndexes);
    printf("bounds penalty: %f\n", slidingWindow->g1WindowBounds(circuit));
    printf("overlaps: %f\n", slidingWindow->g2Overlaps(circuit));

//-------------------------------------------------------------------------------------------
    GE_solver.N = nCells;
    GE_solver.a = new double[GE_solver.N];
    GE_solver.b = new double[GE_solver.N];
    GE_solver.firstPoint = new double [GE_solver.N];

    for (int i = 0; i < GE_solver.N; ++i)
    {
      GE_solver.a[i] = 0;
      GE_solver.b[i] = slidingWindow->boundX[slidingWindow->nRows]; //slidingWindow->CalcFreeSpace(circuit);
      GE_solver.firstPoint[i] = slidingWindow->elements[i].XCoord;
    }

    double* y = new double[GE_solver.N];
    double* f = new double[GE_solver.NumOfConstraints + GE_solver.NumOfCriteria];
    TGlobExpResults searchResults;
    searchResults.foundY = new double [GE_solver.N];
    char error_msg[MAX_PATH_LENGTH];
    GE_solver.processLogging = true;

    if (!GE_solver.Solve(searchResults, error_msg))
    {
      printf("\n\n----- Error: %s", error_msg);
    }
    else
    {
      printf("\n\n+++++ Minimum was found!\n");

      printf("y* = [ ");
      for (int i = 0; i < GE_solver.N; ++i)
      {
        printf("%.3f", y[i]);
        if (i < GE_solver.N - 1)
        {
         printf(", ");
        }
      }

      printf(" ]\nf* = (");
      for (int i = 0; i < GE_solver.NumOfConstraints + GE_solver.NumOfCriteria; ++i)
      {
        printf("%.3f", f[i]);
        if (i < GE_solver.NumOfConstraints + GE_solver.NumOfCriteria - 1)
        {
          printf(", ");
        }
      }
      printf(" )\n");
    }

    slidingWindow->PlaceCells(circuit);
    currWL = slidingWindow->RecalcWl(circuit, currWL, changedNets, nChangedNets);
    currWL = slidingWindow->CalcWL(circuit);

    delete slidingWindow;
    delete [] x_array;
    delete [] changedNets;
    delete [] siteIndexes;
    delete [] cellIndexes;
    delete [] nodeFromThisNet;
    x_array = NULL;
    changedNets = NULL;
    //delete [] GE_solver.OrderOfConstraints;
    //delete [] GE_solver.OrderOfCriteria;
    //delete [] GE_solver.a;
    //delete [] GE_solver.b;
    //delete [] GE_solver.firstPoint;
    printf("WL: %.10lf\n", currWL);
    _getch();
  }

  printf("WL after GE: %f\n", cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement));
  printf("\n\nPress any key to exit...\n");
  MPI_Finalize();
  _getch();
  return 0;
}



#ifdef WITHOUT_SOLVER

#include <string>
#include <vector>
#include <math.h>
#include <windows.h>

#define PREFIX_EXPORT extern "C" __declspec(dllexport)
#define M_PI 3.1415926535897932384626433832795

int DIMENSION = 9; 
bool first_time = true;

// ---------------------------------------------------------------------------
PREFIX_EXPORT int __cdecl getDimension()
{
    return DIMENSION;
}

// ---------------------------------------------------------------------------
PREFIX_EXPORT bool __cdecl setDimension(int dimension)
{
    if (dimension > 1)
    {
        DIMENSION = dimension;
        return true;
    }
    else
        return false;
}

// ---------------------------------------------------------------------------
PREFIX_EXPORT int __cdecl getNumberOfFunctions()
{
  return 3;
}

//----------------------------------------------------------------------------
PREFIX_EXPORT int __cdecl getNumberOfConstraints()
{
    return 2;
}

// ---------------------------------------------------------------------------
PREFIX_EXPORT bool __cdecl getOrderOfCriteria(int* order)
{
    if (order == NULL)
        return false;
    order[0] = 2;
    return true;
}

PREFIX_EXPORT bool __cdecl getOrderOfConstraints(int* order)
{
    if (order == NULL)
        return false;
    order[0] = 0;
    order[1] = 1;
    return true;
}

// ---------------------------------------------------------------------------
PREFIX_EXPORT bool __cdecl getDomain(double* a, double* b)
{
    if (a != NULL && b != NULL)
    {
        for (int i = 0; i < DIMENSION; ++i)
        {
            a[i] = 1; b[i] = 21;
        }
        return true;
    }
    else
    {
        return false;
    }
}

// ---------------------------------------------------------------------------
PREFIX_EXPORT void __cdecl 
getDescription(char* name, char* desc, int num_of_function)
{
    switch (num_of_function)
    {
    case 0:
        if (name != NULL)
            strcpy(name, "Geometry");
        if (desc != NULL)
            strcpy(desc, "constraint 1");
        break;
    case 1:
        if (name != NULL)
            strcpy(name, "Overlaps");
        if (desc != NULL)
            strcpy(desc, "constraint 2");
        break;
    case 2:
        if (name != NULL)
            strcpy(name, "HPWL");
        if (desc != NULL)
            strcpy(desc, "criterion");
        break;
    }
}

// ---------------------------------------------------------------------------
PREFIX_EXPORT double __cdecl function(const double * y, int num_of_function)
{
  _getcwd(ExternalWorkingDir, 256);
  _chdir(DllWorkingDir);

  double res;

  for(int i = 0; i < slidingWindow->elementsNum; i++) {
    slidingWindow->elements[i].xRecalculated = y[i];
  }

  switch (num_of_function) {
    case 0: // constraint 1 - geometry
      res = slidingWindow->g1Geometry(circuit); break;

    case 1: // constraint 2 - overlaps
      res = slidingWindow->g2Overlaps(circuit); break;

    case 2: // criterion - HPWL
      res = slidingWindow->CalcWL(circuit); break;

    default:
      res = 0; break;
  }

  _chdir(ExternalWorkingDir);

  return res;
}

// ---------------------------------------------------------------------------
PREFIX_EXPORT void _cdecl init_task_dll()
{
  _getcwd(ExternalWorkingDir, 256);
  _chdir(DllWorkingDir);

  if(first_time)
  {
    Initialize();
    first_time = false;
  }

  _chdir(ExternalWorkingDir);
}

//----------------------------------------------------------------------------
PREFIX_EXPORT void __cdecl setDllWorkingDir(const char* work_dir)
{
  strcpy(DllWorkingDir, work_dir);
}

//----------------------------------------------------------------------------
PREFIX_EXPORT int __cdecl getNumberOfInputFiles()
{
  return 6;
}

//----------------------------------------------------------------------------
PREFIX_EXPORT bool __cdecl getInputFiles(char** names)
{
  if (names == NULL)
    return false;
  strcpy(names[0], "abt01\\abt01.aux");
  strcpy(names[1], "abt01\\abt01.nets");
  strcpy(names[2], "abt01\\abt01.nodes");
  strcpy(names[3], "abt01\\abt01.pl");
  strcpy(names[4], "abt01\\abt01.scl");
  strcpy(names[5], "abt01\\abt01.wts");

  return true;
}

#endif // WITHOUT_SOLVER
