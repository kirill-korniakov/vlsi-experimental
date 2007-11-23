/*  
 * detailed_placement.h
 * this is a part of itlDragon
 * Copyright (C) 2005, ITLab, Zhivoderov
 * email: zhivoderov.a@gmail.com
 */

#ifndef _DETAILED_PLACEMENT_H_
#define _DETAILED_PLACEMENT_H_

#include <iostream>
#include "..\include\data_structures.h"
#include "..\include\cost_function.h"
#include "..\include\random_numbers_generation.h"
#include "..\include\check_legality.h"
#include "..\include\optimal_region_determination.h"
#include "..\include\output.h"
#include "..\include\global.h"
#include <time.h>    //for timing
#include "math.h"
//#include "mkl.h"
//#include <windows.h>

//#define _OPT_REG_INFO_
//#define FULL_WL_NOTATION

struct RowElement
{
  double xCoord;
  int  cellIdx;
};

namespace DetailedPlacementNS
{
  struct Cluster
  {
    int leftX;
    double width;
    vector<int> cellIdxs;
    vector<double> boundsList;
  };
  typedef Cluster* pCluster;
}
enum HorSearchDir{ LEFT, RIGHT, RANDOM };

MULTIPLACER_ERROR DetailedPlacement(Circuit& circuit, Statistics& statistics);

MULTIPLACER_ERROR Legalize(Circuit& circuit);

double HorizontalSearch(HorSearchDir direction, RowElement** greedy_array, int* numOfCellsInRow, int row, 
        int number, Circuit& circuit, Statistics& statistics, int** arrOfSites);

double VerticalSearch(int currCellIdx, int* numOfCellsInRow, int** arrOfSites, Circuit& circuit, Statistics& statistics);

double GlobalSwap(int currCellIdx, int* numOfCellsInRow, int** arrOfSites,
        Circuit& circuit, Statistics& statistics);

void QuickSortRowElement(RowElement* a, long N);

void InsertSortRowElement(RowElement* a, long DataCount);

void NextPermutation(unsigned int x[], unsigned int len);

int  Factorial(unsigned int a);

#endif