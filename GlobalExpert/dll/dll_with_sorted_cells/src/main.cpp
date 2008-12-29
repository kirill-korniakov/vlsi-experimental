/* 
* main.cpp
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2008, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#include <iostream>
#include <conio.h>
#include <algorithm>
#include <string>
#include <vector>
#include <math.h>
#include "..\include\parser.h"
#include "..\include\output.h"
#include "..\include\check_legality.h"
#include "..\include\errors.h"
#include "..\include\global.h"
#include "..\include\abs_detailed_placement.h"

//Window *slidingWindow;
WindowWithOrderedCells *slidingWindow;
double *x_array;
Circuit    circuit;

int ReadBounds(char fileName[], double *&boundsArray, char *&auxName, int &firstRow )
{
  FILE *f = fopen(fileName, "r");
  int stringSize = 100;
  int k1 = 0;
  int k2 = 0;

  if (f == NULL)
  {
    cout<<"File with bounds not found!\n";
    return 0;
  }
  
  char *str = new char [stringSize];
	auxName = new char [stringSize];
  char *num = new char [stringSize];

  for (int i = 0; i < stringSize; i++)
  {
    str[i] = '\0';
    num[i] = '\0';
		auxName[i] = '\0';
  }

  fgets(str, stringSize - 1, f); //getting aux file name

	for (int i = 0; str[i] != '\n'; i++)
		auxName[i] = str[i];
	
	fgets(str, stringSize - 1, f); //getting number of rows

  while ((str[k1] != ':') && (k1 < stringSize))
    k1++;

  k1 += 2;

  while ((k1 < stringSize) && (isdigit(str[k1]) ))
    num[k2++] = str[k1++];
  
  int rowsNum = atoi(num);
  //cout<<"num: "<<rowsNum<<endl; ///
  boundsArray = new double [rowsNum * 2];

  fgets(str, stringSize - 1, f); //getting index of first row
  for (int j = 0; j < k2 + 1; j++)
		num[j] = '\0';

  k1 = 0;
  k2 = 0;

  while ((str[k1] != ':') && (k1 < stringSize))
	  k1++;

  k1 += 2;

  while ((k1 < stringSize) && (isdigit(str[k1]) ))
    num[k2++] = str[k1++];
  
  firstRow = atoi(num);
  cout<<"firstRow: "<<firstRow<<endl;

  for (int i = 0; i < rowsNum * 2;) //getting bounds of rows
  {
    for (int j = 0; j < k2 + 1; j++)
		num[j] = '\0';

    k1 = 0;
    k2 = 0;
    fgets(str, stringSize - 1, f);

    if (str[0] == 'e')
    {
      cout<<"incorrect file\n";
      delete [] str;
      delete [] num;
      return 0;
    }

    k1 = 0;
    k2 = 0;
    
    while ((k1 < stringSize) && (str[k1] != ':'))
      k1++;

    k1 += 3;

    while ((k1 < stringSize) && (isdigit(str[k1]) ))
      num[k2++] = str[k1++];

    //cout<<num<<endl;
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
  
int Initialize()
{
    Statistics statistics;
    int _argc = 3;
    char **_argv = new char* [_argc];
    _argv[0] = "detailed_placement";
    _argv[1] = "-f";
    //_argv[2] = "abt01.aux";

    char abtBoundsFile[] = "benchmark_info.txt";
    int firstRow = 0;
    int rows_num = ReadBounds(abtBoundsFile, x_array, _argv[2], firstRow);

    MULTIPLACER_ERROR errorCode = OK;
    CMDParse(_argc, _argv);

    if (gOptions.calcTimingFileName[0] != '\0')
    {        
        if (gOptions.defName[0] != '\0')
        {            
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
    //  statistics.currentWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);

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

    if (gOptions.isLEFDEFinput && gOptions.convert2BookshelfName[0] != '\0')
    {
        LEFDEF2Bookshelf(gOptions.convert2BookshelfName, circuit);
        cout << "Writing bookshelf from LEFDEF to " << gOptions.convert2BookshelfName << endl;
        Exit();
    }

    PrintToTmpPL(circuit, statistics);

    //*************************** O U T P U T ************************// 
    int *all_elements = new int [circuit.nNodes];

    for(int i = 0; i < circuit.nNodes; i++)
        all_elements[i] = i;
    
    //slidingWindow = new Window( rows_num, 1, x_array, circuit.nNodes, circuit, all_elements);
    slidingWindow  = new WindowWithOrderedCells( rows_num, firstRow, x_array, circuit.nNodes, circuit, all_elements);
    //system("PAUSE");
    /*slidingWindow->GetCoords(circuit);
    slidingWindow->elements[0].XCoord += 2;

    for(int i = 0; i < slidingWindow->elementsNum; i++)
        cout<<"X["<<i<<"]= "<<slidingWindow->elements[i].XCoord<<"; ";*/
    
    /*cout<<endl<<slidingWindow->boundX[2]<<endl;
    cout<<endl<<slidingWindow->CalcBoundsPenalty(circuit)<<endl;    
    cout<<slidingWindow->CalcWireLenght(circuit, x_array)<<endl;
    cout<<slidingWindow->CalcFreeSpace(circuit);
    for (int i = 0; i < slidingWindow->elementsNum; i++)
      cout<<"site["<<i<<"]= "<<slidingWindow->spaces[i]<<"; ";*/

    return 0;
}

int main() {
    Initialize();
    return 0;
}





///////////////////////////////////////////////////////////////////////////////
////                                                                         //
////                    NIZHNY NOVGOROD STATE UNIVERSITY                     //
////                                                                         //
////                 Copyright (c) 1999-2006 by A. Sysoyev.                  //
////                          All Rights Reserved.                           //
////                                                                         //
////  File:      ${DLL_NAME}.cpp                                             //
////                                                                         //
////  Purpose:   implementation of all involved functionals                  //
////                                                                         //
////  Author(s): Vinogradov                                                  //
////                                                                         //
///////////////////////////////////////////////////////////////////////////////

#define PREFIX_EXPORT extern "C" __declspec(dllexport)
#define M_PI 3.1415926535897932384626433832795

int DIMENSION = circuit.nNodes; 
bool first_time = true;

#define MAX_PATH 260
static char DllWorkingDir[MAX_PATH];
static char ExternalWorkingDir[MAX_PATH];

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
    return 2; ///3
}

//----------------------------------------------------------------------------
PREFIX_EXPORT int __cdecl getNumberOfConstraints()
{
    return 1; ///2
}

// ---------------------------------------------------------------------------
PREFIX_EXPORT bool __cdecl getOrderOfCriteria(int* order)
{
    if (order == NULL)
        return false;
    order[0] = 1; ///2
    return true;
}

PREFIX_EXPORT bool __cdecl getOrderOfConstraints(int* order)
{
    if (order == NULL)
        return false;
    order[0] = 0;
    ///order[1] = 1;
    return true;
}

// ---------------------------------------------------------------------------
PREFIX_EXPORT bool __cdecl getDomain(double* a, double* b)
{
    if (a != NULL && b != NULL)
    {
        for (int i = 0; i < DIMENSION; ++i)
        {
            a[i] = 0; b[i] = slidingWindow->CalcFreeSpace(circuit);
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
            strcpy(name, "WindowBounds");
        if (desc != NULL)
            strcpy(desc, "constraint 1");
        break;    
    case 1:
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
    double res = 0.0;   

    for(int i = 0; i < slidingWindow->elementsNum; i++) {
      slidingWindow->spaces[i] = y[i]; //elments[i].XCoord
    }

    switch (num_of_function) {
    case 0: // constraint 1 - geometry
      res = slidingWindow->CalcBoundsPenalty(circuit); break; ///g1WindowBounds(circuit)

    case 1: // criterion - HPWL
      res = slidingWindow->CalcWireLenght(circuit, x_array); break; ///g2Overlaps(circuit)

    /*case 2: // criterion - HPWL
        res = slidingWindow->CalcWL(circuit, x_array); break;*/

    default:
        res = 0; break;
    }

    _chdir(ExternalWorkingDir);
    return res;
}

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
  return 7;
}

//----------------------------------------------------------------------------
PREFIX_EXPORT bool __cdecl getInputFiles(char** names)
{
  if (names == NULL)
  return false;
  strcpy(names[0], "abt03\\abt03.aux");
  strcpy(names[1], "abt03\\abt03.nets");
  strcpy(names[2], "abt03\\abt03.nodes");
  strcpy(names[3], "abt03\\abt03.pl");
  strcpy(names[4], "benchmark_info.txt");
  strcpy(names[5], "abt03\\abt03.scl");
  strcpy(names[6], "abt03\\abt03.wts");

  return true;
}


// ---------------------------------------------------------------------------
