/*  
* GESW.h
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#ifndef _GE_SW_H_
#define _GE_SW_H_

//#include <conio>
#include "SlidingWindow.h"
#include "GlobalExpertSolver.h"
//#include <mpi.h>

using namespace GlobalExpert;

//WindowWithNotOrderedCells   *slidingWindow;
//HDesign          hd;
//HDPGrid          DPGrid(hd);   
//double          *x_array;      //array of window bounds
//double           currWL;
//int              nChangedNets; // number of changed nets (i.e. nets captured by window)
//int             *changedNets;

void GlobalExpertOptimization(HDPGrid &_grid, int _argc, char** _argv);
void GlobalExpertOptimizationDebug(HDPGrid &_grid, int _argc, char** _argv);
double __cdecl function(const double * y, int num_of_function);

#endif
